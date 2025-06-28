#include "instance.h"
#include "solution.h"
#include "computer.h"
#include <limits>
#include <list>
#include <queue>
#include <random>
#include <unordered_set>

using ul = unsigned long;
using ld = long double;

std::mt19937_64 eng{std::random_device{}()};
std::uniform_int_distribution<ul> dist{0, std::numeric_limits<ul>::max()};

struct change{
    ul change_type, from, to, who;
    std::vector<std::pair<ul, ul>> computer_sequence;
};

inline change gen_change(solution& sol){
    change c;
    c.change_type = dist(eng) % sol._computers.size();
    if(c.change_type == 0) { // move market
        c.from = dist(eng) % sol._computers.size(), c.who = dist(eng) % sol._computers[c.from]._markets.size();
        while(true){
            c.to = dist(eng) % sol._computers.size();
            if(c.to != c.from) break;
        }
    }else{ // k-swap
        ul aux, num = 0;
        std::unordered_set<ul> used;
        while(true){
            aux = dist(eng) % sol._computers.size();
            if(used.find(aux) == used.end()){
                used.insert(aux), num++;
                if(num == c.change_type + 1) break;
            }
        }
        while(!used.empty())
            c.computer_sequence.push_back({*used.begin(), 0}), used.erase(used.begin());
        for(auto &i : c.computer_sequence)
            i.second = dist(eng) % sol._computers.at(i.first)._markets.size();
    }
    return c;
};

inline void initial_greedy(instance &inst, solution &sol) {
    std::sort(inst._markets.begin(), inst._markets.end(), [](const market &a, const market &b) {return a._events_avg > b._events_avg;});
    std::list<market> v(inst._markets.begin(), inst._markets.end());
    ul idx = 0;
    computer c(0);
    bool found = false, (*cmp)(const computer &, const computer &) = [](const computer &a, const computer &b){return a._events_sum > b._events_sum;};
    
    std::priority_queue<computer, std::vector<computer>, decltype(cmp)> min_heap(cmp);
    for (ul i = 0; i < inst._k; ++i)
        min_heap.push({inst._n_exchanges});
    while(!v.empty()){
        c = min_heap.top(), found = false, min_heap.pop();
        for (std::list<market>::iterator it = v.begin(); it != v.end(); ++it) {
            if (c._constraint_count[it->_exchange_id] < inst._hard_constraints[it->_exchange_id]) {
                c._markets.push_back(*it), c._events_sum += it->_events_avg, c._constraint_count[it->_exchange_id]++, v.erase(it), found = true;
                break;
            }
        }
        if(!found) printf("No more markets can be added. This solution is invalid.\n"), exit(EXIT_FAILURE);
        min_heap.push(c);
    }
    while(!min_heap.empty()){
        sol._computers.insert({idx++, min_heap.top()});
        min_heap.pop();
    }
    sol.evaluation();
};

inline void reconstruct_greedy(solution &sol){
    sol.calc_mean();
};

