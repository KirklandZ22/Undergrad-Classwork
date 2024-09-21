// COP3502 Spring 2022


#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Node Struct given by instructor
typedef struct node
{
	// Each node holds a single integer.
	int data;

	// Pointers to the node's left and right children.
	struct node *left, *right;
} node;


// Linked List to hold the preorder and postorder
typedef struct listy
{
    int data;

    struct listy *next;
} listy;


// Functional Prototypes
node *makeTreeNode(int data);
listy *makeListyNode(int data);
listy *preorder(node *a, listy *a_listy);
listy *postorder(node *b, listy *b_listy);
listy *listyDelete(listy *list);
int listyCompare(listy *a, listy *b);


// Recursivly checks if two trees are reflections of each other
// This checks if the roots of the trees are the same then checks
// if the left is equal to the right and the right is equal to the left.
// Returns 1 if they are reflections and 0 if they are not
int isReflection(node *a, node *b)
{
    // If both are NULL then they are the reflections if only one then they are not
    if (a == NULL && b == NULL)
        return 1;
    if (a == NULL || b == NULL)
        return 0;
    // If the data in the root of the tree passed are different then they are not reflections
    if (a->data != b->data)
        return 0;
    // Checks if the left is that same as right and right is same as left recursivly.
    return (isReflection(a->left, b->right) || isReflection(a->right, b->left));
}

// Create a new tree node with the data that is passed in
// Sets both left and right pointers to null
node *makeTreeNode(int data)
{
    node *temp = (node *)malloc(sizeof(node));
    // If malloc failed
    if (temp == NULL)
        return NULL;
    // Initialize Values
    temp->data = data;
    temp->left = temp->right = NULL;

    return temp;
}

// Creates a new tree that is a reflection to the tree passed in.
node *makeReflection(node *root)
{
    node *new = NULL;
    // If the root is NULL then NULL is the reflection
    if (root == NULL)
        return NULL;

    // The roots are the same in the reflections
    new = makeTreeNode(root->data);
    // The left side is made from the right side
    new->left = makeReflection(root->right);
    // The right side is made from the lift side
    new->right = makeReflection(root->left);

    return new;
}

// Creates a new Linked List node with the data that is passed in
// Sets the next pointer to NULL
listy *makeListyNode(int data)
{
    listy *node = (listy *)malloc(sizeof(listy));
    // If malloc fails
    if (node == NULL)
        return NULL;
    // Initialize Values
    node->next = NULL;
    node->data = data;

    return node;
}

// Gets the preorder traversal of the tree at root and stores it in the Linked List passed
// The root of the tree because the tail of the Linked List using head insert
listy *preorder(node *root, listy *list)
{
    listy *temp;
    // Base case
    if (root == NULL)
        return list;

    // If the list has nothing in it (first time running) set the list equal to the node
    if (list == NULL)
    {
        list = makeListyNode(root->data);
    }
    // If the list has something in it set the new node to the head and move the old head back
    else
    {
        temp = makeListyNode(root->data);
        temp->next = list;
        list = temp;
    }
    // Recrusive Calls
    list = preorder(root->left, list);
    list = preorder(root->right, list);

    return list;
}

// Gets the postorder traversal of the tree at root and stores it in the Linked List passed
// The root of the tree because the head of the Linked List using head insert
listy *postorder(node *root, listy *list)
{
    listy *temp;
    // Base case
    if (root == NULL)
        return list;

    // Recrusive Calls
    list = postorder(root->left, list);
    list = postorder(root->right, list);

    // If the list has nothing in it (first time running) set the list equal to the node
    if (list == NULL)
        list = makeListyNode(root->data);
    // If the list has something in it set the new node to the head and move the old head back
    else
    {

        temp = makeListyNode(root->data);
        temp->next = list;
        list = temp;
    }

    return list;
}

// Free the Linked List recursivly
// Frees the head then passes the rest of the list to be freed.
listy *listyDelete(listy *list)
{
    listy *temp;
    // Base Case (returns NULL to prevent access after free)
    if (list == NULL)
        return NULL;
    // temp hold rest of list
    temp = list->next;
    // free head
    free(list);

    return listyDelete(temp);
}

// Checks the values of the list to see if they are the same recursivly
// Returns 1 if same and 0 if different
int listyCompare(listy *a, listy *b)
{
    // If both NULL then they are the same if only one then they are different
    if (a == NULL && b == NULL)
        return 1;
    if (a == NULL || b == NULL)
        return 0;
    // If value not the same theny they are different
    if (a->data != b->data)
        return 0;

    return listyCompare(a->next, b->next);
}

// Sees if the preorder and postorder of the two trees are the same. (order doesnt matter)
// Returns 1 if they are the same and 0 if different. Runtime is O(n) n = tree size
int kindredSpirits(node *a, node *b)
{
    int ret = 0;
    listy *a_listy = NULL, *b_listy = NULL;
    // If both are NULL then same if only one then they are different
    if (a == NULL && b == NULL)
        return 1;
    if (a == NULL || b == NULL)
        return 0;

    // Get the preorder of a and postorder of b and then compare then
    a_listy = preorder(a, a_listy); // O(n)
    b_listy = postorder(b, b_listy); // O(n)
    ret = listyCompare(a_listy, b_listy); // O(n)

    // If the first order was not the same check the other order
    if (ret == 0)
    {
        // Clears the Linked Lists
        a_listy = listyDelete(a_listy); // O(n)
        b_listy = listyDelete(b_listy); // O(n)

        // Get the postorder of a and preorder of b and then compare then
        a_listy = postorder(a, a_listy); // O(n)
        b_listy = preorder(b, b_listy); // O(n)
        ret = listyCompare(a_listy, b_listy); // O(n)
    }

    // Clears the Linked List
    a_listy = listyDelete(a_listy); // O(n)
    b_listy = listyDelete(b_listy); // O(n)

    return ret;
}

double difficultyRating(void)
{
    return 2.35;
}

double hoursSpent(void)
{
    return 8.25;
}
