
extern const char hello[];

int main() {
	if (hello[0] != 'h') return -1;
	if (hello[1] != 'e') return -1;
	if (hello[2] != 'l') return -1;
	if (hello[3] != 'l') return -1;
	if (hello[4] != 'o') return -1;

	return 0;
}
