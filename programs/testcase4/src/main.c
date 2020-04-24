#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 4";

void main() {
	printf("%s\n", testName);

	exec(prog_id_testcase4_1);
	exec(prog_id_testcase4_2);

	for(unsigned int i = 0u; i < 5u; i++)
	{
		sleep(600);
		printf("Pozdrowienia od procesu rodzica\n");
	}
}
