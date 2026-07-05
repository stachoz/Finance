#include "Wallet.h"

bool Wallet::update(Signal signal, double price) {
    last_price = price;

    bool was_transaction_completed = true;
    switch (signal) {
        case Signal::BUY:
            was_transaction_completed = buy(price);
            break;
        case Signal::SELL:
            was_transaction_completed = sell(price);
            break;
        case Signal::NONE:
            break;
    }

    wallet_history.emplace_back(price, get_total_net_worth());
    return was_transaction_completed;
}

std::string Wallet::get_string() const {
    std::stringstream ss;

    ss << "saldo: " << saldo << std::endl
        << "shares_per_transaction: " << shares_per_transaction << std::endl
        << "owned_shares: " << owned_shares << std::endl
        << "net_worth: " << get_total_net_worth() << std::endl;

    return ss.str();
}

double Wallet::get_total_net_worth() const {
    return saldo + calculate_shares_value();
}

double Wallet::get_saldo() const {
    return saldo;
}

double Wallet::get_owned_shares() const {
    return owned_shares;
}

const std::vector<std::pair<double, double>> & Wallet::get_wallet_history() const {
    return wallet_history;
}

bool Wallet::buy(double price) {
    const double transaction_value = shares_per_transaction * price;

    if (transaction_value > saldo) {
        return false;
    }

    saldo -= transaction_value;
    owned_shares += shares_per_transaction;

    return true;
}

bool Wallet::sell(double price) {
    if (owned_shares == 0) {
        return false;
    }

    const double shares_to_sell = std::min(owned_shares, shares_per_transaction);
    const double transaction_value = shares_to_sell * price;

    saldo += transaction_value;
    owned_shares -= shares_to_sell;

    return true;
}

double Wallet::calculate_shares_value() const {
    return owned_shares * last_price;
}
