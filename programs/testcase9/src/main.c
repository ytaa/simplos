#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 9";

void main() {
	printf("%s\n", testName);

	printf("Przed terminacja\n");
	exit();
	printf("Po terminacji\n");
}
