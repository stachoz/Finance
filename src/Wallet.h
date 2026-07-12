#pragma once

#include "strategy/IStrategy.h"

class Wallet {
public:
    Wallet(double saldo_value, double shares_per_transaction_value)
        : saldo(saldo_value), shares_per_transaction(shares_per_transaction_value) {
        std::cout << "initial saldo: " << saldo_value << std::endl;
    }

    bool update(Signal signal, double price);

    [[nodiscard]] std::string get_string() const;

    double get_total_net_worth() const;

    double get_saldo() const;

    /// it is saldo after the first signal. It may be different from
    /// saldo passed as Wallet constructor parameter
    double get_initial_saldo() const;

    double get_owned_shares() const;

    double get_spread() const;

    double get_commission_per_share() const;

    const std::vector<std::pair<double, double>>& get_wallet_history() const;
private:
    bool buy(double price);

    bool sell(double price);

    double calculate_shares_value() const;

    double calculate_commission(double shares_amount) const;

    double spread = 0.02;

public:
    void set_spread(double spread);

    void set_commission_per_share(double commission_per_share);

private:
    double commission_per_share = 0.0035;
    double saldo {0};
    double shares_per_transaction {0};
    double owned_shares {0};

    std::vector<std::pair<double, double>> wallet_history; // market price, net-worth
};
