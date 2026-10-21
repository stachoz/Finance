#pragma once

#include <charconv>
#include <filesystem>
#include <vector>
#include <fstream>
#include <functional>
#include <memory>

#include "TimeSeries.h"

namespace utils::csv {
    /// helper func to skip pointer after the coma
    inline const char* skip_to_next(const char* ptr, const char* end) {
        while (ptr < end && *ptr != ',') {
            ++ptr;
        }

        return (ptr < end) ? ptr + 1 : end;
    }


    inline std::vector<DOHLCV> read(const std::filesystem::path& path, bool contains_header = false) {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("File could not be opened");
        }

        std::vector<DOHLCV> result;
        result.reserve(10'000);

        std::string line;

        if (contains_header) {
            std::getline(file, line);
        }

        while (std::getline(file, line)) {
            const char* ptr = line.data();
            const char* end = line.data() + line.size();

            int y;
            unsigned m;
            unsigned d;

            std::from_chars(ptr, ptr + 4, y);
            std::from_chars(ptr + 5, ptr + 7, m);
            std::from_chars(ptr + 8, ptr + 10, d);

            std::chrono::year_month_day ymd {
                std::chrono::year{y},
                std::chrono::month{m},
                std::chrono::day{d}
            };

            ptr = skip_to_next(ptr, end);

            // open, high, low, close, volume
            std::array<double, 5> values {};

            for (auto& value : values) {
                auto [next_ptr, ec] = std::from_chars(ptr, end, value);
                if (ec != std::errc()) {
                    break;
                }
                ptr = skip_to_next(ptr, end);
            }

            result.emplace_back(ymd, values[0], values[1], values[2], values[3], values[4]);
        }

        return result;
    }

    template <typename T>
    struct is_unique_ptr : std::false_type {};

    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

    template <typename T, typename RowWriterCallable>
    void save_to_file(const std::vector<T>& data_to_save, const std::filesystem::path& output_path,
            RowWriterCallable row_writer, const std::vector<std::string>& header = {}) {
        std::ofstream file(output_path);
        if (!header.empty()) {
            for (size_t i = 0; i < header.size() - 1; ++i) {
                file << header[i] << ",";
            }
            file << header[header.size() - 1] << "\n";
        }

        for (const auto& value : data_to_save) {
            if constexpr (is_unique_ptr<T>::value) {
                std::invoke(row_writer, *value, file);
            } else {
                std::invoke(row_writer, value, file);
            }
        }
    }
};
