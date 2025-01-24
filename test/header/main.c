
#include <header_assets.h>

int main() {
	if (sizeof(asset) != 1) return -1;
	if (asset[0] != '5') return -1;

	return 0;
}
