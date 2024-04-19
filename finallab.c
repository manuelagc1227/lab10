#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct TrieNode* children[ALPHABET_SIZE];
    int count; // Number of occurrences of the word ending at this node

};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    // Start from the root of the trie
    struct Trie *current = pTrie;
    
    // For each character in the word:
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        
        // Check if the character's corresponding child node exists
        if (current->children[index] == NULL) {
            // If it doesn't exist, create a new node and link it to the current node
            current->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            // Initialize the new node
            for (int j = 0; j < 26; j++) {
                current->children[index]->children[j] = NULL;
            }
            current->children[index]->isEndOfWord = 0; // Initialize isEndOfWord flag
        }
        
        // Move to the child node corresponding to the current character
        current = current->children[index];
    }
    
    // Mark the last node as the end of a word
    current->isEndOfWord = 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    // Start from the root of the trie
    struct Trie *current = pTrie;
    
    // For each character in the word:
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        
        // Check if the character's corresponding child node exists
        if (current->children[index] == NULL) {
            // If any character is missing, the word doesn't exist in the trie
            return 0;
        }
        
        // Move to the child node corresponding to the current character
        current = current->children[index];
    }
    
    // After traversing the word, if the current node's isEndOfWord flag is set,
    // the word exists in the trie and its count can be returned
    if (current->isEndOfWord) {
        return 1; // If the word exists, return 1 (as it occurs once)
    } else {
        return 0; // Otherwise, the word doesn't exist in the trie
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    // If trie is NULL, return NULL
    if (pTrie == NULL) {
        return NULL;
    }

    // Recursively deallocate children nodes
    for (int i = 0; i < 26; i++) {
        if (pTrie->children[i] != NULL) {
            deallocateTrie(pTrie->children[i]);
        }
    }

    // Free memory for current node
    free(pTrie);

    return NULL; // Return NULL to indicate deallocation is complete
}

// Initializes a trie structure
struct Trie *createTrie()
{
    // Allocate memory for the root node of the trie
    struct Trie *root = (struct Trie *)malloc(sizeof(struct Trie));
    if (root == NULL) {
        // Memory allocation failed
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize the root node
    for (int i = 0; i < 26; i++) {
        root->children[i] = NULL;
    }
    
    // Set isEndOfWord flag for root node to 0
    root->isEndOfWord = 0;
    
    return root; // Return the pointer to the root node
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    int wordCount = 0;
    char buffer[100]; // Assuming maximum word length is 100 characters

    // Read each word from the file and store it in pInWords
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline character if present
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        // Allocate memory for the word and copy it to pInWords
        pInWords[wordCount] = strdup(buffer);
        if (pInWords[wordCount] == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        wordCount++;
    }

    fclose(file);
    return wordCount;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}