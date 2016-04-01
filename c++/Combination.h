#ifndef COMBINATION_H
#define COMBINATION_H

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

#define MAX_N 300
#define MAX_K 15

namespace bm = boost::multiprecision;

class Combination {
private:
	bm::uint1024_t c[MAX_N+1][MAX_K+1];		// memo table for (n k)

public:
	Combination();
	inline bm::uint1024_t getCombination(uint32_t n, uint32_t k) const {
		return c[n][k];
	}

	void print() const;
};

#endif