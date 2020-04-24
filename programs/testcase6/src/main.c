#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 6";
char inputBuffer[STDIN_BUFFER_SIZE];

void main() {
	printf("%s\n", testName);

	printf("Podaj imie: ");
	gets(inputBuffer);

	printf("Czesc, %s\n", inputBuffer);
}
