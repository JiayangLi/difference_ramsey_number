#include "Combination.h" 

Combination::Combination() {
	// initialize the first row
	c[0][0] = 1;
	for (uint32_t j = 1; j <= MAX_K; j++) {
		c[0][j] = 0;
	}

	// constructs the memo table
	for (uint32_t i = 1; i <= MAX_N; i++) {
		c[i][0] = 1;
		for (uint32_t j = 1; j <= MAX_K; j++) {
			c[i][j] = c[i-1][j-1] + c[i-1][j];
		}
	}
}

void Combination::print() const {
	// debugging print
	for (uint32_t i = 0; i <= MAX_N; i++) {
		for (uint32_t j = 0; j <= MAX_K; j++) {
			std::cout << c[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}