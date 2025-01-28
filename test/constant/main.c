
#include <constant_assets.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#endif

int check_writeable(void* p, int expect) {
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

// This test always passes on windows
int main() {

	if (check_writeable((void*) mdat, 0)) {
		return -2;
	}

	return 0;
}
