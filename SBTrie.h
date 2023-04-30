
#ifndef _MY_SB_TRIE_H
#define _MY_SB_TRIE_H

#include "Trie.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class SBTrie : public Trie {

  // the main reason for inheriting the Trie class is that the words() method in
  // Tries is quite inefficient for the use of the Spelling Bee game.
  //
  // This allows us to customize a better method to get the desired words by
  // "adding an addtional method" on the the Trie class at the cost of using
  // inheritance.

protected:
  // private/protected stuff goes here!
  //  - protected allows this class to be inherited
  //    and adapted to the problem at hand (if needed)
  //   typedefs
  //   data members
  char centralLetter;
  string allowedLetters;
  int score;
  Trie *dictionary;
  Trie *wordsFoundSoFar;
  bool pangramFound;
  bool bingoFound;
  //   definition of trie node structure
  //   private helper functions
  bool contains(vector<char> letters, char input) {
    for (auto letter : letters) {
      if (tolower(input) == letter) {
        cout << "contains letter: " << input << endl;
        return true;
      }
    }
    return false;
  }
  //   etc.

public:
  /**
   * constructor and destructor
   */
  SBTrie() {
    // your constructor code here!
    score = 0;
    wordsFoundSoFar = new Trie();
    dictionary = new Trie();
    pangramFound = false;
    bingoFound = false;
  }
  ~SBTrie() {
    // your destructor code here.
    wordsFoundSoFar->clear();
    delete wordsFoundSoFar;
    dictionary->clear();
    delete dictionary;
  }
  /*
   * function: sbWordsHelper
   * description: Goes through all the nodes in the trie
   *    by using recursion. Loops the letters and adds
   *    only the valid words to the vector.
   *
   * return:  nothing
   */
  void sbWordsHelper(TrieNode *currNode, vector<string> *trieWords, string prevChars) const {
    // loop through the allowed letters
    for ( auto letter : allowedLetters){
      int index = (int)letter - (int)'a';
      // check if letter is a next node
      if (currNode->letters[index] != nullptr){
        // check if it is a word
        if (currNode->letters[index]->isWord){
          string word = prevChars + letter;
          // check if its a valid word and add it
          bool cLetter = (word.find(centralLetter) != string::npos);
          if (word.length() >= 4 && cLetter){
            trieWords->push_back(word);
          }
        }
        sbWordsHelper(currNode->letters[index], trieWords, prevChars + letter);
      }
    }
  }
  /*
   * function: sbWords
   * description:  build a vector of all words in the dictionary that
   *   are valid words for the spelling bee problem and return that
   *   vector.  The words are to be in sorted ascending order.
   *   The words included must have a length of 4 or greater,
   *                      must contain the central letter, and
   *                      may contain the allowed letters
   *
   * parameter: char centralLetter - the letter that MUST be contained in the
   * words string letters - the other letters that are allowed to be in words
   *
   * return:  a pointer to a vector of strings
   *
   */
  std::vector<string> *sbWords(char centralLetter, string letters) const {
    vector<string> *trieWords = new vector<string>();
    sbWordsHelper(dictionary->getRoot(), trieWords, "");
    return trieWords;
  }

  /*
   * function: setNewDictionary
   * description:  removes the existing dictionary from the trie and
   *		insert the new file into the trie.
   *
   * parameter: string filename - file the we are going to use to
   * read in words.
   *
   * return:  void
   */
  void setNewDictionary(string filename) {
    dictionary->clear();
    dictionary->getFromFile(filename);
  }

  /*
   * function: addToDictionary
   * description:  adds onto the existing dictionary by insert the file
   *		data into the dictionary trie.
   *
   * parameter: string filename - file that is opened and read from.
   *
   * return:  void
   */
  void addToDictionary(string filename) { dictionary->getFromFile(filename); }

  /*
   * function: setLetters
   * description:  sets the central letter and populates the vector
   *		allowedLetters with 6 other letters.
   *
   * parameter: string letters - input from the user that needs
   * to contains 7 different letters.
   *
   * return:  void
   */
  void setLetters(string letters) {
    string myLetters = "";
    int counter = 0;
    for (auto character : letters){
      if (isalpha(character) && myLetters.find(character) == string::npos) {
        // add to vector?
        counter++;
        myLetters += tolower(character);
      }
    }
    if (counter != 7) {
      cout << "Invalid letter set." << endl;
      return;
    }
    centralLetter = myLetters[0];
    sort(myLetters.begin(), myLetters.end());
    allowedLetters = myLetters;
    wordsFoundSoFar->clear();
    score = 0;
    pangramFound = false;
    bingoFound = false;
  }

  /*
   * function: getLetters
   * description:  prints the central letter and the other 6 letters that
   *		can be used to make a word
   *
   * return:  void
   */
  void getLetters() {
    cout << "Central Letter: " << centralLetter << endl;
    cout << "6 Other Letters: ";
    for (int j = 0; j < 7; j++) {
      if (j != 6 && allowedLetters[j] != centralLetter) {
        cout << allowedLetters[j] << ",";
      }
      if (j == 6 && allowedLetters[j] != centralLetter){
        cout << allowedLetters[j] << endl;
      }
    }
  }

  /*
   * function: invalidLettersCheck
   * description:  goes through the input and checks if any char
   *		is not a letter inside allowedLetters vector.
   *
   * parameter: string input - input from the user that contains
   * 7 different letters.
   *
   * return:  indicates success/failure
   */
  bool invalidLettersCheck(string input) {
    for (auto character : input) {
      // check if character is not allowed
      if (allowedLetters.find(character) == string::npos){
        return true;
      }
    }
    return false;
  }

  /*
   * function: calculateScore
   * description: calculates the amount of points the user earned by
   * 	taking into accound the length of the word and if the word was
   * 	a pangram they get an extra 7 points
   *
   * parameters: string input - input from the user that contains
   * 	7 different letters. bool isPangram - represents if the word
   * 	is a pangram
   *
   * return:  an integer containing the amount of points for finding
   * 	the current word.
   */
  int calculateScore(string input, bool &isPangram) {
    int totalPoints = input.length() - 3;
    isPangram = checkForPangram(input);
    if (isPangram) {
      // Add to score
      totalPoints += 7;
    }
    return totalPoints;
  }

  /*
   * function: checkForPangram
   * description: checks if the input contains every allowed
   * 	letter
   *
   * parameter: string input - input from the user that contains
   * 7 different letters.
   *
   * return:  indicates success/failure
   */
  bool checkForPangram(string input) {
    for (auto letter : allowedLetters) {
      // check if letter is in input
      if (input.find(letter) == string::npos){
        return false;
      }
    }
    pangramFound = true;
    return true;
  }

  /*
   * function: checkForBingo
   * description: goes through the Trie containing all the
   * 	words found so far and checks if there is a word
   * 	found starting with every allowed letter
   *
   *
   * return:  indicates success/failure
   */
  bool checkForBingo() {
    // Temporary pointer to the root
    TrieNode *currentNode = wordsFoundSoFar->getRoot();
    // Loop through every letter in the allowed letters
    for (auto letter : allowedLetters) {
      // Index where the current character should be
      int index = letter - 'a';
      // Check if theres a word with the starting letter
      if (currentNode->letters[index] == nullptr) {
        // No bingo
        return false;
      }
    }
    bingoFound = true;
    return true;
  }

  /*
   * function: tryWord
   * description: the user attemps a word(input) and the word goes
   * 	a series of checks. First checks if the word is at least 4
   * 	characters then check if the central letter is in the word.
   * 	Next checks if the word is made of letters from the vector
   * 	of allowed letters. Then checks if the input is a word and
   * 	not already discovered. Finally checks if the input is a
   * 	pangram and if bingo is scored.
   *
   * parameter: string input - input from the user that contains
   * 7 different letters.
   *
   * return:  void
   */
  void tryWord(string input) {
    // Lower the input
    for (size_t k = 0; k < input.length(); k++) {
      input[k] = tolower(input[k]);
    }
    // Checks that the word is at least 4 characters
    bool isShort = false;
    if (input.length() < 4) {
      isShort = true;
      cout << "word is too short" << endl;
      return;
    }
    // Checks if word does not contain central letter
    size_t found = input.find(centralLetter);
    if (found == string::npos) {
      cout << "word is missing central letter" << endl;
      return;
    }
    // Checks for invalid letters
    bool invalidLetters = invalidLettersCheck(input);
    if (invalidLetters) {
      cout << "word contains invalid letter" << endl;
      return;
    }
    bool isWord = dictionary->search(input);
    bool inputIsPangram = false;
    bool inputIsBingo = false;
    // Checks if the word is not in the dictionary
    if (!isWord) {
      cout << "word is not in the dictionary" << endl;
      return;
    }
    if (isWord && !invalidLetters && !isShort) {
      if (wordsFoundSoFar->search(input)) {
        // Check if word is not already discovered
        cout << "word has already been found" << endl;
      } else {
        // word is not discovered yet
        wordsFoundSoFar->insert(input);
        cout << "found " << input << " ";
        // Do score calculation
        int currentScore = calculateScore(input, inputIsPangram);
        cout << currentScore;
        (currentScore > 1) ? cout << " points" : cout << " point";
        score += currentScore;
        cout << ", total " << score;
        (score > 1) ? cout << " points" : cout << " point";
        // Check for Pangram
        if (inputIsPangram) {
          pangramFound = true;
          cout << ", Pangram found";
        }
        // Check for Bingo
        if (checkForBingo()) {
          bingoFound = true;
          cout << ", Bingo scored";
        }
        cout << endl;
      }
    }
  }

  /*
   * function: getFoundWords
   * description: displays all found words in
   *  alphabetical order, 1 per line. Last line
   *  display total count and score, and if player
   *  has gotten a pangram and/or Bingo.
   *
   * parameter: None
   *
   * return:  void
   */
  void getFoundWords() {
    vector<string> *words = wordsFoundSoFar->words();
    for (auto word : *words){
      cout << word << endl;
    }
    cout << words->size() << " words found, total " << score << " points";
    if (pangramFound){
      cout << ", Pangram found";
    }
    if (bingoFound){
      cout << ", Bingo scored";
    }
    cout << endl;
    delete words;
  }

  /*
   * function: getAllWords
   * description: display all spelling bee words
   *  1 per line and the length of the word after 20
   *  characters/spaces and Pangram status.
   *
   * parameter: None
   *
   * return:  void
   */
  void getAllWords() {
    vector<string> *words = sbWords(centralLetter, allowedLetters);
    for (auto word : *words){
      // if word is too long just print word and 1 space
      if (word.length() > 17){
        cout << word << " ";
      }
      // else print word then spaces
      else {
        cout << word;
        for (int i = word.length(); i < 19; i++){
          cout << " ";
        }
      }
      cout << word.length();
      // check if word is long enough and is a Pangram
      if (word.length() > 6 && checkForPangram(word)){
        cout << " Pangram";
      }
      cout << endl;
    }
    delete words;
  }
};

#endif
