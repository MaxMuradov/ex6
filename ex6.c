/* ------------------------------------------------------------
Maxim Muradov
346975386
ex6
------------------------------------------------------------ */

//I structured .c flie like .h so its easier to navigate for you

#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128

// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------

void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

/* ------------------------------------------------------------
   2) Creating & Freeing Nodes
   ------------------------------------------------------------ */
PokemonNode *createPokemonNode(PokemonData *data, int printed)//only for mergecloning not to spam
{
    //creating PokemonNode, alloc memory and init data/NULLs toa variables
    if (printed == 1)
        printf("Creating Pokemon %s (ID %d)...\n", data->name, data->id);

    PokemonNode *ptr = (PokemonNode*)malloc(sizeof(PokemonNode));
    if (!ptr)
    {
        printf("Memory allocation failed for PokemonNode.\n");
        return NULL;
    }
    ptr->data = data;
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->parent = NULL;

    return ptr;
}

OwnerNode *createOwner(char *ownerName, PokemonNode *starter)
{
    //creating PokemonNode, alloc memory and init name/root/NULLs to variables
    OwnerNode *ptr = (OwnerNode*)malloc(sizeof(OwnerNode));
    ptr->ownerName =  ownerName;
    ptr->pokedexRoot = starter;
    ptr->next = NULL;
    ptr->prev = NULL;

    return ptr;
}

void freePokemonNode(PokemonNode *node)
{
    //setting to NULL pointers, freeing pointer to node himself
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->data = NULL;
    free(node);
}

void freePokemonTree(PokemonNode *root)
{
    //recursively freeing BST
    if (root == NULL)
        return;

    if (root->left != NULL)
        freePokemonTree(root->left);
    if (root->right != NULL)
        freePokemonTree(root->right);
    
    freePokemonNode(root);
}

void freeOwnerNode(OwnerNode *owner)
{
    //freeing pointers to name/BST and owner himself also setting to NULL ptrs
    if (owner->pokedexRoot != NULL)
        freePokemonTree(owner->pokedexRoot);
    if (owner->ownerName != NULL)
        free(owner->ownerName);
    owner->pokedexRoot = NULL;
    owner->next = NULL;
    owner->prev = NULL;
    owner->ownerName = NULL;
    free(owner);
}

/* ------------------------------------------------------------
   3) BST Insert, Search, Remove
   ------------------------------------------------------------ */

//i added printing to print only if added by user if merged it will not print
PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode, int printing)
{
    //insertingPokemonNode to BST recursively

    if (root == NULL) //if empty BST
    {
        root = newNode;
        return root;
    }

    if (newNode->data->id < root->data->id)//moving left
    {   
        PokemonNode *ptr = insertPokemonNode(root->left, newNode, printing);
        root->left = ptr;
        ptr->parent = root;
    }
    else if (newNode->data->id > root->data->id)//moving right
    {
        PokemonNode*ptr = insertPokemonNode(root->right, newNode, printing);
        root->right = ptr;
        ptr->parent = root;
    }
    else
    {
        //if already exist same id
        if (printing == 1)
            printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", newNode->data->id);
        free(newNode);
    }

    return root;
}

struct Queue* InitQueue() {
    struct Queue *q = (Queue*)malloc(sizeof(struct Queue));
    if (!q) {
        printf("Memory allocation failed for Queue.\n");
        return NULL;
    }
    q->qfirst = NULL;
    q->qlast = NULL;
    return q;
}

PokemonNode* DeQue(Queue *q) {
    if (q->qfirst == NULL)
        return NULL;

    QNode *ptr = q->qfirst;
    q->qfirst = q->qfirst->next;
    if (q->qfirst == NULL)
        q->qlast = NULL;

    PokemonNode *pokemonNode = ptr->pokemonNode;
    free(ptr);
    return pokemonNode;
}

