//==============================================================
// Find 2-color difference ramsey number
// Jiayang Li
// Spring 2016
//==============================================================

#include "ColorSet.h"

Combination cmb;

ColorSet::ColorSet(int_vec_t& init_red, int_vec_t& init_blue, uint32_t k, uint32_t l)
	: k(k-1), l(l-1), red(move(init_red)), blue(move(init_blue)) {
		init_set(red, red_set);
		init_set(blue, blue_set);
	}

uint32_t ColorSet::search(uint32_t new_diff) {
	uint32_t p = 0, q = 0;

	//========== debugging print statements ==============
	//std::cout << "Coloring " << new_diff << " red in: ";
	//this->pprint();
	//====================================================

	// try coloring new_diff red
	red.push_back(new_diff);
	red_set.insert(new_diff);
	if (avoids(red, red_set, k)) {
		p = search(new_diff+1);
	}
	red_set.erase(new_diff);
	red.pop_back();

	//========== debugging print statements ==============
	//std::cout << "Coloring " << new_diff << " blue in: ";
	//this->pprint();
	//====================================================

	// try coloring new_diff blue
	blue.push_back(new_diff);
	blue_set.insert(new_diff);
	if (avoids(blue, blue_set, l)) {
		q = search(new_diff+1);
	}
	blue_set.erase(new_diff);
	blue.pop_back();

	return MAX(p, q, new_diff);
}

void ColorSet::pprint() {
	std::cout << "red: ";
	print_color_set(red);
	std::cout << ", blue: ";
	print_color_set(blue);
	std::cout << std::endl;
}


// helper utility functions
inline void init_set(const int_vec_t& v, int_set_t& s) {
	for (int_vec_t::const_iterator iter = v.begin(); iter != v.end(); iter++) {
		uint32_t n = *iter;
		s.insert(n);
	}
}

inline void print_color_set(const int_vec_t& color_set) {
	std::cout << "{";
	for (int_vec_t::const_iterator iter = color_set.begin(); iter != color_set.end(); iter++) {
		std::cout << " " << *iter;
	}
	std::cout << " }";
}

inline bool avoids(const int_vec_t& color_set, const int_set_t& set, uint32_t size) {
	uint32_t n = color_set.size(); 

	// easy case: not enough colors, definitely true
	if (size > n) {
		return true;
	}

	//uint32_t num_combs = nchoosek(n, size);
	bm::uint1024_t num_combs = cmb.getCombination(n, size);

	// n too small, use sequential version
	if (num_combs < THRESHOLD) {
		uint32_t subset[size];
		return avoids_seq(size, color_set, set, subset, 0, 0);
	} else {	// for big n, use parallelized version
		return avoids_para(color_set, set, size, n, num_combs);
	}
}

inline bool avoids_para(const int_vec_t& color_set, const int_set_t& set, uint32_t size, uint32_t n, bm::uint1024_t num_combs) {
	bool no_clique = true;

	#pragma omp parallel default(shared)
	{
		uint32_t subset[size+2];	// the first and last element are dummy
		subset[0] = n;				// for eaiser comb generation
		subset[size+1] = n;			

		uint32_t id, num_threads;
		bm::uint1024_t chunk, start, end;

		// evenly spread subsets across different threads
		id = omp_get_thread_num();
		num_threads = omp_get_num_threads();
		chunk = num_combs / num_threads;
		start = id * chunk;
		if (id == num_threads-1)
			end = num_combs;
		else
			end = (id+1) * chunk;

		// search for clique
		unrank(subset, start, n, size);
		while (no_clique) {
			if (clique_exists_para(color_set, set, subset, size)) {
				no_clique = false;
			}
			start++;
			if (start == end)
				break;
			else
				successor(subset, n, size);
		}
	}

	return no_clique;
}

inline bool clique_exists_para(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k) {
	// In parallel version, subset consists of indexes
	int_vec_t::const_iterator begin = color_set.begin(), end = color_set.end();
	
	for (uint32_t i = 1; i <= k; i++) {
		for (uint32_t j = i+1; j <= k; j++) {
			if (set.find(color_set[subset[j]]-color_set[subset[i]]) == set.end()) {
				return false;
			}
		}
	}

	return true;
}

inline bool clique_exists_seq(const int_vec_t& color_set, const int_set_t& set, const uint32_t *subset, uint32_t k) {
	// In sequential version, subset consists of actual values
	int_vec_t::const_iterator begin = color_set.begin(), end = color_set.end();
	
	for (uint32_t i = 0; i < k; i++) {
		for (uint32_t j = i+1; j < k; j++) {
			if (set.find(subset[j]-subset[i]) == set.end()) {
				return false;
			}
		}
	}

	return true;
}

inline uint32_t nchoosek(uint32_t n, uint32_t k) {
	//take the smaller side, minimize num of operations
	uint32_t l = n-k;
	if (l < k)
		k = l;

	uint32_t numerator = 1, denominator = 1;
	for (uint32_t i = 1; i <= k; i++) {
		numerator *= (n-k+i);
		denominator *= i;
	}
	return numerator/denominator;
}

inline void unrank(uint32_t *t, bm::uint1024_t r, uint32_t n, uint32_t k) {
	// Using minimal change order
	uint32_t x = n;
	for (uint32_t i = k; i > 0; i--) {
		while (cmb.getCombination(x, i) > r) {
			x--;
		}
		t[i] = x;
		r = cmb.getCombination(x+1, i)-r-1;
	}
}

inline void successor(uint32_t *t, uint32_t n, uint32_t k) {
	// Using minimal change order
	// Generate in-place the next combination following sub_idx
	uint32_t j = 1;
	while (j <= k && t[j] == j-1) {
		j++;
	}

	if ((k-j) % 2 != 0) {
		if (j == 1) {
			t[j] = t[j] - 1;
		} else {
			t[j-1] = j-1;
			t[j-2] = j-2;
		}
	} else {
		if (t[j+1] != t[j] + 1) {
			t[j-1] = t[j];
			t[j] = t[j] + 1;
		} else {
			t[j+1] = t[j];
			t[j] = j-1;
		}
	}
}

inline bool avoids_seq(uint32_t k, const int_vec_t& color_set, const int_set_t& set, uint32_t *subset, 
						uint32_t start, uint32_t i) {
	if (i == k) {
		return (!clique_exists_seq(color_set, set, subset, k));
	}

	subset[i] = color_set[start];

	//always include the next number
	if (!avoids_seq(k, color_set, set, subset, start+1, i+1)) {
		return false;
	}

	//only exclude when there're enough elements
	if (color_set.size()-start > k-i) {
		return avoids_seq(k, color_set, set, subset, start+1,i);
	}

	return true;
}


//==============================================================
//====================== main function =========================
int main(int argc, char *argv[]) {
	uint32_t k, l;

	if (argc != 3) {
		std::cerr << "Usage: ramsey k1 k2" << std::endl;
		exit(EXIT_FAILURE);
	}

	try {
		k = std::stoi(argv[1]);
		l = std::stoi(argv[2]);
	} catch (std::invalid_argument& e) {
		std::cerr << "Usage: ramsey k1 k2" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set the number of threads for checking if 
	// a monochromatic clique is avoided
	// omp_set_num_threads(12);

	std::cout << k << "  " << l << std::endl;

	int_vec_t red, blue;
	red.push_back(1);
	ColorSet cs (red, blue, k, l);
	std::cout << "Result for starting from {1}, {}: " << cs.search(2) << std::endl; 
	
	return 0;
}
