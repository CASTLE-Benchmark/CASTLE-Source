/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-6.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/149153/versions/2.0.0
version: 1.1
compile: gcc CASTLE-78-6.c -o CASTLE-78-6
vulnerable: true
description: Missing input validation of fgets input allows for OS command injection
cwe: 78
============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE_CMD 10
const char cmd[SIZE_CMD] = "/bin/cat ";

int main(int argc, char *argv[])
{
	unsigned i;
	char buff[512];
	char sys[512];
	if (fgets(buff,512 - SIZE_CMD,stdin))
	{
		strcpy(sys, cmd);
		strcat(sys, buff);

        // Validate input
        bool valid = true;
        for (i = 0; i < strlen(sys); i++) {
            if (sys[i] == ';' || sys[i] == '|' || sys[i] == '&') {
                valid = false;
                break;
            }
        }

		for (i=0;i<5;++i) {
			if(system(sys) < 0)	// {!LINE}
				fprintf(stderr, "system() failed");
		}
	}
	return 0;
}