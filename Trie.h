
#ifndef _MY_TRIE_H
#define _MY_TRIE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Trie {

protected:
  // private/protected stuff goes here!
  //  - protected allows this class to be inherited
  //    and adapted to the problem at hand (if needed)
  //   typedefs
  //   definition of trie node structure
  struct TrieNode {
    vector<TrieNode *> letters;
    bool isWord;
    TrieNode() : letters(26, nullptr) {
      // cout << "TrieNode Constructor Called\n";
      isWord = false;
    }
    ~TrieNode() {
      // cout << "TrieNode Destructor Called\n";
      for (int i = 0; i < 26; i++) {
        letters[i] = nullptr;
        delete letters[i];
      }
    }
  };
  //   data members
  TrieNode *root;
  int wordCounter;
  //   private helper functions
  //   etc.

  int letterCount(TrieNode *currNode) {
    int count = 0;
    for (int i = 0; i < 26; i++) {
      if (currNode->letters.at(i) != nullptr) {
        count++;
      }
    }
    return count;
  }

  /*
   * function: lowerString
   * description: Goes through all the letters in the string
   * 	and lowers every letter.
   *
   * return:  void
   */
  void lowerString(string &input) {
    for (auto &letter : input) {
      letter = tolower(letter);
    }
  }

  /*
   * function: wordsHelper
   * description: Goes through all the nodes in the trie
   *    by using recursion. Loops the letters and adds
   *    any words to its vector. Then returns it
   *     to the parent recursive step.
   *
   * return:  vector of strings from Trie
   */
  void wordsHelper(TrieNode *currNode, vector<string> *trieWords,
                   string prevChars) const {
    // loop through all letters
    for (int i = 0; i < 26; i++) {
      // if we are at a letter
      if (currNode->letters[i] != nullptr) {
        // get the current letter
        char letter = (char)((int)'a' + i);
        // if we are at a word then add it
        if (currNode->letters[i]->isWord) {
          string word = prevChars + letter;
          trieWords->push_back(word);
          // cout << word << " ";
        }
        // go further in recursion while adding letter
        wordsHelper(currNode->letters[i], trieWords, prevChars + letter);
      }
    }
  }
  /*
   * function: clearHelper
   * description: removes all the nodes in the trie
   *    by using recursion. Loops until it reaches
   *    a leaf node. Then it deletes the leaf node
   *    and returns to the parent node and repeats
   *    until it reaches a nullptr node.
   *
   *
   * return:  nothing
   */
  void clearHelper(TrieNode *currentNode) {
    for (int i = 0; i < 26; i++) {
      if (currentNode->letters[i] != nullptr) {
        clearHelper(currentNode->letters[i]);
      }
    }
    delete currentNode;
  }
  /*
   * function: isWord
   * description:  loops through every character in the word
   *   that has been extracted from the file. Words must
   *   only contain letters.
   *
   *   fails if word contains non-letters
   *
   * return:  indicates success/failure
   */
  bool isValidWord(string word) {
    // Checks if word only contain letters
    for (int i = 0; i < word.length(); i++) {
      if (!isalpha(word.at(i))) {
        // Not a word
        return false;
      }
    }
    return true;
  }

