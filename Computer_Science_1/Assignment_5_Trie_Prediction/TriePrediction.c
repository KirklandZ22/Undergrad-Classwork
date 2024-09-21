// COP3502 Spring 2022

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TrieNode Struct given by instructor.
typedef struct TrieNode
{
	// number of times this string occurs in the corpus.
	int count;

	// 26 TrieNode pointers, one for each letter of the alphabet.
	struct TrieNode *children[26];

	// the co-occurrence subtrie for this string.
	struct TrieNode *subtrie;
} TrieNode;

// Defined values given by instructor.
#define MAX_WORDS_PER_LINE 30
#define MAX_CHARACTERS_PER_WORD 1023

// Num of children each node has.
#define NUM_CHILDREN 26


// Helper function called by printTrie().
// This function was given by instructor.
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;
	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. 
// This function was given by instructor.
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// Dynamically creates a TrieNode and initalizes all values to NULL or 0
TrieNode *createNode(void)
{
	return calloc(1, sizeof(TrieNode));
}

// Converts the char to a number 0 - 25 to indicate which child the letter corresponds to
int getIndex(char letter)
{
	if (letter >= 'a'  && letter <= 'z')
		return letter - 'a';
	else if (letter >= 'A' && letter <= 'Z')
		return letter - 'A';
	else
		return -1;
}

// Inserts a word str into the TrieNode starting from the passed in root
// If the nodes do not exist then it creates the nodes
// Increases count on the last node by 1 to indicate how many times the word has appeeared
void insertWord(TrieNode *root, char *str)
{
	//printf("%s", str);
	int i, index, len = strlen(str);
	TrieNode *temp = root;
	if (root == NULL)
		root = createNode();
	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;
		index = getIndex(str[i]);
		if (temp->children[index] == NULL)
			temp->children[index] = createNode();
		temp = temp->children[index];
	}
	temp->count++;
}

// Goes to the last node of the word and creates a subtrie of the word that directly follows it
// If nextChar is a new-line character then no word follows it as each line is a new sentence.
// Parent is the tring that host the subtrie of child
void buildSubTrie(FILE *ifp, TrieNode *root)
{
	char parent[MAX_CHARACTERS_PER_WORD + 1], child[MAX_CHARACTERS_PER_WORD + 1], nextChar;
	TrieNode *temp;
	fscanf(ifp, "%s", parent);
	while ((fscanf(ifp, "%s%c", child, &nextChar)) != EOF)
	{
		temp = getNode(root, parent);
		// If subtrie is not initalized then creates it
		if (temp->subtrie == NULL)
			temp->subtrie = createNode();
		insertWord(temp->subtrie, child);
		// Checks if at the end of the line(sentence)
		if (nextChar == '\n')
		{
			// Checks until a line has more than one word or reach the end of file
			while (nextChar == '\n')
				if ((fscanf(ifp, "%s%c", parent, &nextChar)) == EOF)
					return;
		}
		else
		{
			strcpy(parent, child);
		}
	}
}

// Creates the main trie looping through the text file. Then calls buildSubTrie to create
// the subtrie on the initalized trie
TrieNode *buildTrie(char *filename)
{
	TrieNode *root = NULL;
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	FILE *ifp = fopen(filename, "r");
	if (ifp == NULL)
		return root;
	root = createNode();
	// Creates the main trie
	while ((fscanf(ifp, "%s", buffer)) != EOF)
	{
		insertWord(root, buffer);
	}
	// Creates the subtrie
	rewind(ifp);
	buildSubTrie(ifp, root);

	fclose(ifp);
	return root;

}
// Called from processInputFile this will print the subtrie of a specificied string
void printSubTrie(TrieNode * root, char *str)
{
	printf("%s\n", str);
	TrieNode *temp;
	if ((temp = getNode(root, str)) != NULL)
	{
		if (temp->subtrie == NULL)
		{
			printf("(EMPTY)\n");
		}
		else
		{
			printTrie(temp->subtrie, 1);
		}
	}
	else
	{
		printf("(INVALID STRING)\n");
	}
}

// Called from processInputFile this will print out what words comes after the previous word
// most frequently starting with str num times.
void printPrediction(TrieNode *root, char *str, int num)
{
	int i;
	char parent[MAX_CHARACTERS_PER_WORD + 1], child[MAX_CHARACTERS_PER_WORD + 1];
	TrieNode *temp;
	strcpy(parent, str);
	for (i = 0; i < num; i++)
	{
		printf("%s", parent);
		temp = getNode(root, parent);
		// If the string does not exist in the trie then terminate
		if (temp == NULL)
		{
			printf("\n");
			return;
		}
		getMostFrequentWord(temp->subtrie, child);
		// If the string is empty then terminate
		if (strcmp(child , "") == 0)
		{
			printf("\n");
			return;
		}
		strcpy(parent, child);
		printf(" ");
	}
	// ending condition
	printf("%s\n", parent);
}

