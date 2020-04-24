#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 4_2";

void main() {
    for(unsigned int i = 0u; i < 5u; i++)
	{
		sleep(200);
		printf("Pozdrowienia od procesu potomnego 2\n");
	}
}
