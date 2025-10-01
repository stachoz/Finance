#pragma once

#include <filesystem>
#include <vector>
#include <fstream>

class CSVReader {
public:
    static std::vector<std::vector<std::string>> read(const std::filesystem::path& path, bool skip_header = true) {
        std::vector<std::vector<std::string>> result {};
        std::ifstream file(path);
        std::string line;

        if(!file.is_open()) {
            throw std::runtime_error("File could not be opened");
        }

        bool is_first_line = true;
        while(std::getline(file, line)) {
            if(skip_header && is_first_line) {
                is_first_line = false;
                continue;
            }

            std::istringstream iss(line);
            std::vector<std::string> row;
            std::string field;

            while(std::getline(iss, field, ',')) {
                row.push_back(field);
            }

            result.emplace_back(std::move(row));
        }

        return result;
    }
};
