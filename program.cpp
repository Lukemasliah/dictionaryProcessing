#include <iostream>
#include <fstream>
#include <string>

struct Node {
	std::string item;
	Node *leftNode;
	Node *rightNode;
};

// Part 1, 2 & 4 methods
void findEmordnilap();
void reverseWord(std::string &word, char *revWord, int length);
int find(char *revWord);
int binarySearch(std::string revWord);
int binarySearchEmords(std::string revWord, int found);
bool isAnagram(std::string s, std::string t);

// BST
struct Node* search(std::string item, Node* node);
struct Node* newNode(std::string item);
struct Node* insert(std::string item, Node* node);
void countUnique(struct Node* root);
void spellCheck(std::string &word);
void findUniqueWords(Node* root);

// GLOBAL VARIABLES
std::string list[400000] = {""};
std::string foundEmords[1600] = {""};
int wordCounter = 0;
int bstCounter = 1;	//Starts at one because root is initialised with a value
std::string uniqueWords[300];
int longestEmordCounter;
int longestEmordPosition;
int uniqueCounter = 0;	// Unique words matched in dictionary
int totalUnique = 0;	// Unique words found

int main()
{
	// PART 1 & PART 2 **************************************************
	
	double clock1 = clock();
	double clock2 = 0;
	double clock3 = 0;
	
	std::ifstream inFile;
	std::string word;
	
	//Read dictionary into an array
	inFile.open("dictionary-v2.txt");
	if(inFile.is_open())
	{
		while(inFile >> word)
		{
			list[wordCounter] = word;
			wordCounter++;
		}
	}
	inFile.close();
	
	//Perform search and display results
	std::cout << "\nNumber of words in dictionary: " << wordCounter << std::endl;
	findEmordnilap();
	clock2 = clock();
	clock1 = clock1/CLOCKS_PER_SEC;
	clock2 = clock2/CLOCKS_PER_SEC;
	std::cout << "Total run time (secs): " << (clock2 - clock1) << std::endl;
	
	// PART 3 ***************************************************************
	
	inFile.open("sample.txt");
	Node* root = NULL;

	if (inFile.is_open())
	{
		inFile >> word;
		spellCheck(word);
		
		// Create a node object and insert the first value.
		root = newNode(word);
		
		while (inFile >> word)
		{
			// Correct word and check if it is reduced to 0 size before adding to array
			spellCheck(word);
			if(word.length() > 0)
			{
				insert(word, root);
				bstCounter++;
			}
		}
		inFile.close();
	}
	else
	{
		std::cout << "File could not open" << std::endl;
	}

	// Display Spell-Check information 
	
	countUnique(root);
	findUniqueWords(root);
	std::cout << "\n-File Read: Sample.txt-" << std::endl;
	std::cout << "Number of Valid words read: " << bstCounter << std::endl;
	std::cout << "Number of Unique words read: " << totalUnique << std::endl;
	std::cout << "Unique words matched in dictionary: " << uniqueCounter << std::endl << std::endl;

	// PART 4 ********************************************************************
	
	// Find Anagrams
	std::cout << "-Lets find Anagrams!-" << std::endl;
	int found = 0;
	for(int i = 0; found < 10;i++)
	{
		// Checks whether the Unique word has anagrams
		for(int p = 0; p < wordCounter; p++)
		{
			// If the word contains atleast 1 anagram, perform another search with printing
			// And break this loop immediately after
			
			if(isAnagram(uniqueWords[i], list[p]))
			{
				std::cout << uniqueWords[i] << ": ";
				
				// Print anagrams
				for(int q = 0; q < wordCounter; q++)
				{
					if(isAnagram(uniqueWords[i], list[q]))
					{
						std::cout << list[q] << " ";
					}
				}
				std::cout << std::endl;
				found++;
				break;
			}	
		}
	}
	
	// Record total run time
	clock3 = clock();
	clock1 = clock1/CLOCKS_PER_SEC;
	clock3 = clock3/CLOCKS_PER_SEC;
	std::cout << "\nTotal run time of program: " << (clock3 - clock1) << std::endl;
	
}


bool isAnagram(std::string s, std::string t)
{
	// False if words are different length or both strings are the same word
	if (s.length() != t.length() || s.compare(t) == 0)
	{
		return false;
	}
	
	int letters[26] = { 0 };
	for(int i = 0; i < s.length(); i++)
	{
		letters[(s[i] - 97)]++;
		letters[(t[i] - 97)]--;
	}
	// Check whether letter values = 0
	for(int i = 0; i < 26; i++)
	{
		if(letters[i] != 0)
			return false;		
	}
	// The values of the letters array are all 0, the strings are anagrams
	return true;
}

struct Node* search(std::string item, Node* node)
{
	// Node is null or searched item exists in tree
	if (node == NULL || node->item == item)
	{
		return node;
	}
	
	// Recurse down the branches searching for the item
	if (item < node->item)
	{
		search(item, node->leftNode);
	}	
	else
	{
		search(item, node->rightNode);
	}		
}


void findUniqueWords(Node* root)
{
	if (root != NULL)
	{
		findUniqueWords(root->leftNode);
		int n = binarySearch(root->item);
		if(n != -1)
		{
			uniqueWords[uniqueCounter] = list[n];
			uniqueCounter++;
		}
		findUniqueWords(root->rightNode);
	}
}

