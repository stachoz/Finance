#pragma once
#include <vector>
#include <cmath>

namespace stats {

    /// \brief Calculates percentage returns based on given prices.
    /// \details Returns are one less than the of price points.
    /// \param values input sequence
    /// \return Vector of relative changes. Empty if fewer than 2 elements.
    /// Positive difference - price increased.
    /// Negative difference - price decreased.
    template<typename T>
    std::vector<T> returns(const std::vector<T>& values) {
        if(values.size() < 2) {
            return {};
        }

        std::vector<T> returns;
        returns.reserve(values.size() - 1);

        for(size_t i = 1; i < values.size(); ++i) {
            auto prev = values[i - 1];
            auto curr = values[i];

            returns.emplace_back(curr / prev - 1);
        }

        return returns;
    }

    /// \brief Calculates logarithmic returns from a sequence of numeric values.
    /// \param values input sequence
    /// \return Vector of log returns. Empty if fewer than 2 inputs.
    template<typename T>
    std::vector<T> log_returns(const std::vector<T>& values) {
        if(values.size() < 2) {
            return {};
        }

        std::vector<T> returns;
        returns.reserve(values.size() - 1);

        for(size_t i = 1; i < values.size(); ++i) {
            T prev = values[i - 1];
            T curr = values[i];

            returns.push_back(std::log(curr / prev));
        }

        return returns;
    }

    /// Calculates average of given data
    /// @tparam T numeric type of input values
    /// \param values input sequence
    /// @return mean value or 0 if the \a values is empty
    template<typename T>
    double mean(const std::vector<T>& values) {
        if(values.empty()) {
            return 0;
        }
        double sum = 0;
        for(const auto& v : values) {
            sum += v;
        }

        return sum / values.size();
    }

    /// \brief Computes a simple moving average over a sliding window.
    /// \tparam T numeric type of input values
    /// \param values Input sequence
    /// \param window windwo size (must be > 0)
    /// \return Vector of averages values, one for each full window.
    /// Empty if values.size() < window
    template<typename T>
    std::vector<double> moving_average(const std::vector<T>& values, int window) {
        if(window == 0 || values.empty() || values.size() < window) {
            throw std::invalid_argument("Invalid window size or values");
        }

        std::vector<double> rolling_mean;
        rolling_mean.reserve(values.size() - window + 1);

        double sum = 0;
        for(int i = 0; i < window; i++) {
            sum += values[i];
        }

        rolling_mean.emplace_back(sum / window);

        for(int i = window; i < values.size(); ++i) {
            sum += values[i] - values[i - window];
            rolling_mean.emplace_back(sum / window);
        }

        return rolling_mean;
    }

    /// @brief Computes the population variance of a dataset.
    /// @tparam T numeric type of input values
    /// @param values input sequence
    /// @return Variance of values.
    template<typename T>
    double variance(const std::vector<T>& values) {
        if(values.empty()) {
            throw std::invalid_argument("values cannot be empty");
        }

        double mean = stats::mean(values);
        double sumOfSquaredDiff = 0;

        for(auto& v : values) {
            sumOfSquaredDiff += std::pow(v - mean, 2);
        }

        return sumOfSquaredDiff / values.size();
    }

    /// @brief Computes the population standard deviation of a dataset.
    /// @tparam T numeric type of input values.
    /// @param values input sequence
    /// @return Standard deviation of values.
    template<typename T>
    double std_deviation(const std::vector<T>& values) {
        return std::sqrt(variance(values));
    }

    /// Covariance indicates the extent to which two random variables
    /// change together.
    /// @tparam T type of values in datasets
    /// @param x_dataset first dataset
    /// @param y_dataset second dataset
    /// @return the measure can be positive (one variable increases so does the other),
    /// negative (one variable decreases so does the other) or zero.
    template<typename T>
    double covariance(const std::vector<T>& x_dataset, const std::vector<T>& y_dataset) {
        if(x_dataset.size() != y_dataset.size()) {
            throw std::invalid_argument("Datasets must be equal of length");
        }

        double x_mean = mean(x_dataset);
        double y_mean = mean(y_dataset);

        double sum = 0;
        
        for(int i = 0; i < x_dataset.size(); ++i) {
            sum += (x_dataset[i] - x_mean) * (y_dataset[i] - y_mean); 
        }

        return sum / (x_dataset.size() - 1);
    }
};
