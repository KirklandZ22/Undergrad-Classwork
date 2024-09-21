// COP3502 Spring 2022

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIFF INT_MIN

// Struct Definitions given by professor
typedef struct ListyNode
{
	// Each node holds a single character.
	char data;

	// Pointer to next node in linked list.
	struct ListyNode *next;
} ListyNode;

typedef struct ListyString
{
	// Head of the linked list representing this string.
	ListyNode *head;

	// Length of this linked list.
	int length;
} ListyString;


// Functional prototypes of helper functions
char *listyToString(ListyString *listy);
void deleteChar(ListyString *listy, char key);

// Read in command line args
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("You are the worst :(\n");
        return 0;
    }

    processInputFile(argv[1]);
    return 0;
}

// Read input file and change strings based on file commands
// Returns 1 if file does not exist else returns 0
int processInputFile(char *filename)
{
    char str[1024];
    ListyString *listy;
    FILE *ifp;
    if ((ifp = fopen(filename, "r")) == NULL)
        return 1;

    fscanf(ifp, "%s", str);
    listy = createListyString(str);

    while (fscanf(ifp, "%s", str) != EOF)
    {
        if (str[0] == '@')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            if ((fscanf(ifp, "%s", str + 1)) == EOF)
                break;
            replaceChar(listy, str[0], str + 1);
        }
        else if (str[0] == '>')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            listy = listyCat(listy, str);
        }
        else if (str[0] == '<')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            listy = listyPrepend(listy, str);
        }
        else if (str[0] == '~')
        {
            reverseListyString(listy);
        }
        else if (str[0] == '*')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            listyCensor(listy, str);
        }
        else if (str[0] == '-')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            replaceChar(listy, str[0], "");
        }
        else if (str[0] == '&')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            listy = stringWeave(listy, str);
        }
        else if (str[0] == '#')
        {
            if ((fscanf(ifp, "%s", str)) == EOF)
                break;
            printf("%d\n", charCount(listy, str[0]));
        }
        else if (str[0] == '?')
        {
            printf("%d\n", listyLength(listy));
        }
        else if (str[0] == '!')
        {
            printListyString(listy);
        }
    }

    listy = destroyListyString(listy);
    fclose(ifp);

    return 0;
}

// Dynamicall allocate new ListyString with length of length of the string
// and data from the string. Runtime is O(k) where k is the length of the string
// Return new ListyString pointer
ListyString *createListyString(char *str)
{
    int i, len;
    ListyNode *tail = NULL, *new_tail = NULL, *temp;
    ListyString *listy;

    if ((listy = (ListyString *)malloc(sizeof(ListyString))) == NULL)
        return NULL;

    listy->head = NULL;
    listy->length = 0;
    if (str == NULL || (len = strlen(str)) == 0)
        return listy;

    for (i = 0; i < len; i++)
    {
        if ((new_tail = (ListyNode *)malloc(sizeof(ListyNode))) == NULL)
        {
            listy = destroyListyString(listy);
            return NULL;
        }

        new_tail->data = str[i];
        new_tail->next = NULL;
        if (i == 0)
        {
            listy->head = tail = new_tail;
            listy->length ++;
            continue;
        }

        tail->next = new_tail;
        tail = new_tail;
        listy->length ++;
    }
    return listy;
}

// Free all dynamic memory alloaction of ListyStrings and ListyNodes
// Runtime is O(n) where n is the length of the ListyString
// Return NULL to indicate freed ListyString
ListyString *destroyListyString(ListyString *listy)
{
    ListyNode *temp, *temp_next;
    if (listy == NULL)
        return NULL;

    if (listy->head == NULL)
    {
        free(listy);
        return NULL;
    }
    else
    {
        temp = listy->head;
        temp_next = temp->next;
    }

    while (temp != NULL)
    {
        temp_next = temp->next;
        free(temp);
        temp = temp_next;
    }

    free(listy);
    return NULL;
}

// Take the data from ListyString and put it in a dynamically allocated string
// Runtime is O(n) where n is the length of ListyString
// Return string
char *listyToString(ListyString *listy)
{
    char *str;
    int i;
    ListyNode *temp;
    if (listy == NULL)
        return NULL;

    if (listy->head == NULL)
        return "";

    if ((str = (char *)malloc(sizeof(char) * (listy->length + 1))) == NULL)
        return NULL;

    temp = listy->head;
    for (i = 0; i < listy->length; i++)
    {
        str[i] = temp->data;
        temp = temp->next;
    }
    str[i] = '\0';
    return str;
}

// Create a deep copy of the ListyString in new ListyString
// Runtime is O(n) where n is length of ListyString
// Return the new ListyString
ListyString *cloneListyString(ListyString *listy)
{
    char *str;
    int i;
    ListyString *new_listy;
    if (listy == NULL)
        return NULL;

    // Runtime for listyToString is O(n) where n is the length of str
    str = listyToString(listy);

    // Runtime is O(n) where n is the length of str which is equal to length of ListyString
    new_listy = createListyString(str);

    if (new_listy->length == 0)
        return new_listy;

    free(str);
    return new_listy;
}

