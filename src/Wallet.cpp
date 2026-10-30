#include "Wallet.h"

bool Wallet::update(Signal signal, double price, double transaction_size) {
    bool was_transaction_completed = true;

    switch (signal) {
        using enum Signal;
        case BUY:
            was_transaction_completed = buy(price, transaction_size);
            break;
        case SELL:
            was_transaction_completed = sell(price, transaction_size);
            break;
        case NONE:
            break;
    }

    wallet_history.emplace_back(price, saldo + calculate_shares_value(price));
    return was_transaction_completed;
}

std::string Wallet::get_string() const {
    std::stringstream ss;

    ss << "saldo: " << saldo << std::endl
        << "owned_shares: " << owned_shares << std::endl
        << "net_worth: " << get_last_total_net_worth() << std::endl;

    return ss.str();
}


bool Wallet::buy(double price, double transaction_size) {
    // buying for higher price (ask)
    const double execution_price = price + (spread / 2.0);
    const double total_cost = transaction_size * execution_price + calculate_commission(transaction_size);

    if (total_cost > saldo) {
        return false;
    }

    saldo -= total_cost;
    owned_shares += transaction_size;

    return true;
}

bool Wallet::sell(double price, double transaction_size) {
    if (owned_shares == 0) {
        return false;
    }

    // selling for lower price (bid)
    const double execution_price = price - (spread / 2.0);
    const double shares_to_sell = std::min(owned_shares, transaction_size);
    const double total_revenue = shares_to_sell * execution_price - calculate_commission(shares_to_sell);

    saldo += total_revenue;
    owned_shares -= shares_to_sell;

    return true;
}

double Wallet::calculate_shares_value(double current_price) const {
    return owned_shares * current_price;
}

double Wallet::calculate_commission(double shares_amount) const {
    return std::max(shares_amount * commission_per_share, 1.0);
}

double Wallet::get_saldo() const {
    return saldo;
}

double Wallet::get_initial_saldo() const {
    return initial_saldo;
}

double Wallet::get_owned_shares() const {
    return owned_shares;
}

double Wallet::get_spread() const {
    return spread;
}

double Wallet::get_commission_per_share() const {
    return commission_per_share;
}

double Wallet::get_last_total_net_worth() const {
    if (wallet_history.empty()) {
        return 0;
    }

    return wallet_history.back().second;
}

void Wallet::set_spread(double value) {
    spread = value;
}

void Wallet::set_commission_per_share(double value) {
    commission_per_share = value;
}

const std::vector<std::pair<double, double>> & Wallet::get_wallet_history() const {
    return wallet_history;
}
