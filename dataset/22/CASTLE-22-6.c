/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-6.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/1887/versions/1.0.0
version: 1.1
compile: gcc CASTLE-22-6.c -o CASTLE-22-6
vulnerable: true
description: The test case shows a path manipulation weakness.
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **fileNames = NULL;

int printFile(const char *fileName)
{
	FILE *fp = (FILE *)NULL;
	if ((fp = fopen(fileName, "r"))) // {!LINE}
	{
		char buffer[512];
		unsigned int lNumber = 0;
		printf (">>> %s\n",fileName);
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
	short badAlloc = 0;
	// Open the file in the command line
	if (argc > 1)
	{
		const unsigned int nbArgs = argc - 1;
		if ((fileNames = (char **)malloc((nbArgs) * sizeof(char **))) == NULL)
			return 0;
		for (unsigned int i=0;i<nbArgs;++i)
		{
			// allocate the direction
			if (!(fileNames[i] = (char *)malloc(256 * sizeof(char))))
				badAlloc = 1;
			else {
				strncpy(fileNames[i], argv[i+1], 255);
				fileNames[255] = '\0'; // ensuring null termination
			}
		}

		if (!badAlloc)
		{
			for (unsigned int i=0;i<nbArgs;++i)
				if (printFile(fileNames[i])) // {!LINE}
					printf("Argument error, the given argument is not a readable file (%s).\n", fileNames[i]);
		}

		for (unsigned int i=0;i<nbArgs;++i)
			free(fileNames[i]);
		free(fileNames);
	}
	return 0;
}