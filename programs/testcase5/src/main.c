#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 5";

void main() {
	printf("%s\n", testName);

	exec(prog_id_testcase5_1);
	exec(prog_id_testcase5_2);
	exec(prog_id_testcase5_3);
	
	wait();

	printf("Wszystkie procesy potomne zakonczone\n");
}
