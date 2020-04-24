#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 7_1";
char inputBuffer[STDIN_BUFFER_SIZE];

void main() {
	printf("Czytam tekst\n");
	gets(inputBuffer);

	printf("Wprowadzono: %s\n", inputBuffer);
}
