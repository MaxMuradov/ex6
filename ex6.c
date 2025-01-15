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
OwnerNode *createOwner(char *ownerName, PokemonNode *starter)
{
    OwnerNode *ptr = (OwnerNode*)malloc(sizeof(OwnerNode));
    strcpy(ptr->ownerName, ownerName);
    ptr->pokedexRoot = starter;
    ptr->next = NULL;
    ptr->prev = NULL;
}

PokemonNode *createPokemonNode(const PokemonData *data)
{
    PokemonNode *ptr = (PokemonNode*)malloc(sizeof(PokemonNode));
    ptr->data = data;
    ptr->left = NULL;
    ptr->right = NULL;
}

void freePokemonNode(PokemonNode *node)
{

}

void freePokemonTree(PokemonNode *root)
{

}

void freeOwnerNode(OwnerNode *owner)
{

}

/* ------------------------------------------------------------
   3) BST Insert, Search, Remove
   ------------------------------------------------------------ */

PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode)
{

}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id)
{

}

PokemonNode *removeNodeBST(PokemonNode *root, int id)
{

}

PokemonNode *removePokemonByID(PokemonNode *root, int id)
{

}

/* ------------------------------------------------------------
4) Generic BST Traversals (Function Pointers)
------------------------------------------------------------ */

void BFSGeneric(PokemonNode *root, VisitNodeFunc visit)
{

}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit)
{

}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit)
{

}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit)
{

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

void initNodeArray(NodeArray *na, int cap)
{

}

void addNode(NodeArray *na, PokemonNode *node)
{

}

void collectAll(PokemonNode *root, NodeArray *na)
{

}

int compareByNameNode(const void *a, const void *b)
{

}

void displayAlphabetical(PokemonNode *root)
{

}

void displayBFS(PokemonNode *root)
{

}

void preOrderTraversal(PokemonNode *root)
{

}

void inOrderTraversal(PokemonNode *root)
{

}

void postOrderTraversal(PokemonNode *root)
{

}

/* ------------------------------------------------------------
   6) Pokemon-Specific
   ------------------------------------------------------------ */

void pokemonFight(OwnerNode *owner)
{

}

void evolvePokemon(OwnerNode *owner)
{

}

void addPokemon(OwnerNode *owner)
{

}

void freePokemon(OwnerNode *owner)
{

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

void sortOwners(void)
{

}

void swapOwnerData(OwnerNode *a, OwnerNode *b)
{

}

/* ------------------------------------------------------------
   9) Circular List Linking & Searching
   ------------------------------------------------------------ */

void linkOwnerInCircularList(OwnerNode* newOwner)
{
    if (ownerHead == ownerTail == NULL)
    {
        ownerHead = ownerTail = newOwner;
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

}

OwnerNode *findOwnerByName(const char *name)
{

}

/* ------------------------------------------------------------
   10) Owner Menus
   ------------------------------------------------------------ */

void enterExistingPokedexMenu(void)
{
    // list owners
    printf("\nExisting Pokedexes:\n");
    printOwnersCircular();
    int pokedexNum = readIntSafe("Choose a Pokedex by number:\n");

    OwnerNode *cur = ownerHead;
    for (--pokedexNum; pokedexNum > 0; pokedexNum--)
            cur = cur->next;
    //In your version i can travell over one circle so i will not stop al ownerTail
    

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
            freePokemon(cur);
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

void openPokedexMenu()
{
    printf("Enter your name:\n");
    char *name = getDynamicInput();
    if (!findOwnerByName(name))
    {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        exit(1);
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
    
    
    linkOwnerInCircularList(createOwner(name, createPokemonNode(&pokedex[starter])));
    printf("New Pokedex created for %s with starter %s.\n", name, pokedex[starter].name);
}

void deletePokedex(void)
{

}

void mergePokedexMenu(void)
{

}

/* ------------------------------------------------------------
   11) Printing Owners in a Circle
   ------------------------------------------------------------ */

void printOwnersCircular(void)
{
    if (ownerHead == ownerTail == NULL)
    {
        printf("No owners.\n");
        exit(1);
    }

    //idk in you version only char*[0] is checked i will copy)
    char* direction;
    char d;

    while (1) // Loop until valid direction
    {
        direction = getDynamicInput();
        
        if (direction[0] == 'F' && direction[0] == 'f')
        {
            d = '+';
            break;
        }
        else if (direction[0] == 'B' && direction[0] == 'b')
        {
            d = '-';
            break;
        }
            printf("Invalid direction.\n");
    }

    int prints = readIntSafe("How many times to print?\n");
    OwnerNode* ptr = ownerHead;

    for (int i = 0; i < prints; i++)
    {
        if (d == '+')
        {
            printf("%s\n", ptr->ownerName);
            ptr = ptr->next;
        }
        else
        {
            printf("%s\n", ptr->ownerName);
            ptr = ptr->prev;
        }
    }
}

void printOwnersNotCircular(void)
{
    if (ownerHead == ownerTail == NULL)
    {
        printf("No existing Pokedexes.\n");
        exit(1);
    }
    
    OwnerNode* ptr = ownerHead;
    int i = 1;
    while(ptr != ownerTail)
    {
        printf("%d. %s\n", i, ptr->ownerName);
    }
}

/* ------------------------------------------------------------
   12) Cleanup All Owners at Program End
   ------------------------------------------------------------ */

void freeAllOwners(void)
{

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
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            printOwnersCircular();
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
