
extern const char long_test_one[];
extern const char long_test_two[];

int main() {
	if (long_test_one[0] != '1') return -1;
	if (long_test_two[0] != '2') return -1;

	return 0;
}
