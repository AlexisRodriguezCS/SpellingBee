
#include "SBTrie.h"
#include "Trie.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using std::cout;
using std::cin;
using std::string;

void getNewDictionary(SBTrie *sbt, string filename){
  // enter needed code here for command 1
  sbt->setNewDictionary(filename);
}

void updateDictionary(SBTrie *sbt, string filename){
  // enter needed code here for command 2
  sbt->addToDictionary(filename);
}

void setupLetters(SBTrie *sbt, string letters){
  // enter needed code here for command 3
  sbt->setLetters(letters);
}

void showLetters(SBTrie *sbt){
  // enter needed code here for command 4
  sbt->getLetters();
}

void attemptWord(SBTrie *sbt, string letters){
  // enter needed code here for command 5
  sbt->tryWord(letters);
}

void showFoundWords(SBTrie *sbt){
  // enter needed code here for command 6
  sbt->getFoundWords();
}

void showAllWords(SBTrie *sbt){
  // enter needed code here for command 7
  sbt->getAllWords();
}

void displayCommands(){
  cout << "\nCommands are given by digits 1 through 9\n\n";
  cout << "  1 <filename> - read in a new dictionary from a file\n";
  cout << "  2 <filename> - update the existing dictionary with words from a file\n";
  cout << "  3 <7letters> - enter a new central letter and 6 other letters\n";
  cout << "  4            - display current central letter and other letters\n";
  cout << "  5 <word>     - enter a potential word\n";
  cout << "  6            - display found words and other stats\n";
  cout << "  7            - list all possible Spelling Bee words from the dictionary\n";
  cout << "  8            - display this list of commands\n";
  cout << "  9            - quit the program\n\n";

}

int main(int argc, char** argv){
  SBTrie *sbt = new SBTrie;
  
  cout << "Welcome to Spelling Bee Game\n";

  displayCommands();
  
  bool done = false;
  string line;
  string input;

  do {
    cout << "cmd> ";

    // read a complete line
    std::getline(std::cin, line);

    // now create a "stringstream" on the line just read
    std::stringstream ss(line);

    // clear input from any previous value
    input = "";

    // get command character
    char command;
    ss >> command;
    //cout << "Debug command:" << command << "***\n";
    
    if(command == '1'){
        ss >> input;
        //cout << std::setw(17) << input << " " << std::right << std::setw(2) << input.length() << std::left << endl;
        //cout << "Debug 1:" << input << "***\n";
        getNewDictionary(sbt, input);
    }

    if(command == '2'){
        ss >> input;
        //cout << "Debug 2:" << input << "***\n";
        updateDictionary(sbt, input);
    }
        
    if(command == '3'){
        ss >> input;
        cout << "Debug 3:" << input << "***\n";
        setupLetters(sbt, input);
    }

    if(command == '4'){
        showLetters(sbt);
    }

    if(command == '5'){
        ss >> input;
        //cout << "Debug 5:" << input << "***\n";
        attemptWord(sbt, input);
    }

    if(command == '6'){
        showFoundWords(sbt);
    }

    if(command == '7'){
        showAllWords(sbt);
    }

    if(command == '8' || command == '?'){
        displayCommands();
    }

    if(command == '9' || command == 'q'){
        done=true;
    }

  } while(!done && !cin.eof());
  delete sbt;
  return 0;
}