void EnQue(Queue *q, PokemonNode *node) {
    QNode *ptr = (QNode*)malloc(sizeof(QNode));
    if (!ptr) {
        printf("Memory allocation failed for QNode.\n");
        return;
    }
    ptr->pokemonNode = node;
    ptr->next = NULL;

    if (q->qfirst == NULL) {
        q->qfirst = ptr;
        q->qlast = ptr;
    } else {
        q->qlast->next = ptr;
        q->qlast = ptr;
    }
}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id) {
    Queue *q = InitQueue();
    if (!q) return NULL;

    if (root == NULL) {
        while (q->qfirst != NULL) {
            DeQue(q);
        }
        free(q);
        return NULL;
    }

    EnQue(q, root);
    while (q->qfirst != NULL) {
        PokemonNode* ptr = DeQue(q);
        if (!ptr) {
            printf("Error dequeuing node.\n");
            while (q->qfirst != NULL) {
                DeQue(q);
            }
            free(q);
            return NULL;
        }

        if (ptr->data->id == id) {
            while (q->qfirst != NULL) {
                DeQue(q);
            }
            free(q);
            return ptr;
        }

        if (ptr->left != NULL)
            EnQue(q, ptr->left);
        if (ptr->right != NULL)
            EnQue(q, ptr->right);
    }

    while (q->qfirst != NULL) {
        DeQue(q);
    }
    free(q);
    return NULL;
}


PokemonNode* findSuccesor(PokemonNode* ptr)//helper func to find successor
{
    if (ptr == NULL || ptr->right == NULL)
        return NULL;
    ptr = ptr->right;
    while (ptr != NULL && ptr->left != NULL)
        ptr = ptr->left;
    return ptr;
}

PokemonNode *removeNodeBST(PokemonNode *root, int id)//removing node from BST recursively
{
    if (root == NULL)
        return root;

    if (root->data->id > id)
        root->left = removeNodeBST(root->left, id);
    else if (root->data->id < id)
        root->right = removeNodeBST(root->right, id);
    else
    {
        if (root->left == NULL)//situation with only 1 right child
        {
            PokemonNode *tmp = root->right;
            if (tmp != NULL)
                tmp->parent = root->parent;
            free(root);
            return tmp;
        }

        if (root->right == NULL)//with only 1 left child
        {
            PokemonNode *tmp = root->left;
            if (tmp != NULL)
                tmp->parent = root->parent;
            free(root);
            return tmp;
        }

        PokemonNode* succ = findSuccesor(root);//swap node data to remove
        if (succ != NULL)
        {
            PokemonData* tempData = root->data;
            root->data = succ->data;
            succ->data = tempData;
            root->right = removeNodeBST(root->right, succ->data->id);
        }
    }
    return root;
}

PokemonNode *removePokemonByID(PokemonNode *root, int id)//more generic func that calls removeBST
{
    PokemonNode* ptr = searchPokemonBFS(root, id);
    if (ptr == NULL)
    {
        printf("No Pokemon with ID %d found.\n", id);
        return root;
    }
    else
    {
        printf("Removing Pokemon %s (ID %d).\n", ptr->data->name, ptr->data->id);
        root = removeNodeBST(root, id);
    }
    return root;
}

/* ------------------------------------------------------------
4) Generic BST Traversals (Function Pointers)
------------------------------------------------------------ */

