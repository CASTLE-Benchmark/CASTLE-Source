/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-5.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/1889/versions/1.0.0
version: 1.1
compile: gcc CASTLE-22-5.c -o CASTLE-22-5
vulnerable: true
description: The test case exposes a path manipulation.
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printFile(const char *fileName)
{
	FILE *fp = (FILE *)NULL;
	if ((fp = fopen(fileName, "r"))) // {!LINE}
	{
		char buffer[512];
		unsigned int lNumber = 0;
		while (fgets(buffer, 512, fp))
		{
			printf("%3u: %s", ++lNumber, buffer);
		}
		fclose(fp);
		return 0;
	}
	return 1;
}


int main(int argc, char *argv[])
{
	// Open the file in the command line
	if (argc > 1)
	{
		if (printFile(argv[1])) // {!LINE}
			printf("Argument error, the given argument is not a readable file.\n");
	}
	return 0;
}