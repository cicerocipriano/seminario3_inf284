#ifndef GENETIC_H
#define GENETIC_H

#include "algorithms.h"
#include <vector>
#include <random>
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>
#include <Exception>

using ld = long double;
using ul = unsigned long;


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
	int pop_size;
	int iterations;
	std::vector<ul> hard_c;


    //std::numeric_limits<long double>::max()

	inline Genetic(instance i)
	{

		pop_size = 5000;
		iterations=1000;
		sols = std::priority_queue<solution, std::vector<solution>, std_comp>();
		hard_c = std::vector<ul>(i._n_exchanges);

		for (std::pair<ul, ul> i : i._hard_constraints)
			hard_c[i.first] = i.second;

		eng = std::mt19937(rd());
		dist = std::uniform_int_distribution<>(0, 1);
		dist2 = std::uniform_int_distribution<>(0, 1);
		//i.print();

		int a = 0;
		while (a < pop_size)
		{
			
			a++;
			solution s = gen_random_sol(i);
			
			

			if (!s.check_validity(hard_c))
			{
				a--;
				continue;
			}
			
			

		
			s.evaluation();

			//s.print(false);
			
			//std::cout << "aa" << std::endl;
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
		
		for (int a=0;a< i._markets.size();a++)
		{
			auto m = i._markets[a];

			auto inx = dist(eng);
			//std::cout << inx << std::endl;
			s._computers.at(inx)._markets.push_back(m);
			
		}

		


		for (int a = 0; a < i._k; a++)
		{
			s._computers.at(a).calc_sum();
			//std::cout << s._computers.at(0)._events_sum << std::endl;
		}
		
		





		return s;


	}


	inline void make_children(instance i,solution p1,solution p2, std::priority_queue<solution, std::vector<solution>, std_comp>& hold)
	{
		

		//std::cout << p1._computers.size() << std::endl;
		//std::cout << p2._computers.size()  << std::endl;

		//p1.print(false);
		//p2.print(false);


		try
		{ 

			solution a1b1 = p1;
			solution b1a1 = p2;
			solution b1a2 = p2;
			solution a1b2 = p1;

			solution a2b2 = solution();
			solution b2a2 = solution();

			
			

			ul half = p1._computers.size() / 2;

			if (p1._computers.size() != p2._computers.size()) { std::cout << p1._computers.size() <<" Ack "<< p2._computers.size() << std::endl; }

			for (ul a = 0; a < 5; a++)
			{

				
				
				a1b1._computers.at(half + a) = p2._computers.at(a);
				b1a1._computers.at(half + a) = p1._computers.at(a);
				b1a2._computers.at(half + a) = p1._computers.at(a + half);
				a1b2._computers.at(half + a) = p2._computers.at(a + half);

				//std::cout << "Comeco 2" << std::endl;

				

				

				a2b2._computers.insert({ a,p1._computers.at(a + 0 + half) });
				b2a2._computers.insert({ a,p2._computers.at(a + half) });
				

			}

			

			for (ul a = 0; a < 5; a++)
			{
				a2b2._computers.insert({ a + half,p2._computers.at(a + half) });
				b2a2._computers.insert({ a + half,p1._computers.at(a + half) });
			}



			a1b1.evaluation();
			a1b2.evaluation();
			//b1a1.evaluation();
			b1a2.evaluation();
			
			a2b2.evaluation();
			//b2a2.evaluation();


			if (a1b1.check_validity(hard_c))
			{
				hold.emplace(a1b1);
			}
			
			if (a1b2.check_validity(hard_c))
			{
				hold.emplace(a1b2);
			}
			//hold.emplace(b1a1);
			if (b1a2.check_validity(hard_c))
			{
				hold.emplace(b1a2);
			}
			//hold.emplace(b2a2);

			if (a2b2.check_validity(hard_c))
			{
				hold.emplace(a2b2);
			}
			
			//std::cout << "Aqui" << std::endl;

		}

		catch (int errorCode)
		{
			std::cout << "Exception" << std::endl;
		}
		



	}

	inline void next_generation(instance i)
	{
		std::vector<solution> good_sols = std::vector<solution>();
		std::vector<solution> elite_sols = std::vector<solution>();

		int good_limit = sols.size()/4;
		int elite_limit = (sols.size() * 3) / 100;

		for (int a = 0; a < good_limit; a++)
		{
			auto s = sols.top(); sols.pop();

			if (a < elite_limit)
			{
				elite_sols.push_back(s);
			}
			//std::cout << "tamanho good sol:" << s._computers.size() << std::endl;
			good_sols.push_back(s);

			
		}

		//auto first_place = good_sols[0];
		//auto second_place = good_sols[1];
		
		sols = std::priority_queue<solution, std::vector<solution>, std_comp>();
		//auto rng = std::default_random_engine{};
		std::ranges::shuffle(good_sols.begin(),good_sols.end(), eng);

		for (int a = 0; a < 25; a++)
		{
			auto s = good_sols[a];
			//std::cout << "tamanho shuffle:" << s._computers.size() << std::endl;
		}

		for (int a = 0; a < 25; a++)
		{
			int next = 0;
			if (a + 1 < 25) { next = a + 1; }


			//std::cout << good_sols[a]._computers.size() << std::endl;
			//std::cout << good_sols[next]._computers.size() << std::endl;

			make_children(i,good_sols[a],good_sols[next],sols);


		}

		while (sols.size() < 25 || sols.size() % 2 != 0)
		{
			auto s = gen_random_sol(i);
			s.evaluation();
			sols.emplace(s);

		}

		//sols.emplace(first_place);
		//sols.emplace(second_place);

		for (int a = 0; a < elite_sols.size(); a++)
		{
			sols.emplace(elite_sols[a]);
		}

		
		


	}

	inline void solve(instance i)
	{

		std::priority_queue<solution, std::vector<solution>, std_comp> best_it_sols = std::priority_queue<solution, std::vector<solution>, std_comp>();
		for (int a = 0; a < iterations; a++)
		{
			next_generation(i);
			//std::cout << "gen size: " << sols.size() << std::endl;
			best_it_sols.emplace(sols.top());
		}

		best_it_sols.top().print(false);
	}


	
};

#endif