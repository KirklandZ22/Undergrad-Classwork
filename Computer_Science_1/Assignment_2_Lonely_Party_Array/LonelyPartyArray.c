// COP3502 Spring 2022

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Definitions for the program given by instructor
#define UNUSED INT_MIN
#define LPA_SUCCESS -1911218925
#define LPA_FAILURE -1911218926

// Struct for LonelyPartyArray given by instructor
typedef struct LonelyPartyArray
{
    int size;                  // number of occupied cells across all fragments
    int num_fragments;         // number of fragments (arrays) in this struct
    int fragment_length;       // number of cells per fragment
    int num_active_fragments;  // number of allocated (non-NULL) fragments
    int **fragments;           // array of pointers to individual fragments
    int *fragment_sizes;       // stores number of used cells in each fragment
} LonelyPartyArray;

// This function will create a LonelyPartyArray with size accomidations based on inputs
// int num_fragments : num of fragments to be made, int fragment_length: size of a fragment
// Returns: NULL (if invalid memory made or malloc fails) or address of the party made
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
    int i;
    LonelyPartyArray *party;

    if (num_fragments <= 0 || fragment_length <= 0)
        return NULL;

    if ((party = (LonelyPartyArray *) malloc(sizeof(LonelyPartyArray))) == NULL)
        return NULL;

    party->num_fragments = num_fragments;
    party->fragment_length = fragment_length;
    party->num_active_fragments = party->size = 0;

    if ((party->fragments = (int **) malloc(sizeof(int *) * num_fragments)) == NULL)
        {
            free(party);
            return NULL;
        }

    for (i = 0; i < num_fragments; i++)
        party->fragments[i] = NULL;

    if ((party->fragment_sizes = (int *) calloc(num_fragments,sizeof(int))) == NULL)
        {
            free(party->fragments);
            free(party);
            return NULL;
        }

    printf("-> A new LonelyPartyArray has emerged from the void. ");
    printf("(capacity: %d, fragments: %d)\n", num_fragments * fragment_length, num_fragments);
    return party;
}

// Frees party and all party arrays
// Input: party pointer
// Return NULL when completed
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    if (party == NULL)
        return NULL;

    free(party->fragment_sizes);

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
            party->num_active_fragments--;
        free(party->fragments[i]);
    }

    free(party->fragments);
    free(party);

    printf("-> The LonelyPartyArray has returned to the void.\n");
    return NULL;

}

