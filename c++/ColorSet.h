#ifndef COLORSET_H
#define COLORSET_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <omp.h>
#include "Combination.h"
	
#define MAX(a, b) ((a>b)?a:b)
#define THRESHOLD 50

typedef std::vector<uint32_t> int_vec_t;
typedef std::unordered_set<uint32_t> int_set_t;
typedef std::vector<int_vec_t> vec_vec_t;
typedef std::vector<int_set_t> set_vec_t;
//==============================================================

class ColorSet {
public:
	int_vec_t clique_sizes;
	vec_vec_t colors_vec;
	set_vec_t colors_set;

public:
	ColorSet(int_vec_t& sizes, vec_vec_t& colors);
	uint32_t search(uint32_t new_diff);
	void pprint();
};


// helper utility functions 
inline void init_set(const vec_vec_t& v, set_vec_t& s);
inline void print_color_set(const int_vec_t& color_set);
inline bool avoids(const int_vec_t& color_set, const int_set_t& set, uint32_t size);
inline bool clique_exists_para(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k);
inline bool clique_exists_seq(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k);
inline uint32_t nchoosek(uint32_t n, uint32_t k);
inline void unrank(uint32_t *t, bm::uint1024_t r, uint32_t n, uint32_t k);
inline void successor(uint32_t *t, uint32_t n, uint32_t k);
inline bool avoids_seq(uint32_t k, const int_vec_t& color_set, const int_set_t& set, uint32_t *subset, 
						uint32_t start, uint32_t i);
inline bool avoids_para(const int_vec_t& color_set, const int_set_t& set, uint32_t size, uint32_t n, bm::uint1024_t num_combs);

#endif