#ifndef COLORSET_H
#define COLORSET_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <omp.h>

#define MAX(a, b, c) ((a>b)?((a>c)?a:c):((b>c)?b:c))
#define THRESHOLD 50

using namespace std;
typedef vector<uint32_t> int_vec_t;
typedef unordered_set<uint32_t> int_set_t;
//==============================================================

class ColorSet {
private:
	uint32_t k, l;
	int_vec_t red, blue;
	int_set_t red_set, blue_set;

public:
	ColorSet(int_vec_t& init_red, int_vec_t& init_blue, uint32_t k, uint32_t l);
	uint32_t search(uint32_t new_diff);
	void pprint();
};


// helper utility functions 
inline void init_set(const int_vec_t& v, int_set_t& s);
inline void print_color_set(const int_vec_t& color_set);
inline bool avoids(const int_vec_t& color_set, const int_set_t& set, uint32_t size);
inline bool clique_exists_para(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k);
inline bool clique_exists_seq(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k);
inline uint32_t nchoosek(uint32_t n, uint32_t k);
inline void unrank(uint32_t *t, uint32_t r, uint32_t n, uint32_t k);
inline void successor(uint32_t *t, uint32_t n, uint32_t k);
inline bool avoids_seq(uint32_t k, const int_vec_t& color_set, const int_set_t& set, uint32_t *subset, 
						uint32_t start, uint32_t i);
inline bool avoids_para(const int_vec_t& color_set, const int_set_t& set, uint32_t size, uint32_t n, uint32_t num_combs);

#endif