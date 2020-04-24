#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 7";
char inputBuffer[STDIN_BUFFER_SIZE];

void main() {
	printf("%s\n", testName);

	exec(prog_id_testcase7_1);
	exec(prog_id_testcase7_1);

	printf("Wprowadz tekst\n");
	gets(inputBuffer);

	printf("Wprowadzono: %s\n", inputBuffer);
}
