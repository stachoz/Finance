#include "Wallet.h"

bool Wallet::update(Signal signal, double price) {
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


bool Wallet::buy(double price) {
    // buying for higher price (ask)
    const double execution_price = price + (spread / 2.0);
    const double total_cost = shares_per_transaction * execution_price + calculate_commission(shares_per_transaction);

    if (total_cost > saldo) {
        return false;
    }

    saldo -= total_cost;
    owned_shares += shares_per_transaction;

    return true;
}

bool Wallet::sell(double price) {
    if (owned_shares == 0) {
        return false;
    }

    // selling for lower price (bid)
    const double execution_price = price - (spread / 2.0);
    const double shares_to_sell = std::min(owned_shares, shares_per_transaction);
    const double total_revenue = shares_to_sell * execution_price - calculate_commission(shares_to_sell);

    saldo += total_revenue;
    owned_shares -= shares_to_sell;

    return true;
}

double Wallet::calculate_shares_value() const {
    if (wallet_history.empty()) {
        return 0.0;
    }
    return owned_shares * wallet_history.back().first;
}

double Wallet::calculate_commission(double shares_amount) const {
    return std::max(shares_amount * commission_per_share, 1.0);
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

double Wallet::get_spread() const {
    return spread;
}

double Wallet::get_commission_per_share() const {
    return commission_per_share;
}

void Wallet::set_spread(double spread) {
    this->spread = spread;
}

void Wallet::set_commission_per_share(double commission_per_share) {
    this->commission_per_share = commission_per_share;
}

const std::vector<std::pair<double, double>> & Wallet::get_wallet_history() const {
    return wallet_history;
}
