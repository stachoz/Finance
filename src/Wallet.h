#pragma once

#include "strategy/IStrategy.h"

class Wallet {
public:
    Wallet(double saldo_value, double shares_per_transaction_value)
        : saldo(saldo_value), shares_per_transaction(shares_per_transaction_value) {}

    void update(Signal signal, double price);

    [[nodiscard]] std::string get_string() const;

    double get_total_net_worth() const;

    double get_saldo() const;

    double get_owned_shares() const;

private:
    void buy(double price);

    void sell(double price);

    double calculate_shares_value() const;

    double saldo {0};
    double shares_per_transaction {0};
    double owned_shares {0};
    double last_price {0};

    std::vector<std::pair<double, double>> wallet_history; // market price, net-worth
};
