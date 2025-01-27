
#include <mutable_assets.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#endif

int check_writeable(void *p, int expect) {
#ifdef __linux__
	int fd = open("/dev/zero", O_RDONLY);
	int writeable;

	if (fd < 0) {
		return 1;
	}

	writeable = read(fd, p, 1) == 1;
	close(fd);

	return writeable != expect;
#else
	return 0;
#endif
}

int main() {

	if (check_writeable(mdat, 1)) {
		return -2;
	}

	// undo check_writeable damage
	mdat[0] = 'E';

	for (int i = 0; 1; i ++) {
		char* c = mdat + i;

		if (*c == 'D') {
			*c = 'R';
		}

		if (*c == '0') {
			*c = 0;
			break;
		}
	}

	char buffer[64];
	snprintf(buffer, 64, "Hello %s!", mdat);

	if (strcmp(buffer, "Hello EMBER!")) {
		return -1;
	}

	return 0;
}