void BFSGeneric(PokemonNode *root, VisitNodeFunc visit)//BFS traversizl
{
    Queue *q = InitQueue();
    if (root == NULL)
    {
        free(q);
        return;
    }

    EnQue(q, root);
    
    while (q->qfirst != NULL)
    {
        PokemonNode* ptr = DeQue(q);

        visit(ptr);

        if (ptr->left != NULL)
            EnQue(q, ptr->left);
        if (ptr->right != NULL)
            EnQue(q, ptr->right);
    }
    free(q);
    return;
}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit)//preorder traversial
{
    if (root == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    PokemonNode *ptr = root;

    visit(ptr);

    if(ptr->left != NULL)
        preOrderGeneric(ptr->left, visit);

    if(ptr->right!= NULL)    
        preOrderGeneric(ptr->right, visit);

    return;
}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit)//inorder traversial
{
    if (root == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    PokemonNode *ptr = root;

    if(ptr->left != NULL)
        inOrderGeneric(ptr->left, visit);

    visit(ptr);
    //free(ptr);

    if(ptr->right!= NULL)    
        inOrderGeneric(ptr->right, visit);

    return;
}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit)//postorder traversial
{
    if (root == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    PokemonNode *ptr = root;

    if(ptr->left != NULL)
        postOrderGeneric(ptr->left, visit);

    if(ptr->right!= NULL)    
        postOrderGeneric(ptr->right, visit);

    visit(ptr);
    //free(ptr);

    return;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
        node->data->id,
        node->data->name,
        getTypeName(node->data->TYPE),
        node->data->hp,
        node->data->attack,
        (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

/* ------------------------------------------------------------
   5) Display Methods (BFS, Pre, In, Post, Alphabetical)
   ------------------------------------------------------------ */

void initNodeArray(NodeArray *na, int cap)//init NodeArray and check if ptr != NULL
{
    if (na == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    na->size = 0;
    na->capacity = cap;
    na->nodes = (PokemonNode**)malloc(cap * sizeof(PokemonNode*));
}

void addNode(NodeArray *na, PokemonNode *node)//adding node to array, realloc if needed
{
    na->size++;
    if (na->size >= na->capacity)
    {
        na->capacity = na->capacity * 2;
        na->nodes = (PokemonNode**)realloc(na->nodes, na->capacity * sizeof(PokemonNode*));
    }
    na->nodes[na->size - 1] = node;
}

void collectAll(PokemonNode *root, NodeArray *na)//collecting pokemonNodes preordertraversial
{
    if (root == NULL)
        return;

    addNode(na, root);

    if (root->left != NULL)
        collectAll(root->left, na);

    if (root->right != NULL)
        collectAll(root->right, na);
}

NodeArray* collectAllBFS(PokemonNode* root)//BFS with queue in the form of array
{
    if (root == NULL)
        return NULL;

    int front = 0;
    NodeArray *qu = (NodeArray*)malloc(sizeof(NodeArray));
    if (!qu)
    {
        printf("Memory allocation failed for NodeArray.\n");
        return NULL;
    }

    initNodeArray(qu, 1);
    addNode(qu, root);
    while (front < qu->size)
    {
        PokemonNode *ptr = qu->nodes[front++];

        if (ptr->left != NULL)
        {
            addNode(qu, ptr->left);
        }

        if (ptr->right != NULL)
        {
            addNode(qu, ptr->right);
        }
    }

    return qu;
    //check if work corrctly(i think it's but it was rebundant)
    //never used but cool
}

int compareByNameNode(const void *a, const void *b)//compare names but returns (< || > || =) 0
{
    const PokemonNode *ptr = *(const PokemonNode **)a;
    const PokemonNode *ptr2 = *(const PokemonNode **)b;
    return strcmp(ptr->data->name, ptr2->data->name);
}

void swapPok(PokemonNode **a, PokemonNode **b)//swap nodes by ptrs
{
    PokemonNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(PokemonNode **na, int low ,int high)//helpfunc to qsort finding pivot + sort part
{
    PokemonNode *pivot = na[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (compareByNameNode(&na[j], &pivot) <= 0)
        {
            i++;
            swapPok(&na[i], &na[j]);
        }
    }
    swapPok(&na[i + 1], &na[high]);
    return i + 1;
}

void quicksort(PokemonNode **pokArray, int low ,int high)//qsort itself
{
    if (low < high)
    {
        int pi = partition(pokArray, low, high);
        quicksort(pokArray, low, pi - 1);
        quicksort(pokArray, pi + 1, high);
    }
}

void displayAlphabetical(PokemonNode *root)//traverse BST, sorts and prints
{
    NodeArray *na = (NodeArray*)malloc(sizeof(NodeArray));
    initNodeArray(na, 1);
    collectAll(root, na);

    quicksort(na->nodes, 0, na->size - 1);

    for (int i = 0; i < na->size; i++)
        printPokemonNode(na->nodes[i]);
    
    free(na->nodes);
    free(na);
}

void displayBFS(PokemonNode *root)//i think i've seen it somewhere
{
    BFSGeneric(root, printPokemonNode);
   // NodeArray *na;
    //na = collectAllBFS(root);
    //for (int i = 0; i < na->size; i++)
     //   printPokemonNode(na->nodes[i]);
     //ex how was planned to use BFS with array
}

void preOrderTraversal(PokemonNode *root)
{
    preOrderGeneric(root, printPokemonNode);
}

void inOrderTraversal(PokemonNode *root)
{
    inOrderGeneric(root, printPokemonNode);
}

void postOrderTraversal(PokemonNode *root)
{
    postOrderGeneric(root, printPokemonNode);
}

/* ------------------------------------------------------------
   6) Pokemon-Specific
   ------------------------------------------------------------ */

void pokemonFight(OwnerNode *owner)//no comments
{
    if (owner->pokedexRoot == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    int id1 = readIntSafe("Enter ID of first Pokemon: ");
    int id2 = readIntSafe("Enter ID of second Pokemon: ");

    PokemonNode *p1 = searchPokemonBFS(owner->pokedexRoot, id1);
    PokemonNode *p2 = searchPokemonBFS(owner->pokedexRoot, id2);

    if (p1 == NULL || p2 == NULL)
    {
        printf("One or both Pokemons ID not found.\n");
        return;
    }

    int score1 = p1->data->hp * 1.5 + p1->data->attack * 1.2;
    int score2 = p2->data->hp * 1.5 + p2->data->attack * 1.2;

    printf("Pokemon1: %s (Score = %d)\n", p1->data->name, score1);
    printf("Pokemon2: %s (Score = %d)\n", p2->data->name, score2);

    if (score1 > score2)
        printf("%s wins!\n", p1->data->name);
    else if (score1 < score2)
        printf("%s wins!\n", p2->data->name);
    else
        printf("It's a tie!\n");
}

void evolvePokemon(OwnerNode *owner)//it was tahatpain the big one
{
    int id = readIntSafe("Enter Pokemon ID to evolve: ");

    //check if exists and can evolve
    PokemonNode *ptr = searchPokemonBFS(owner->pokedexRoot, id);
    if (ptr == NULL)
    {
        printf("No Pokemon with ID %d found.\n", id);
        return;
    }

    if (ptr->data->CAN_EVOLVE == CANNOT_EVOLVE)
    {
        printf("Pokemon %s (ID %d) cannot evolve.\n", ptr->data->name, ptr->data->id);
        return;
    }

    //check evo then evolving (remove + insert)
    PokemonNode* ptrEvo = searchPokemonBFS(owner->pokedexRoot, ptr->data->id + 1);
    if (ptrEvo != NULL)
    {
        printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).\n",
        ptrEvo->data->id, ptrEvo->data->name, ptr->data->name, ptr->data->id);
        owner->pokedexRoot = removeNodeBST(owner->pokedexRoot, id);
        return;
    }

    ptrEvo = createPokemonNode(&pokedex[ptr->data->id], 1);
    char* name = ptr->data->name;
    owner->pokedexRoot = removeNodeBST(owner->pokedexRoot,id);
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, ptrEvo, 1);

    printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n", name, id, ptrEvo->data->name, ptrEvo->data->id);

}

void addPokemon(OwnerNode *owner)//insert just userfriendly
{
    int id = readIntSafe("Enter Pokemon ID: ");
    if (id < 1 || id > 151)
    {
        printf("Invalid ID.\n");
        return;
    }

    // Create a new node
    PokemonNode *newNode = createPokemonNode(&pokedex[id - 1], 1);

    // Check for duplicates
    if (searchPokemonBFS(owner->pokedexRoot, newNode->data->id) != NULL) {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", newNode->data->id);
        freePokemonNode(newNode); // Free the new node to prevent leaks
        return;
    }

    // Insert the new node into the tree (BST logic)
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, newNode, 0);
}

void freePokemon(OwnerNode *owner)//bye bye pikachu
{
    int id = readIntSafe("Enter Pokemon ID to release: ");
    owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, id);  // Update the root
}

// --------------------------------------------------------------
// 7)Display Menu
// --------------------------------------------------------------

void displayMenu(OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case 3:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case 4:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}

/* ------------------------------------------------------------
   8) Sorting Owners (Bubble Sort on Circular List)
   ------------------------------------------------------------ */

void sortOwners(void) //alphabetically
{
    if (ownerHead == NULL || ownerHead->next == ownerHead)//check
    {
        printf("0 or 1 owners only => no need to sort..\n");
        return;
    }

    //break circle
    ownerHead->prev = NULL;
    ownerTail->next = NULL;

    for (OwnerNode *i = ownerHead; i != ownerTail; i = i->next)//sort himself
    {
        for (OwnerNode *j = i->next; j != NULL; j = j->next)
        {
            if (strcmp(i->ownerName, j->ownerName) > 0)
                swapOwnerData(i, j);
        }
    }

    //reconnect
    ownerHead->prev = ownerTail;
    ownerTail->next = ownerHead;
}

void swapOwnerData(OwnerNode *a, OwnerNode *b)//idk what to say fingers burned
{
    OwnerNode* tmp = (OwnerNode*)malloc(sizeof(OwnerNode));
    tmp->ownerName = a->ownerName;
    tmp->pokedexRoot = a->pokedexRoot;

    a->ownerName = b->ownerName;
    a->pokedexRoot = b->pokedexRoot;

    b->ownerName = tmp->ownerName;
    b->pokedexRoot = tmp->pokedexRoot;

    free(tmp);
    tmp = NULL;
}

/* ------------------------------------------------------------
   9) Circular List Linking & Searching
   ------------------------------------------------------------ */

void linkOwnerInCircularList(OwnerNode* newOwner)
{
    if (ownerHead == NULL && ownerTail == NULL)
    {
        ownerHead = newOwner;
        ownerTail = newOwner;
        ownerHead->next = ownerTail;
        ownerHead->prev = ownerTail;
        ownerTail->next = ownerHead;
        ownerTail->prev = ownerHead;
    }
    else
    {
        ownerTail->next = newOwner;
        newOwner->prev = ownerTail;
        ownerTail = newOwner;
        newOwner->next = ownerHead;
        ownerHead->prev = ownerTail;
    }
}

void removeOwnerFromCircularList(OwnerNode *target)
{
    //0
    if (ownerHead == NULL && ownerTail == NULL)
    {
        return;
    }

    //1
    if (ownerHead == ownerTail)
    {
        freeOwnerNode(ownerHead);
        ownerHead = NULL;
        ownerTail = NULL;
        return;
    }

    //break circle
    ownerHead->prev = NULL;
    ownerTail->next = NULL;

    if (target == ownerHead)
    {
        ownerHead = ownerHead->next;
        ownerHead->prev = NULL;
    }
    else if (target == ownerTail)
    {
        ownerTail = ownerTail->prev;
        ownerTail->next = NULL;
    }
    else
    {
    //reconnetion
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }

    freeOwnerNode(target);

    //connect circle
    ownerHead->prev = ownerTail;
    ownerTail->next = ownerHead;

}

OwnerNode *findOwnerByName(const char *name)
{
    if (ownerHead == NULL)
        return NULL;

    OwnerNode *i = ownerHead;
    do
    {
        if (strcmp(i->ownerName, name) == 0)
            return i;
        i = i->next;
    } while (i != ownerHead);
    return NULL;
}

/* ------------------------------------------------------------
   10) Owner Menus
   ------------------------------------------------------------ */

void enterExistingPokedexMenu(void)//menu?!
{
    // list owners
    if (printOwnersNotCircular() == 0)
    {
        printf("No existing Pokedexes.\n");
        return;
    }

    printf("\nExisting Pokedexes:\n");
    int pokedexNum = readIntSafe("Choose a Pokedex by number:\n");

    OwnerNode *cur = ownerHead;
    for (--pokedexNum; pokedexNum > 0; pokedexNum--)
            cur = cur->next;
    
    printf("\nEntering %s's Pokedex...\n", cur->ownerName);

    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
            addPokemon(cur);
            break;
        case 2:
            displayMenu(cur);
            break;
        case 3:
            cur->pokedexRoot = removePokemonByID(cur->pokedexRoot, readIntSafe("Enter Pokemon ID to release: "));
            break;
        case 4:
            pokemonFight(cur);
            break;
        case 5:
            evolvePokemon(cur);
            break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

void openPokedexMenu()//creation of pokedex
{
    printf("Enter your name:\n");
    char *name = getDynamicInput();

    if (findOwnerByName(name) != NULL)
    {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        free(name);
        return;
    }

    int starter;
    printf("Choose starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    starter = readIntSafe("Your choice: ");
    switch (starter)
    {
        case 1:
        starter = 0;
        break;

        case 2:
        starter = 3;
        break;

        case 3:
        starter = 6;
        break;

        default:
        printf("Invalid choice.\n");
    }
    
    
    linkOwnerInCircularList(createOwner(name, createPokemonNode(&pokedex[starter], 1)));//link new owner
    printf("New Pokedex created for %s with starter %s.\n", name, pokedex[starter].name);
}

void deletePokedex(void)//bye bye all of them..
{
    if (printOwnersNotCircular() ==0)
    {
        printf("No existing Pokedexes to delete.\n");
        return;
    }

    printf("\n=== Delete a Pokedex ===\n");
    int delowner = readIntSafe("Choose a Pokedex to delete by number:\n");

    OwnerNode *ptr = ownerHead;
    for (int i = 0; i < delowner - 1; i++)
    {
       ptr = ptr->next;
    }

    printf("Deleting %s's entire Pokedex...\n", ptr->ownerName);
    removeOwnerFromCircularList(ptr);
    printf("Pokedex deleted.\n");
}

void mergePokedexMenu(void) //pain betahat X 151 omg it was annoing
{
    if (ownerHead == NULL || ownerTail == NULL || ownerHead == ownerTail) {
        printf("Not enough owners to merge.\n");
        return;
    }

    printf("Enter name of first owner: ");
    char* name1 = getDynamicInput();
    printf("\nEnter name of second owner: ");
    char* name2 = getDynamicInput();
    printf("\n");

    OwnerNode *owner1 = findOwnerByName(name1);
    OwnerNode *owner2 = findOwnerByName(name2);

    if (owner1 == NULL || owner2 == NULL) {
        printf("One or both owners not found.\n");
        free(name1);
        free(name2);
        return;
    }

    printf("Merging %s and %s...\n", name1, name2);
    NodeArray* na = (NodeArray*)malloc(sizeof(NodeArray));
    initNodeArray(na, 1);
    collectAll(owner2->pokedexRoot, na);

    for (int i = 0; i < na->size; i++) {//cloning from owner 2
        PokemonNode *clone = createPokemonNode(na->nodes[i]->data, 0);
        owner1->pokedexRoot = insertPokemonNode(owner1->pokedexRoot, clone, 0);
    }

    // Clean up owner2
    removeOwnerFromCircularList(owner2);

    printf("Merge completed.\nOwner '%s' has been removed after merging.\n", name2);

    free(name1);
    free(name2);
    free(na->nodes);
    free(na);
}
    

/* ------------------------------------------------------------
   11) Printing Owners in a Circle
   ------------------------------------------------------------ */

void printOwnersCircular(void)
{
    if (ownerHead == NULL && ownerTail == NULL)
    {
        printf("No Owners.\n");
        return;
    }

    //idk in you version only char*[0] is checked i will copy) for real it's
    char* direction;
    char d;
    int i = 1;
    printf("Enter direction (F/B): ");
    while (i == 1) // Loop until valid direction
    {
        direction = getDynamicInput();
        
        //breaking from 4 variations to 2
        if (direction[0] == 'F' || direction[0] == 'f')
        {
            d = '+';
            i--;
        }
        else if (direction[0] == 'B' || direction[0] == 'b')
        {
            d = '-';
            i--;
        }
        else
            printf("Invalid direction.\n");
    }

    int prints = readIntSafe("How many times to print?\n");
    OwnerNode* ptr = ownerHead;

    for (int i = 1; i < prints + 1; i++)//traverse
    {
        if (d == '+')
        {
            printf("[%d] %s\n", i , ptr->ownerName);
            ptr = ptr->next;
        }
        else
        {
            printf("[%d] %s\n", i , ptr->ownerName);
            ptr = ptr->prev;
        }
    }

    free(direction);
}

int printOwnersNotCircular(void)//it was easier for me
{
    if (ownerHead == NULL && ownerTail == NULL)
        return 0;
    
    ownerTail->next = NULL; //break the circle

    OwnerNode* ptr = ownerHead;
    int i = 1;
    while(ptr != NULL)
    {
        printf("%d. %s\n", i++, ptr->ownerName);
        ptr = ptr->next;
    }

    ownerTail->next = ownerHead; //reconnect the circle
    return 1;
}

/* ------------------------------------------------------------
   12) Cleanup All Owners at Program End
   ------------------------------------------------------------ */

void freeAllOwners(void)//freeing all memory
{
    if (ownerHead == NULL && ownerTail == NULL)
    {
        return;
    }

    OwnerNode *ptr = ownerHead;
    ownerTail->next = NULL; //break the circle

    while (ptr != NULL)
    {
        OwnerNode *tmp = ptr->next;
        freeOwnerNode(ptr);
        ptr = tmp;
    }
    
    ownerHead = ownerTail = NULL;
}

/* ------------------------------------------------------------
   13) The Main Menu
   ------------------------------------------------------------ */

void mainMenu()
{
    int choice;
    do
    {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. return\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            enterExistingPokedexMenu();
            break;
        case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        case 7:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}

int main()
{
    mainMenu();
    freeAllOwners();
    return 0;
}
