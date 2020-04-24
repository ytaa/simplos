#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 2";

void main() {
    printf("%s\n", testName);

	printf("Spij 1s start\n");
	sleep(1000);
	printf("Spij 1s stop\n");	

	printf("Spij 100ms start\n");
	sleep(100);
	printf("Spij 100ms stop\n");

	printf("Spij 10ms start\n");
	sleep(10);
	printf("Spij 10ms stop\n");
}