// Add the string to the end of ListyString
// Runtime is O(n + m) where n is the length of ListyString and m is length of new string
// Return ListyString pointer
ListyString *listyCat(ListyString *listy, char *str)
{
    int i, len;
    ListyNode *tail, *new_tail;
    ListyString *new_string;
    if (str == NULL || (len = strlen(str)) == 0)
        return listy;
    tail = listy->head;
    for (i = 0; i < listy->length - 1; i++)
            tail = tail->next;

    // Runtime for createListyString is O(m) where m is the length of str
    new_string = createListyString(str);

    if (listy->length != 0)
        tail->next = new_string->head;
    else
        listy->head = new_string->head;

    listy->length += new_string->length;
    free(new_string);

    return listy;
}

// Add the string to the beginning of ListyString
// Runtime is O(n + m) where n is length of listy and m is length of str
// return ListyString pointer
ListyString *listyPrepend(ListyString *listy, char *str)
{
    int i, len;
    ListyNode *temp;
    ListyString *temp_string;
    if (str == NULL && listy == NULL)
        return NULL;

    if (str == NULL || (len = strlen(str)) == 0)
        return listy;

    if (listy == NULL && len != 0)
        return createListyString(str);

    // Runtime for createListyString is O(m) where m is the length of str
    temp_string = createListyString(str);
    temp = temp_string->head;

    for (i = 0; i < len - 1; i++)
    {
        temp = temp->next;
    }

    temp->next = listy->head;
    listy->head = temp_string->head;
    listy->length += temp_string->length;

    free(temp_string);
    return listy;
}

// Removes all instances of key from ListyString
// Runtime O(n) where n is the length of ListyString
// Return nothing
void deleteChar(ListyString *listy, char key)
{
    ListyNode *temp = listy->head, *temp_next;

    if (listy == NULL || listy->head == NULL)
        return;

    // check if head is key
    while (listy->head->data == key)
    {
        if (listy->length == 1)
        {
            free(listy->head);
            listy->length = 0;
            listy->head = NULL;
            return;
        }

        listy->head = temp->next;
        free(temp);
        listy->length --;
        temp = listy->head;
    }

    temp_next = temp->next;

    while (temp_next != NULL)
    {
        if (temp_next->data == key)
        {
            temp->next = temp_next->next;
            free(temp_next);
            listy->length --;
            if (temp == NULL)
                break;

            temp_next = temp->next;
            continue;
        }

        temp = temp->next;
        temp_next = temp->next;
    }

    return;
}

// Replaces all instances of key with str in ListyString
// If str is NULL or empty then deletes the key using deleteChar
// Runtime O( n + km) where n is the lenght of ListyString, k is number of times key occurs
// and m is the length of str
void replaceChar(ListyString *listy, char key, char *str)
{
    int len;
    ListyNode *temp, *temp_next;
    ListyString *temp_string;

    if (listy == NULL || listy->head == NULL)
        return;

    if (str == NULL || (len = strlen(str)) == 0)
    {
        deleteChar(listy, key);
        return;
    }

    temp = listy->head;

    // If head is key
    if (temp->data == key)
    {
        // Runtime for createListyString is O(m) where m is the length of str
        temp_string = createListyString(str);
        temp = temp_string->head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = listy->head->next;
        free(listy->head);
        listy->head = temp_string->head;
        listy->length += temp_string->length - 1;
        free(temp_string);
    }

    temp_next = temp->next;

    while (temp_next != NULL)
    {
        if (temp_next->data == key)
        {
           // Runtime for createListyString is O(m) where m is the length of str
            temp_string = createListyString(str);
            temp->next = temp_string->head;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = temp_next->next;
            free(temp_next);
            listy->length += temp_string->length - 1;

            if (temp == NULL)
                break;

            temp_next = temp->next;
            free(temp_string);
            continue;
        }

        temp = temp->next;
        temp_next = temp->next;
    }
}

// Replaces all badChars in ListyString with '*'
// Runtime is O(k + n) where k is length of badChars and n is lenght of Listy
void listyCensor(ListyString *listy, char *badChars)
{
    int len, i;
    ListyNode *temp;
    int *censor;

    if (badChars == NULL || (len = strlen(badChars)) == 0)
        return;

    if (listy == NULL || listy->head == NULL)
        return;

    censor = (int *)calloc(256, sizeof(int));
    if (censor == NULL)
        return;

    // O(k)
    for (i = 0; i < len; i++)
    {
        censor[badChars[i]] = 1;
    }

    temp = listy->head;

    // O(n)
    for (i = 0; i < listy->length; i++)
    {
        if (censor[temp->data] == 1)
            temp->data = '*';
        temp = temp->next;
    }

    free(censor);
}

// Reverse the ListyNodes within ListyString
// Runtime is O(n) where n is the length of ListyString
void reverseListyString(ListyString *listy)
{
    int i;
    char *str;
    ListyNode *temp;

    if (listy == NULL || listy->head == NULL)
        return;

    temp = listy->head;
    // Runtime for listyToString is O(n) where n is the length of str
    str = listyToString(listy);

    // O(n)
    for (i = listy->length; i > 0; i--)
    {
        temp->data = str[i-1];
        temp = temp->next;
    }

    free(str);
}

