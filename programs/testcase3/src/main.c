#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 3";

void main() {
	printf("%s\n", testName);

	exec(prog_id_testcase3_1);
	exec(prog_id_testcase3_1);
	exec(prog_id_testcase3_1);

	printf("Pozdrowienia od procesu rodzica\n");
}