// Insert key into a specific index of party fragments array
// Input: party pointer, index location for key to be put in, key to be inserted
// Returns LPA_FAILURE is key not inserted or returns LPA_SUCCESS if key is inserted
int set(LonelyPartyArray *party, int index, int key)
{
    int i, index1_num, index2_num;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    index1_num = index / party->fragment_length;
    index2_num = index % party-> fragment_length;

    if (index > party->num_fragments * party->fragment_length - 1 || index < 0)
    {
        printf("-> Bloop! Invalid access in set(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, index1_num, index2_num);
        return LPA_FAILURE;
    }

    // If the index of fragment array is not made yet then dynamically create and initalize it.
    if (party->fragments[index1_num] == NULL)
    {
        party->fragments[index1_num] = (int *) malloc(sizeof(int) * party->fragment_length);
        if (party->fragments[index1_num] == NULL)
            return LPA_FAILURE;

        for (i = 0; i < party->fragment_length; i++)
        {
            party->fragments[index1_num][i] = UNUSED;
        }

        party->num_active_fragments++;
        printf("-> Spawned fragment %d. (capacity: %d, ", index1_num, party->fragment_length);
        printf("indices: %d..", party->fragment_length * index1_num);
        printf("%d)\n", ((party->fragment_length) * (index1_num + 1)) -1);
    }

    // Filling the specific index with the key
    if (party->fragments[index1_num][index2_num] == UNUSED)
    {
        party->fragments[index1_num][index2_num] = key;
        party->size++;
        party->fragment_sizes[index1_num]++;
    }
    else
    {
        party->fragments[index1_num][index2_num] = key;
    }

    return LPA_SUCCESS;
}

// Retrieve the value stored at an index
// Input: party pointer and index to retrieve key from
// Return LPA_FAILURE if failed, UNUSED if not dynamically allocated, or key at the index given.
int get(LonelyPartyArray *party, int index)
{
    int index1_num, index2_num;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    index1_num = index / party->fragment_length;
    index2_num = index % party-> fragment_length;

    if (index > party->num_fragments * party->fragment_length - 1 || index < 0)
    {
        printf("-> Bloop! Invalid access in get(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, index1_num, index2_num);
        return LPA_FAILURE;
    }

    if (party->fragments[index1_num] == NULL)
        return UNUSED;

    return party->fragments[index1_num][index2_num];
}

// Set an index to UNUSED (if empties fragment frees fragment)
// Input: party pointer and inndex to clear
// Returns LPA_FAILURE is function cannot delete at specific index or LPA_SUCCESS if item deleted
int delete(LonelyPartyArray *party, int index)
{
    int index1_num, index2_num;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    index1_num = index / party->fragment_length;
    index2_num = index % party-> fragment_length;

    if (index > party->num_fragments * party->fragment_length - 1 || index < 0)
    {
        printf("-> Bloop! Invalid access in delete(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, index1_num, index2_num);
        return LPA_FAILURE;
    }

    if (party->fragments[index1_num] == NULL)
        return LPA_FAILURE;

    if (party->fragments[index1_num][index2_num] == UNUSED)
        return LPA_FAILURE;

    party->fragments[index1_num][index2_num] = UNUSED;
    party->fragment_sizes[index1_num]--;
    party->size--;

    // If by setting the index to UNUSED empties the fragment free the fragment array.
    if (party->fragment_sizes[index1_num] == 0)
    {
        free(party->fragments[index1_num]);
        party->fragments[index1_num] = NULL;
                    printf("-> Deallocated fragment %d. ", index1_num);
                    printf("(capacity: %d, ", party->fragment_length);
                    printf("indices: %d", party->fragment_length * index1_num);
                    printf("..%d)\n", ((party->fragment_length) * (index1_num + 1)) -1);
        party->num_active_fragments--;
    }

    return LPA_SUCCESS;
}

// Checks if a key is in any array
// Input: party pointer and key to see if in array
// Return 0 if key not found or function could not run or 1 if key is found
int containsKey(LonelyPartyArray *party, int key)
{
    int i, j;

    if (party == NULL)
        return 0;

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] == NULL)
            continue;

        for (j = 0; j < party->fragment_length; j++)
        {
            if (party->fragments[i][j] == key)
                return 1;
        }
    }

    return 0;
}

// Find if there is a value at a specific index
// Input: party pointer and index to check
// Return 0 if function could not run, fragment not set, fragment set to unused or return 1 if set
int isSet(LonelyPartyArray *party, int index)
{
    int index1_num, index2_num;

    if (party == NULL)
        return 0;

    index1_num = index / party->fragment_length;
    index2_num = index % party-> fragment_length;

    if (index > party->num_fragments * party->fragment_length - 1 || index < 0)
        return 0;

    if (party->fragments[index1_num] == NULL)
        return 0;

    if (party->fragments[index1_num][index2_num] != UNUSED)
        return 1;

    return 0;
}

// Print the value at an index
// Input: party pointer and index to print value from
// Return LPA_FAILURE if function failed, nothing prints (due to unused) or LPA_SUCCESS if printed
int printIfValid(LonelyPartyArray *party, int index)
{
    int index1_num, index2_num;

    if (party == NULL)
        return LPA_FAILURE;

    index1_num = index / party->fragment_length;
    index2_num = index % party-> fragment_length;

    if (index > party->num_fragments * party->fragment_length - 1 || index < 0)
        return LPA_FAILURE;

    if (party->fragments[index1_num] == NULL)
        return LPA_FAILURE;

    if (party->fragments[index1_num][index2_num] != UNUSED)
    {
        printf("%d\n", party->fragments[index1_num][index2_num]);
        return LPA_SUCCESS;
    }

    return LPA_FAILURE;
}

