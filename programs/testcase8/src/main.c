#include <simplstd.h>
#include <stdio.h>

const char *testName = "TEST 8";

void main() {
	printf("%s\n", testName);

	printf("Bardzo dlugi tekst ktory z pewnoscia nie zmiesci sie w jednym rzedzie bufora VGA poniewaz posiada wiecej niz osiemdziesiat znakow\n");

	for(int i = 0; i < 30; i++)
	{
		printf("linia numer %d\n", i);
	}
}
