/*=============================================================================
|	Assignment: pa01 - Encrypting a plaintext file using the Vigenere cipher
|
|
|	Language: c
|
|	To Compile: gcc -o pa01 Assignment_1_Vigenere_Cipher.c
|
|	To Execute:	c	-> ./pa01 kX.txt pX.txt
|
|
|	Note: All input files are simple 8 bit ASCII input
|
|	Class: CIS3360 - Security in Computing - Fall 2022
|
|
+=============================================================================*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 512

char *ConvertFileToString(char *filename)
{
	char temp;
	char *ret = calloc(MAX_LEN + 1, sizeof(char));
	if (ret == NULL)
		return NULL;
	int i = 0;
	FILE *ifp;
	if ((ifp = fopen(filename, "r")) == NULL)
		return NULL;
	while (fscanf(ifp, "%c", &temp) != EOF && i != MAX_LEN)
	{
		if (!isalpha(temp))
		{
			continue;
		}

		ret[i] = tolower(temp);
		i++;
	}
	ret[i] = '\n';
	fclose(ifp);
	return ret;
}

void AddPadding(char *text)
{
	if (text == NULL)
		return;
	int i = strlen(text) - 1;
	for (i; i < MAX_LEN; i++)
		text[i] = 'x';
	text[i] = '\n';
}

char *encryptText(char *text, char *key)
{
	int i = 0, j = 0;
	if (text == NULL)
		return NULL;
	else if (key == NULL)
		return text;
	int keylen = strlen(key) - 1;
	char *ret = malloc(sizeof(int) * (MAX_LEN + 1));
	while(text[i] != '\n' && i != MAX_LEN)
	{
		if (j == keylen)
			j = 0;
		int num_text = text[i] % 'a';
		int num_key = key[j] % 'a';
		ret[i] = (((num_text + num_key) % 26) + 'a');
		i++;
		j++;
	}
	ret[i] = '\n';
	return ret;
}

void printString(char *str)
{
	int i = 0;
	while(str[i] != '\n')
	{
		if (i % 80 == 0 && i != 0)
			printf("\n");
		printf("%c", str[i]);
		i++;
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Missing arguments\n");
		return INT_MIN;
	}
	char *key = ConvertFileToString(argv[1]);
	if (key == NULL)
	{
		printf("File not found\n");
		return INT_MIN;
	}
	char *text = ConvertFileToString(argv[2]);
	if (text == NULL)
	{
		printf("File not found\n");
		return INT_MIN;
	}
	AddPadding(text);

	printf("\n\nVigenere Key:\n\n");
	printString(key);

	printf("\n\nPlaintext:\n\n");
	printString(text);

	char *encrypted = encryptText(text, key);

	printf("\n\nCiphertext:\n\n");
	printString(encrypted);

	free(encrypted);
	free(text);
	free(key);

	return 0;
}
