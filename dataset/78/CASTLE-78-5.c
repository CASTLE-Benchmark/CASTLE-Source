/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-5.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/149155/versions/2.0.0
version: 1.1
compile: gcc CASTLE-78-5.c -o CASTLE-78-5
vulnerable: true
description: The test case exposes an OS Command Injection in scope complexity.
cwe: 78
============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE_CMD 14
const char cmd[SIZE_CMD] = "/usr/bin/cat ";

void runCommand(char *str)
{
	if(system(str) < 0) // {!LINE}
		fprintf(stderr, "system() failed");
}

int main(int argc, char *argv[])
{
	char sys[512];
	char buff[512];
	if (fgets(buff,512 - SIZE_CMD,stdin))
	{
		strcpy(sys, cmd);
		strcat(sys, buff);
		runCommand(sys);
	}
	return 0;
}