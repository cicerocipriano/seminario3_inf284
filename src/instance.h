#ifndef INSTANCE_H
#define INSTANCE_H

#include "json.hpp"
#include "market.h"
#include <fstream>

using ul = unsigned long;

struct instance {
  inline instance(const ul k = 0) : _k(k), _n_markets(0), _n_exchanges(0){};

  inline ~instance(){};

  inline void print() const {
    printf("Instance with %lu exchanges and %lu markets.\nExchanges:\n",
           this->_n_exchanges, this->_n_markets);
    for (std::pair<ul, std::string> exchange : this->_exchanges_ids)
      printf("  id-> %lu: %s\n", exchange.first, exchange.second.c_str());
    printf("Markets:\n");
    for (std::pair<ul, std::string> market : this->_markets_ids)
      printf("  id->%lu: %s\n", market.first, market.second.c_str());
    printf("Hard constraints:\n");
    for (std::pair<ul, ul> constraint : this->_hard_constraints)
      printf("  Exchange %s: max %lu markets\n",
             (this->_exchanges_ids.at(constraint.first)).c_str(),
             constraint.second);
    printf("Market values:\n");
    for (const market &m : this->_markets)
      m.print();
  };

  inline void read_input(const std::string &filename) {
    std::ifstream config("../instances/config.json");
    if (!config)
      printf("Error: Config file not found.\n"), exit(EXIT_FAILURE);
    std::ifstream file("../instances/" + filename);
    if (!file)
      printf("Error: Instance file not found.\n"), exit(EXIT_FAILURE);
    this->_data = nlohmann::json::parse(file);
    this->_restrictions = nlohmann::json::parse(config);
    for (nlohmann::json::iterator it = this->_data.begin();
         it != this->_data.end(); ++it) {
      this->_exchanges_ids.insert({this->_n_exchanges, it.key().c_str()});
      for (nlohmann::json::iterator value = it.value().begin();
           value != it.value().end(); ++value) {
        this->_markets_ids.insert({this->_n_markets, value.key().c_str()});
        this->_markets.emplace_back(value.value().get<ul>(), this->_n_exchanges,
                                    this->_n_markets++);
      }
      this->_n_exchanges++;
    }
    for (nlohmann::json::iterator item = this->_restrictions.begin();
         item != this->_restrictions.end(); ++item) {
      for (std::pair<ul, std::string> pair : this->_exchanges_ids) {
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
  std::unordered_map<ul, ul>
      _hard_constraints; // exchange_id -> max number of markets
  std::vector<market> _markets;
};

#endif // INSTANCE_H