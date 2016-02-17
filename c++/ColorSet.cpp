// Find 2-color difference ramsey number
// Jiayang Li
// Spring 2016

#include "ColorSet.h"
//==============================================================

ColorSet::ColorSet(int_vec_t& init_red, int_vec_t& init_blue, size_t k, size_t l)
	: k(k-1), l(l-1), red(move(init_red)), blue(move(init_blue))	{ }

int ColorSet::search(unsigned int new_diff) {
	unsigned int p = 0, q = 0;

	//========== debugging print statements ==============
	//cout << "Trying to add: " << new_diff << endl;
	//cout << "Coloring " << new_diff << " red in: ";
	//this->pprint();
	//====================================================

	// try coloring new_diff red
	red.push_back(new_diff);
	if (avoids(red, k)) {
		p = search(new_diff+1);
	}
	red.pop_back();

	//========== debugging print statements ==============
	//cout << "Coloring " << new_diff << " blue in: ";
	//this->pprint();
	//====================================================

	// try coloring new_diff blue
	blue.push_back(new_diff);
	if (avoids(blue, l)) {
		q = search(new_diff+1);
	}
	blue.pop_back();

	return MAX(p, q, new_diff);
}

void ColorSet::pprint() {
	cout << "red: ";
	print_color_set(red);
	cout << ", blue: ";
	print_color_set(blue);
	cout << endl;
}


// helper utility functions
inline void print_color_set(const int_vec_t& color_set) {
	cout << "{";
	for (int_vec_t::const_iterator iter = color_set.begin(); iter != color_set.end(); iter++) {
		cout << " " << *iter;
	}
	cout << " }";
}

inline bool avoids(const int_vec_t& color_set, size_t size) {
	if (size > color_set.size()) {
		return true;
	}

	int subset[size];
	return recur_avoids(size, color_set, subset, 0, 0);
}

void print_arr(const int *arr, size_t size) {
	for (unsigned int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

inline bool clique_exists(const int_vec_t& color_set, const int *subset, size_t k) {
	int_vec_t::const_iterator begin = color_set.begin(), end = color_set.end();
	
	for (unsigned int i = 0; i < k; i++) {
		for (unsigned int j = i+1; j < k; j++) {
			if (!binary_search(begin, end, subset[j]-subset[i])) {
				return false;
			}
		}
	}

	return true;
}

inline bool recur_avoids(size_t k, const int_vec_t& color_set, int *subset, 
						unsigned int start, unsigned int i) {
	if (i == k) {
		return (!clique_exists(color_set, subset, k));
	}

	subset[i] = color_set[start];

	//always include the next number
	if (!recur_avoids(k,color_set,subset,start+1,i+1)) {
		return false;
	}

	//only exclude when there're enough elements
	if (color_set.size()-start > k-i) {
		return recur_avoids(k,color_set,subset,start+1,i);
	}

	return true;
}



int main() {
	int_vec_t red, blue;
	red.push_back(1);
	ColorSet cs (red, blue, 3, 9);

	cout << cs.search(2) << endl; 
}
