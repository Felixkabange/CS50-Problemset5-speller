// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to get the index in the hash table
    unsigned int index = hash(word);

    // Traverse the linked list at the index
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // Compare word with the current node's word (case-insensitive)
        if (strcasecmp(word, cursor->word) == 0)
        {
            // Word found in the dictionary
            return true;
        }
        cursor = cursor->next;
    }

    // Word not found in the dictionary
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Simple hash function summing ASCII values of characters
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += tolower(word[i]);
    }

    // Reduce hash to fit within the range of the hash table size
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Clear the hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Read words from dictionary and insert into hash table
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy the word into the node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash the word to get the index
        unsigned int index = hash(word);

        // Insert the node at the beginning of the linked list at the index
        if (table[index] == NULL)
        {
            table[index] = new_node;
        }
        else
        {
            new_node->next = table[index];
            table[index] = new_node;
        }
    }

    // Close dictionary file
    fclose(file);

    // Dictionary loaded successfully
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int count = 0;

    // Traverse the hash table and count the words
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            count++;
            cursor = cursor->next;
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate over each bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        // Set the current node to the first node in the bucket
        node *current = table[i];

        // Free the memory for each node in the bucket
        while (current != NULL)
        {
            node *temp = current;
            current = current->next;
            free(temp);
        }
    }

    return true;
}