// Sets the party back to the state when first created
// Input: party pointer
// Return party pointer as if fails party will be NULL or will be the reset pointer
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
        return party;
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
        {
            free(party->fragments[i]);
            party->fragments[i] = NULL;
        }

        party->fragment_sizes[i] = 0;
    }

    party->num_active_fragments = party->size = 0;

    printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: ");
    printf("%d, fragments: ", party->num_fragments * party->fragment_length);
    printf("%d)\n", party->num_fragments);
    return party;
}

int getSize(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    return (party->num_fragments * party->fragment_length);
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    return (party->num_active_fragments * party->fragment_length);
}

// Gets the size if no dynamic memory allocation occured
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
    if (party == NULL)
        return 0;

    return ((long long unsigned int) party->num_fragments * party->fragment_length * sizeof(int));
}

// Get the size of the amount of bytes used from struck and dynamically allocated memory
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
    int i, j;
    long long unsigned int z;

    if (party == NULL)
        return 0;

    z = (long long unsigned int) sizeof(LonelyPartyArray*);
    z += (long long unsigned int) sizeof(LonelyPartyArray);

    for (i = 0; i < party->num_fragments; i++)
    {
        z += ((long long unsigned int) sizeof(party->fragment_sizes[i]));
        z += ((long long unsigned int) sizeof(party->fragments[i]));
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
        {
            for (j = 0; j < party->fragment_length; j++)
                z += ((long long unsigned int) sizeof(party->fragments[i][j]));
        }
     }

    return z;
}

double difficultyRating(void)
{
    return 2.8;
}

double hoursSpent(void)
{
    return 4.7;
}

// Creates an exact replica of the party passed to function at different memory address
// Returns NULL if failed or new party pointer if successful.
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
    int i, j;

    if (party == NULL)
        return NULL;

    LonelyPartyArray *new_party;
    if ((new_party = (LonelyPartyArray *) malloc(sizeof(LonelyPartyArray))) == NULL)
        return NULL;

    new_party->num_fragments = party->num_fragments;
    new_party->fragment_length = party->fragment_length;
    new_party->num_active_fragments = party->num_active_fragments;
    new_party->size = party->size;

    if ((new_party->fragments = (int **) malloc(sizeof(int *) * party->num_fragments)) == NULL)
    {
        free(new_party);
        return NULL;
    }

    if ((new_party->fragment_sizes = (int *) calloc(party->num_fragments,sizeof(int))) == NULL)
    {
        free(new_party->fragment_sizes);
        free(new_party);
        return NULL;
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] == NULL)
        {
            new_party->fragments[i] = NULL;
            new_party->fragment_sizes[i] = 0;
        }
        else
        {
            new_party->fragments[i] = (int *) malloc(sizeof(int) * party->fragment_length);
            if (new_party->fragments[i] == NULL)
            {
                for (j = 0; j < i; j++)
                {
                    if (new_party->fragments[j] != NULL)
                        free(new_party->fragments[j]);
                }

                free(new_party->fragment_sizes);
                free(new_party);
                return NULL;
            }

            for (j = 0; j < party->fragment_length; j++)
            {
                new_party->fragments[i][j] = party->fragments[i][j];
            }
            new_party->fragment_sizes[i] = party->fragment_sizes[i];
        }
    }

    printf("-> Successfully cloned the LonelyPartyArray. (capacity: ");
    printf("%d, ", new_party->num_fragments * new_party->fragment_length);
    printf("fragments: %d)\n", new_party->num_fragments);
    return new_party;
}