// Weave the chars of str between the nodes of the ListyString
// Creates new nodes of str and link them to ListyString
// Runtime is O(n+k) where n is the length of listy and k is the length of str
// Return ListyString pointer
ListyString *stringWeave(ListyString *listy, char *str)
{
    int i = 0, len;
    ListyNode *temp1, *temp1_next, *new_node;
    ListyString *new_listy;

    if (str == NULL)
        return listy;

    if (listy == NULL)
        return createListyString(str);

    if (listy->head == NULL)
    {
        new_listy = createListyString(str);
        listy->head = new_listy->head;
        listy->length = new_listy->length;
        free(new_listy);
        return listy;
    }

    temp1 = listy->head;
    len = strlen(str);

    // Runtime is O(MIN{n, k}) where n is the length of listy and k is the length of str
    while (temp1->next != NULL && i < len)
    {
        temp1_next = temp1->next;

        if ((new_node = (ListyNode *)malloc(sizeof(ListyNode))) == NULL)
            return NULL;

        new_node->data = str[i];
        temp1->next = new_node;
        new_node->next = temp1_next;

        temp1 = temp1_next;
        i++;
    }

    // if str is longer then listy then add the rest of str to then end of Listy
    if (temp1->next == NULL)
    {
        // Runtime for createListyString is O(j) where j is the length of str - i
        new_listy = createListyString(str + i);
        temp1->next = new_listy->head;
        free(new_listy);
    }

    listy->length += len;

    return listy;
}

// Weave the data of listy2 between the nodes of listy1
// listy1 is modified with the new data from copies of listy2
// Runtime is O(n + m) where n is the length of listy1 and m is lenght of listy2
// Return listy1 pointer
ListyString *listyWeave(ListyString *listy1, ListyString *listy2)
{
    ListyNode *temp1, *temp1_next, *temp2, *temp2_next, *new_node;
    ListyString *listy2_copy;

    if (listy2 == NULL)
    {
        return listy1;
    }

    // Runtime O(m)
    listy2_copy = cloneListyString(listy2);

    if (listy1 == NULL)
    {
        listy1 = listy2_copy;
        return listy1;
    }
    if (listy1->head == NULL)
    {
        listy1->head = listy2_copy->head;
        listy1->length = listy2_copy->length;
        free(listy2_copy);
        return listy1;
    }

    temp1 = listy1->head;
    temp2 = listy2_copy->head;

    // O(MIN(n, m))
    while (temp1->next != NULL && temp2 != NULL)
    {
        temp1_next = temp1->next;
        temp2_next = temp2->next;


        temp1->next = temp2;
        temp2->next = temp1_next;

        temp1 = temp1_next;
        temp2 = temp2_next;
    }

    if (temp1->next == NULL)
    {
        temp1->next = temp2;
    }

    listy1->length += listy2->length;
    free(listy2_copy);
    return listy1;
}

// Compares the data of 2 ListyStrings to see if they are the same
// Runtime is O(n + m) where n is the length of listy1 and m is the length of listy2
// Return 0 if the ListyStrings are the same and DIFF if different
int listyCmp(ListyString *listy1, ListyString *listy2)
{
    int i;
    ListyNode *temp1, *temp2;
    if (listy1 == NULL)
    {
        return (listy2 == NULL) ? 0 : DIFF;
    }

    if (listy2 == NULL)
        return DIFF;

    if (listy1->length != listy2->length)
        return DIFF;

    temp1 = listy1->head;
    temp2 = listy2->head;

    // Runtime O(k) where k is the lenght of the listy where the listys are the same length
    for (i = 0; i < listy1->length; i++)
    {
        if (temp1->data != temp2->data)
            return DIFF;

        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    return 0;
}

// Counts the amount of times a char is present in ListyString
// Runtime is O(n) where n is the length of listy
int charCount(ListyString *listy, char key)
{
    int ret = 0, i;
    ListyNode *temp;

    if (listy == NULL)
        return -1;
    if (listy->head == NULL)
        return 0;

    temp = listy->head;

    // Runtime O(n)
    for (i = 0; i < listy->length; i++)
    {
        if (temp->data == key)
            ret++;

        temp = temp->next;
    }

    return ret;
}

// Returns the length of listy if listy is not NULL
// Returns -1 if listy is NULL
// Runtime is O(1)
int listyLength(ListyString *listy)
{
    return (listy == NULL) ? -1 : listy->length;
}

// Converts the listy string to a string to print the ListyString
void printListyString(ListyString *listy)
{
    int i;
    char *str;
    ListyNode *temp;
    if (listy == NULL || listy->head == NULL)
    {
        printf("(empty string)\n");
        return;
    }

    // Runtime O(n)
    str = listyToString(listy);
    printf("%s\n", str);
    free(str);
}

double difficultyRating(void)
{
    return 3.8;
}

double hoursSpent(void)
{
    return 16.5;
}
