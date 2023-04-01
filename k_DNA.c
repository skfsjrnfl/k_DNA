#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 128

int main() {
	
	FILE* input = fopen("hw2_input.txt", "r");
	if (input != NULL) {
		int n;
		char buffer[BUFLEN];
		char** dna;
		char** dnaout;
		int lcsmatrix[101][101];
		char lcs[100];
		while (1) {
			fgets(buffer, BUFLEN - 1, input);
			if (strchr(buffer, '$') != NULL)
				break;
			n = atoi(buffer);
		}
		if (n < 2) {
			printf("Wrong number of input");
			return -1;
		}

		dna = (char**)malloc(sizeof(char*) * n);
		dnaout = (char**)malloc(sizeof(char*) * (n+1));

		for (int i = 0; i < n; i++) {
			dna[i] = (char*)malloc(sizeof(char) * 101);
			dnaout[i] = (char*)malloc(sizeof(char) * 200);

			fgets(dna[i], 101, input);
			dna[i] = strtok(dna[i], "\r\n");
		}
		dnaout[n] = (char*)malloc(sizeof(char) * 200);
		fclose(input);//input

		//Find LCS;
		for (int i = 0; i < (strlen(dna[0])+1); i++) {
			for (int j = 0; j < (strlen(dna[1])+1); j++) {
				if (i == 0 || j == 0)
					lcsmatrix[i][j] = 0;
				else if (dna[0][i -1] == dna[1][j - 1])
					lcsmatrix[i][j] = lcsmatrix[i - 1][j - 1] + 1;
				else {
					if (lcsmatrix[i - 1][j] > lcsmatrix[i][j - 1]) {
						lcsmatrix[i][j] = lcsmatrix[i - 1][j];
					}
					else
						lcsmatrix[i][j] = lcsmatrix[i][j - 1];
				}
			}
		}
		int count = lcsmatrix[strlen(dna[0])][strlen(dna[1])];
		lcs[count] = '\0';
		int i = strlen(dna[0]);
		int j = strlen(dna[1]);
		while (lcsmatrix[i][j]!=0) {
			if (lcsmatrix[i][j] > lcsmatrix[i-1][j - 1] ) {
				count--;
				lcs[count] = dna[1][j-1];
				i--;
			}
			j--;
		}

		for (int k=2;k<n;k++) {
			for (int i = 0; i < (strlen(lcs) + 1); i++) {
				for (int j = 0; j < (strlen(dna[k]) + 1); j++) {
					if (i == 0 || j == 0)
						lcsmatrix[i][j] = 0;
					else if (lcs[i - 1] == dna[k][j - 1])
						lcsmatrix[i][j] = lcsmatrix[i - 1][j - 1] + 1;
					else {
						if (lcsmatrix[i - 1][j] > lcsmatrix[i][j - 1]) {
							lcsmatrix[i][j] = lcsmatrix[i - 1][j];
						}
						else
							lcsmatrix[i][j] = lcsmatrix[i][j - 1];
					}
				}
			}
			int count = lcsmatrix[strlen(lcs)][strlen(dna[k])];
			int i = strlen(lcs);
			int j = strlen(dna[k]);

			lcs[count] = '\0';
			while (lcsmatrix[i][j] != 0) {
				if (lcsmatrix[i][j] > lcsmatrix[i-1][j - 1]) {
					count--;
					lcs[count] = dna[k][j - 1];
					i--;
				}
				j--;
			}
		}

		int* cursor = malloc(sizeof(int) * (n+1));
		int same;
		int end;
		int outcursor = 0;
		for (int i = 0; i < n+1; i++) {
			cursor[i] = 0;
		}

		while (1) {
			if (cursor[n] >= strlen(lcs)) {
				end = 0;
				for (int i = 0; i < n; i++) {
					if (cursor[i] < strlen(dna[i])) {
						dnaout[i][outcursor] = dna[i][cursor[i]];
						cursor[i]++;
					}
					else {
						dnaout[i][outcursor] = '-';
						end++;
					}
				}
				dnaout[n][outcursor] = ' ';
				if (end == n) {
					for (int i = 0; i < (n + 1); i++) {
						dnaout[i][outcursor] = '\0';
					}
					break;
				}
				outcursor++;	
			}
			else {
				same = 0;
				for (int i = 0; i < n; i++) {
					if (dna[i][cursor[i]] == lcs[cursor[n]]) {
						same++;
					}
				}

				if (same == n) {
					for (int i = 0; i < n; i++) {
						dnaout[i][outcursor] = lcs[cursor[n]];
						cursor[i]++;
					}
					cursor[n]++;
					dnaout[n][outcursor] = '*';
					outcursor++;
				}
				else {
					for (int i = 0; i < n; i++) {
						if (dna[i][cursor[i]] == lcs[cursor[n]]) {
							dnaout[i][outcursor] = '-';
						}
						else {
							dnaout[i][outcursor] = dna[i][cursor[i]];
							cursor[i]++;
						}
					}
					dnaout[n][outcursor] = ' ';
					outcursor++;
				}
			}
		}
		
		FILE* output = fopen("hw2_output.txt", "w");

		for (int i = 0; i < n + 1; i++) {
			fprintf(output,"%s\n", dnaout[i]);
		}
		
		fclose(output);
		//Free memory
		free(cursor);
		for (int i = 0; i < n; i++) {
			free(dna[i]);
			free(dnaout[i]);
		}
		free(dnaout[n]);
		free(dna);
		free(dnaout);
	}
	else {
		printf("Fail to open file");
		return -1;
	}	
}
