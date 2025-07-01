#ifndef GENETIC_H
#define GENETIC_H

#include "algorithms.h"
#include <vector>
#include <random>
#include <iostream>
#include <queue>
#include <limits>


using ld = long double;


struct std_comp {
	bool operator()(const solution& a, const solution& b) const {
		return a._std_deviation > b._std_deviation;
	}
};

struct Genetic
{

	std::priority_queue<solution, std::vector<solution>, std_comp> sols;
	std::random_device rd;
	std::mt19937 eng;
	std::uniform_int_distribution<> dist;
	std::uniform_int_distribution<> dist2;


    //std::numeric_limits<long double>::max()

	inline Genetic(instance i)
	{

		
		sols = std::priority_queue<solution, std::vector<solution>, std_comp>();
		
		eng = std::mt19937(rd());
		dist = std::uniform_int_distribution<>(0, 1);
		dist2 = std::uniform_int_distribution<>(0, 1);


		int a = 0;
		while (a < 20)
		{
			
			a++;
			solution s = gen_random_sol(i);
			
			

			if (false)
			{
				a--;
				continue;
			}
			
			

			s.calc_mean();
			s.calc_variance();
			s.calc_std_deviation();
			

			sols.emplace(s);

			


		}
	}


	inline solution gen_random_sol(instance i)
	{
		std::vector<market> mks = i._markets;
		solution s = solution();

		


		for (int a = 0; a < i._k; a++)
		{
			computer c = computer();
			

			s._computers.insert({a,c});
		}
		
		


		dist = std::uniform_int_distribution<>(0, s._computers.size() - 1);
		
		for (auto m : mks)
		{
			computer c = s._computers.at(dist(eng));
			c._markets.push_back(m);
		}

		for (int a = 0; a < i._k; a++)
		{
			computer c = computer();
			c.calc_sum();
		}





		return s;


	}
};

#endif