public:
  /**
   * constructor and destructor
   */
  Trie() {
    // your constructor code here!
    // cout << "Trie Constructor Called\n";
    root = new TrieNode();
    wordCounter = 0;
  }
  ~Trie() {
    // your destructor code here.
    // cout << "Trie Destructor Called\n";
    // Clear will delete every node in Trie
    clear();
    delete root;
  }

  /*
   * function: getFromFile
   * description:  extract all of the words from the file
   *   specified by the filename given in the parameter.
   *   Words are separated by whitespace characters and
   *   must only contain letters.
   *
   * return:  indicates success/failure (file not readable...)
   */
  bool getFromFile(string filename) {
    ifstream fileStream;
    // cout << "Opening file: " << filename << endl;
    fileStream.open(filename);
    if (!fileStream.is_open()) {
      // cout << "Could not open file " << filename << "." << endl;
      return false;
    }
    // cout << "Reading words from file\n";
    string word;
    while (fileStream >> word) {
      // If the word only contains letters
      if (isValidWord(word)) {
        // Calls insert function to put in Trie
        lowerString(word);
        insert(word);
      }
    }
    fileStream.close();
    return true;
  }

  /*
   * function: insert
   * description:  inserts the word given by the parameter
   *   into the trie data structure.
   *
   *   fails if word already exists or contains non-letters
   *
   * return:  indicates success/failure
   */
  bool insert(string word) {
    // Checking if word contains non alphanumeric characters
    if (!isValidWord(word)) {
      return false;
    }
    // Checking if the word exist in Trie already
    if (search(word)) {
      return false;
    }
    // Insert into the Trie
    TrieNode *currentNode = root;
    for (auto character : word) {
      // Index where the character is going to be inserted in TrieNode vector
      int index = character - 'a';
      if (currentNode->letters[index] == nullptr) {
        // cout << "Add to Trie: " << character << endl;
        currentNode->letters[index] = new TrieNode();
      }
      // Move to the new TrieNode that was just made
      // For example word "apple", "a" -> "ap"
      currentNode = currentNode->letters[index];
    }
    // Update the counter after the word has been inserted
    // and mark it as a word
    currentNode->isWord = true;
    wordCounter++;
    return true;
  }

  /*
   * function: search
   * description:  determines if the word given by the parameter
   *   is stored in the trie data structure.
   *
   *   fails if word already does not exist
   *
   * return:  indicates success/failure
   */
  bool search(string word) const {
    // Empty Trie
    if (root == nullptr) {
      return false;
    }
    // Temporary pointer to the root
    TrieNode *currentNode = root;
    // Loop through every character in the word
    for (auto character : word) {
      // Index where the current character should be
      int index = character - 'a';
      // Check if the character does exist
      if (currentNode->letters[index] == nullptr) {
        // cout << "Does not exist: " << character << endl;
        return false;
      }
      // Goes down to the next TrieNode
      // For example word "apple":
      // "a" -> "ap" -> "app" -> "appl" -> "apple"
      currentNode = currentNode->letters[index];
    }
    // Returns true if the current TrieNode is mark as a word
    return currentNode->isWord;
  }

  /*
   * function: remove
   * description:  removes the word given by the parameter
   *   from the trie data structure.  Only deallocate nodes
   *   in subtrees that do not contain any words.
   *
   *   succeed if word exists and is properly removed
   *
   * return:  indicates success/failure
   */
  bool remove(string word) {
    // Empty Trie
    if (root == nullptr) {
      return false;
    }
    // visited vector to keep path of the word
    vector<TrieNode *> *visited = new vector<TrieNode *>();
    vector<int> *indexes = new vector<int>();
    TrieNode *currNode = root;
    // loop to the end of the word
    for (auto character : word) {
      int index = character - 'a';
      // if next letter doesnt exist return false
      if (currNode->letters[index] == nullptr) {
        return false;
      }
      // collect node for the path
      visited->push_back(currNode);
      indexes->push_back(index);
      // go to next node
      currNode = currNode->letters[index];
    }
    // mark word as false and add last node to visited
    currNode->isWord = false;
    visited->push_back(currNode);
    // loop through visited backwards
    int visitedIndex = visited->size() - 1;
    int currLetter;
    while (visitedIndex > 0 && letterCount(currNode) == 0 &&
           currNode->isWord == false) {
      // get current letter from indexes
      currLetter = indexes->at(visitedIndex - 1);
      // null the reference to the node and delete it
      TrieNode *parentNode = visited->at(visitedIndex - 1);
      parentNode->letters.at(currLetter) = nullptr;
      delete currNode;
      visitedIndex--;
      currNode = visited->at(visitedIndex);
    }
    if (letterCount(root) == 0) {
      delete root->letters.at(currLetter);
    }
    delete visited;
    delete indexes;
    return true;
  }

  /*
   * function: clear
   * description:  remove all words from the trie and deallocate
   *   all nodes.
   *
   * return:  indicates success/failure
   */
  bool clear() {
    // Deallocates all nodes
    clearHelper(root);
    // New TrieNode root will have no words
    root = new TrieNode();
    // Reset the counter for total words
    wordCounter = 0;
    return true;
  }

  /*
   * function: wordCount
   * description:  return the number of words currently stored in the trie
   *
   * return:  an integer containing the number of words stored
   *
   * comment/note:  this should have an O(1) runtime.
   */
  int wordCount() const { return wordCounter; }

  /*
   * function: words
   * description:  build a vector of all words in the dictionary and return that
   *   vector.  The words are to be in sorted ascending order.
   *
   * return:  a pointer to a vector of strings
   *
   */
  std::vector<string> *words() const {
    // Empty Trie
    if (root == nullptr) {
      return {};
    }
    vector<string> *trieWords = new vector<string>();
    wordsHelper(root, trieWords, "");
    return trieWords;
  }

  /*
   * function: getRoot
   * description:  returns a pointer to the root of the Trie which
   *	can be use to traverse the Trie.
   *
   * return:  a pointer to the root of the Trie
   *
   */
  TrieNode *getRoot() { return root; }
};

#endif