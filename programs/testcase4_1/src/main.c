#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 4_1";

void main() {
	for(unsigned int i = 0u; i < 5u; i++)
	{
		sleep(400);
		printf("Pozdrowienia od procesu potomnego 1\n");
	}
}
