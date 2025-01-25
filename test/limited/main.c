
#include <limited_assets.h>

int main() {
	if (sizeof(test_a_10) != 10) return -1;
	if (sizeof(test_b_16) != 16) return -1;
	if (sizeof(test_a_2) != 2) return -1;
	if (sizeof(test_b_8) != 8) return -1;

	for (int i = 0; i < 10; i ++) {
		if (test_a_10[i] != 'a') return -1;
	}

	for (int i = 0; i < 16; i ++) {
		if (test_b_16[i] != 'b') return -1;
	}

	for (int i = 0; i < 2; i ++) {
		if (test_a_2[i] != 'a') return -1;
	}

	for (int i = 0; i < 8; i ++) {
		if (test_b_8[i] != 'b') return -1;
	}

	return 0;
}