void countUnique(struct Node* root)
{
	if (root != NULL)
	{
		countUnique(root->leftNode);
		totalUnique++;
		countUnique(root->rightNode);
	}
}

void spellCheck(std::string &word)
{
	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] >= 'A' && word[i] <= 'Z')
		{
			word[i] = tolower(word[i]);
		}
		
		if (word[i] < 'a' || word[i] > 'z' || word[i] == ',' || word[i] == '\'')
		{
			word.erase(i);
		}
	}
}


struct Node* newNode(std::string item)
{
	struct Node* newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->item = item;
	newNode->leftNode = NULL;
	newNode->rightNode = NULL;
	return newNode;
}

struct Node* insert(std::string item, Node* node)
{
	Node* next;
	bool left;

	// If Node is empty, return a new Node
	if (node == NULL)
	{
		return newNode(item);
	}
	
	// Move recursively down the tree
	if (item < node->item)
	{
		node->leftNode = insert(item, node->leftNode);
	}
	else if (item > node->item)
	{
		node->rightNode = insert(item, node->rightNode);
	}

	// Return the unchanged Node pointer
	return node;
}


void findEmordnilap()
{
	const int maxEmords = 10;
	char revWord[35];
	int found = 0;
	
	//Find Emordnilap words by performing linear search.
	std::cout << "\n-First " << maxEmords << " Emordnilap words- " << std::endl;
	for(int i = 0; i < wordCounter; i++)
	{
		int len = list[i].length();
		if(len > 1)
		{
			reverseWord(list[i], revWord, len);
			
			if(revWord >= list[i])  // Only search words that are larger or equal to itself
			{
				// Using Linear Find function
				//int n = find(revWord);
				
				// Using binary search
				int n = binarySearch(revWord); 
				
				if(n != -1 && found != maxEmords)
				{
					foundEmords[found] = list[i];
					std::cout << list[i] << " : " << list[n] << std::endl;
					found++;
				}
			}
			else	// Search the array of already found Emords
			{
				int n = binarySearchEmords(revWord, found);
				
				if(n != -1 && found != maxEmords)
				{
					std::cout << list[i] << " : " << foundEmords[n] << std::endl;
				}
			}
		}
	}
	
	std::cout << "Longest Emordnilap word found: " << list[longestEmordPosition] << std::endl;
	std::cout << "Found " << found << " Emordnilap words" << std::endl;
}

int binarySearchEmords(std::string revWord, int found)
{
	//bool found = false;
	int start = 0;
	int end = found;
	
	
	while(start <= end)
	{
		int mid = (start + end)/2;
		if(revWord < foundEmords[mid])
		{
			end = mid - 1;
		}
		else if(revWord > foundEmords[mid])
		{
			start = mid + 1;
		} 
		else //match found at position mid
		{
			return mid;
		}
	}
	// Word not found
	return -1;
}

int binarySearch(std::string revWord)
{
	int start = 0;
	int end = wordCounter;
	
	while(start <= end)
	{
		int mid = (start + end)/2;
		if(revWord < list[mid])
		{
			end = mid - 1;
		}
		else if(revWord > list[mid])
		{
			start = mid + 1;
		} 
		else //match found at position mid
		{
			// Find first longest word found
			if(list[mid].length() > longestEmordCounter)
			{
				longestEmordCounter = list[mid].length();
				longestEmordPosition = mid;
			}
			return mid;
		}
	}
	// Word not found
	return -1;
}

int find(char *revWord)
{
	for(int i = 0; i < wordCounter; i++)
	{
		if(list[i].compare(revWord) == 0)
		{
			//Emordnilap match found.
			return i;
		}
	}
	// No matches found
	return -1;
}

void reverseWord(std::string &word, char *revWord, int length)
{
	int i, j;
	
	for(i = 0, j = length-1; i < length; i++, j--)
	{
		revWord[i] = word[j];
	}
	revWord[i] = '\0';
}





/* Running times --------------------------------------------------------------------------------

Step 1: - Linear search to find n words
	10: 77 secs (without longest Emord found)
	All (estimate): 346 mins & 30 secs

Step 2: - Binary search to find n words
	10: 4.74 secs (with Longest Emord found)
	All: 4.75 secs

Program total run time: 15.54 secs

Data Structures and Algorithms

Find Emordnilap: Uses a binary search to partition possible results very quickly. Found an immense 
	improvement on linear search speeds.
Spell Check: Uses binary tree to store words, utilizing several inorder search functions.
Find Anagrams: Uses an algorithm based on comparing ascii values of two strings to evaluate 
	whether they are equal or not.

Extra Enhancements: 
1.  Improved search speeds for emordnilap words by not searching for a reversed word
	if its value is less than the current linear position in the dictionary. These words instead 
	search through a seperate 'already found' emordnilap array.
2.  Implemented pass by reference with pointers where possible.

-Machine Specifications- 
Operating System: Windows 10 Pro 64-bit (10.0, Build 17763)
Processor: AMD Ryzen 5 1600X Six-Core Processor            (12 CPUs), ~3.6GHz
Memory: 16384MB RAM
Compiling on Banshee

*///-------------------------------------------------------------------------------------------






	