// This causes the changes indicated by the input file to the trie
int processInputFile(TrieNode *root, char *filename)
{
	int num;
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	FILE * ifp = fopen(filename, "r");
	if (ifp == NULL)
		return 1;
	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		// Prints the predictions starting with string
		if (buffer[0] == '@')
		{
			fscanf(ifp, "%s%d", buffer, &num);
			printPrediction(root, buffer, num);
		}
		// pritns out the entire trie
		else if (buffer[0] == '!')
		{
			printTrie(root, 0);
		}
		// prints out the subtrie of the string
		else
		{
			printSubTrie(root, buffer);
		}
	}
	fclose(ifp);
	return 0;
}

// Recursivly frees the trie starting with the subtrie then the child then the root node
TrieNode *destroyTrie(TrieNode *root)
{
	int i;
	if (root == NULL)
		return NULL;
	if (root->subtrie != NULL)
		root->subtrie = destroyTrie(root->subtrie);

	for (i = 0; i < NUM_CHILDREN; i++)
	{
		if (root->children[i] != NULL)
			root->children[i] = destroyTrie(root->children[i]);
	}


	free(root);
	return NULL;
}

// Finds the last node of the str in the trie and returns it if count >= 1
TrieNode *getNode(TrieNode *root, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *temp = root;
	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;
		index = getIndex(str[i]);
		if (temp->children[index] == NULL)
			return NULL;
		temp = temp->children[index];
	}

	if (temp->count >= 1)
		return temp;
	else
		return NULL;
}

// Finds the node in the trie with the highest count and returns that node
TrieNode *findFrequentNode(TrieNode *root)
{
	int i;
	TrieNode *temp = root, *ret;
	if (root == NULL)
		return NULL;
	for (i = 0; i < NUM_CHILDREN; i++)
	{
		if (root->children[i] != NULL)
		{
			ret = findFrequentNode(root->children[i]);
			// not >= because alphabectially temp will come before ret if there is a tie
			if (ret->count > temp->count)
				temp = ret;
		}
	}
	return temp;
}

// Gets the lowercase char of an index from 0 - 25 which corresponds to a - z
char indexReversal(int index)
{
	return index + 'a';
}

//Given the last node of the word in the trie finds the word and stores it in str
int convertNodetoStr(TrieNode *root, TrieNode *temp, char *str)
{
	int i, ret;
	if (temp == NULL)
		return 1;
	for (i = 0; i < NUM_CHILDREN; i++)
	{
		if (root->children[i] != NULL)
		{
			// if the next node is last node of the word then it stores the value of the index
			// to str[0] and the null sentinal at str[1]
			if (root->children[i] == temp)
			{
				str[0] = indexReversal(i);
				str[1] = '\0';
				return 1;
			}
			else
			{
				// passes str + 1 as we are a character deeper in the trie
				ret = convertNodetoStr(root->children[i], temp, str + 1);
				// indicates that temp was later in the trie and to store values in str and return
				if (ret == 1)
				{
					str[0] = indexReversal(i);
					return 1;
				}
			}
		}
	}
	return 0;
}

// Finds the word that occurs the most in the trie based on its count and stores it into str
void getMostFrequentWord(TrieNode *root, char *str)
{
	int ret;
	TrieNode *temp;
	// clears str from past values
	str[0] = '\0';
	temp = findFrequentNode(root);
	ret = convertNodetoStr(root, temp, str);
}

// Checks if the word is contained in the function
// uses getNode as has the same functionality with a different return value
int containsWord(TrieNode *root, char *str)
{
	if (getNode(root, str) != NULL)
		return 1;
	else
		return 0;
}

// Counts the number of words stores in a trie
// Count includes the times a word is repeated
int countWords(TrieNode *root)
{
	int i, ret = root->count;
	for (i = 0; i < NUM_CHILDREN; i++)
	{
		if (root->children[i] != NULL)
			ret += countWords(root->children[i]);
	}
	return ret;
}

// Same as getNode exoect it always returns the last node no matter the count as it is just
// The prefixs last node not the words last node
TrieNode *getPrefixNode(TrieNode *root, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *temp = root;
	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;
		index = getIndex(str[i]);
		if (temp->children[index] == NULL)
			return NULL;
		temp = temp->children[index];
	}


	return temp;
}

// Sees how many words come after a prefix of str
// Goes to the last node of the prefix then counts the words that follows that node
int prefixCount(TrieNode *root, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *temp = root;
	if (root == NULL)
		return 0;
	temp = getPrefixNode(root, str);
	if (temp == NULL)
		return 0;
	return countWords(temp);
}

// Counts the amount of new nodes that would need to be created if str was entered into the trie
// Sees if each letter of str is represented in the trie if not returns the lenght minus
// the amount of nodes that already exists in the trie of str
int newNodeCount(TrieNode *root, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *temp = root;
	if (root == NULL)
		return len + 1;
	// Checks if each node co
	for (i = 0; i < len; i++)
	{
		index = getIndex(str[i]);
		if (temp->children[index] == NULL)
		{
			return len - i;
		}
		else
		{
			temp = temp->children[index];
		}
	}
	return 0;
}

double difficultyRating(void)
{
	return 2.75;
}

double hoursSpent(void)
{
	return 5.25;
}

// reads in the command line args to create the trie and preform the actions in the input file
int main(int argc, char **argv)
{
	int ret;
	TrieNode *root;
	if (argc != 3)
		return -1;
	root = buildTrie(argv[1]);
	ret = processInputFile(root, argv[2]);
	root = destroyTrie(root);
	return ret;
}
