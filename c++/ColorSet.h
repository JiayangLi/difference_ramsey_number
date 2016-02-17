#ifndef COLORSET_H
#define COLORSET_H

#include <iostream>
#include <algorithm>
#include <vector>

#define MAX(a, b, c) ((a>b)?((a>c)?a:c):((b>c)?b:c))

using namespace std;
typedef vector<int> int_vec_t;
//==============================================================

class ColorSet {
private:
	size_t k, l;
	int_vec_t red, blue;

public:
	ColorSet(int_vec_t& init_red, int_vec_t& init_blue, size_t k, size_t l);
	int search(unsigned int new_diff);
	void pprint();
};


// helper utility functions 
inline void print_color_set(const int_vec_t& color_set);
inline bool avoids(const int_vec_t& color_set, size_t size);
inline bool clique_exists(const int_vec_t& color_set, const int *subset, size_t k);
inline bool recur_avoids(size_t k, const int_vec_t& color_set, int *subset, 
						unsigned int start, unsigned int i);

#endif