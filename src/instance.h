#ifndef INSTANCE_H
#define INSTANCE_H

#include "json.hpp"
#include "market.h"
#include <fstream>

using ul = unsigned long;

struct instance {
    inline instance(const ul k = 0): _k(k), _n_markets(0), _n_exchanges(0){};

    inline ~instance(){};

    inline void print() const{};

    inline void read_input(const std::string &filename){
        std::ifstream config("../instances/config.json"), file("../instances/" + filename);
        if (!config) printf("Error: Config file not found.\n"), exit(EXIT_FAILURE);
        if (!file) printf("Error: File %s not found.\n", filename.c_str()), exit(EXIT_FAILURE);
        this->_data = nlohmann::json::parse(file), this->_restrictions = nlohmann::json::parse(config);
        for (auto it = this->_data.begin(); it != this->_data.end(); ++it) {
            this->_exchanges_ids.insert({this->_n_exchanges, it.key() .c_str()});
            for (auto& value : it.value().items()){
                this->_markets_ids.insert({this->_n_markets, value.key().c_str()});
                this->_markets.emplace_back(value.value().get<ul>(), this->_n_exchanges++, this->_n_markets++);
            }
        }
        for (auto& item : this->_restrictions.items()) {
            for (const auto& pair : this->_exchanges_ids) {
                if (pair.second == item.key()) {
                    this->_hard_constraints.insert({pair.first, item.value().get<ul>()});
                    break;
                }
            }
        }

    };

    nlohmann::json _data, _restrictions;
    ul _k, _n_markets, _n_exchanges;
    std::unordered_map<ul, std::string> _exchanges_ids, _markets_ids;
    std::unordered_map<ul, ul> _hard_constraints; // exchange_id -> max number of markets
    std::vector<market> _markets;
};

#endif // INSTANCE_H