#ifndef INSTANCE_H
#define INSTANCE_H

#include "json.hpp"
#include "market.h"

using ul = unsigned long;

struct instance {
    instance();
    ~instance();

    inline void print() const;
    inline void read_input(const std::string &);

    nlohmann::json _data, _restrictions;
    ul _k;
    std::unordered_map<std::string, ul> _exchanges_ids, _markets_ids;
    std::unordered_map<ul, ul> _hard_constraints, _belongings;
    std::vector<market> _markets;
};

#endif // INSTANCE_H