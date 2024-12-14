/*
Project 1: Ciphers
Class: CS 251 Fall 2024
Author: Freya Modi
*/

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

void decryptSubstCipherFileCommand(const QuadgramScorer& scorer);

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

/**
 * Print instructions for using the program.
 */
void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

int main() {
  Random::seed(time(NULL));
  string command;

  ifstream inFS;

  inFS.open("dictionary.txt");

  vector<string> dictionary;
  string dictWords;

  while (inFS >> dictWords) {
    dictionary.push_back(dictWords);
  }

  inFS.close();
  inFS.open("english_quadgrams.txt");
  vector<string> quadgram;
  vector<int> countScore;

  string text;
  while (getline(inFS, text)) {
    int findCommaPos = text.find(',');
    if (findCommaPos != string::npos) {
      string quadgramText = text.substr(0, findCommaPos);
      int scoreCounts = stoi(text.substr(findCommaPos + 1));
      quadgram.push_back(quadgramText);
      countScore.push_back(scoreCounts);
    }
  }
  inFS.close();
  QuadgramScorer scorer(quadgram, countScore);

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    if (command == "c" || command == "C") {
      runCaesarEncrypt();
    } else if (command == "D" || command == "d") {
      runCaesarDecrypt(dictionary);

    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    } else if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  // TODO: student fill this in
  int convertToNum = ALPHABET.find(c);
  int rotPos = (convertToNum + amount) % 26;

  return ALPHABET[rotPos];
}

string rot(const string& line, int amount) {
  // TODO: student fill this in

  string rotString = "";
  for (char a : line) {
    if (isalpha(a)) {
      char convertCharToUpper = toupper(a);
      rotString += rot(convertCharToUpper, amount);
    } else if (isspace(a)) {
      rotString += a;
    }
  }
  return rotString;
}

void runCaesarEncrypt() {
  // TODO: student fill this in
  cout << "Enter the text to Caesar encrypt: ";
  string text;
  getline(cin, text);

  cout << "Enter the number of characters to rotate by: ";
  string amountRotStr;
  getline(cin, amountRotStr);

  int amount = stoi(amountRotStr);

  string encryptText = rot(text, amount);

  cout << "Encrypted text: " << encryptText << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student fill this in
  for (string& newStr : strings) {
    for (char& a : newStr) {
      if (isalpha(a)) {
        a = toupper(a);
        a = (a - 'A' + amount) % 26 + 'A';
      }
    }
  }
}

string clean(const string& s) {
  // TODO: student fill this in
  string cleanedStr = "";
  for (char a : s) {
    if (isalpha(a)) {
      cleanedStr += toupper(a);
    }
  }
  return cleanedStr;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student fill this in
  vector<string> storesText;
  string text;
  for (char a : s) {
    if (isspace(a)) {
      if (!text.empty()) {
        storesText.push_back(text);
        text.clear();
      }
    } else {
      text += a;
    }
  }
  if (!text.empty()) {
    storesText.push_back(text);
  }
  return storesText;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student fill this in
  string texts = "";

  for (int i = 0; i < words.size(); ++i) {
    texts += words[i];
    if (i < words.size() - 1) {
      texts += " ";
    }
  }
  return texts;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  // TODO: student fill this in
  int sameNumOfWords = 0;
  for (int i = 0; i < words.size(); ++i) {
    for (int j = 0; j < dict.size(); ++j) {
      if (words[i] == dict[j]) {
        sameNumOfWords += 1;
      }
    }
  }

  return sameNumOfWords;
}

void runCaesarDecrypt(const vector<string>& dict) {
  // TODO: student fill this in
  string userInputText;
  cout << "Enter the text to Caesar decrypt: ";
  getline(cin, userInputText);
  bool check = false;

  vector<string> listWords = splitBySpaces(userInputText);

  for (int i = 0; i < 26; ++i) {
    vector<string> rotWords = listWords;
    rot(rotWords, i);
    int numWords = numWordsIn(rotWords, dict);
    if (numWords > (rotWords.size() / 2)) {
      string decText = joinWithSpaces(rotWords);
      cout << decText << endl;
      check = true;
    }
  }

  if (!check) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student fill this in
  string subText;

  for (char a : s) {
    if (isupper(a)) {
      subText += cipher[a - 'A'];
    } else if (islower(a)) {
      subText += cipher[a - 'a'];
    } else {
      subText += a;
    }
  }
  return subText;
}

void applyRandSubstCipherCommand() {
  // TODO: student fill this in
  string userInputText, subEncryptedText;
  cout << "Enter the text to substitution-cipher encrypt: ";
  getline(cin, userInputText);
  vector<char> generateRandomChipher = genRandomSubstCipher();
  subEncryptedText = applySubstCipher(generateRandomChipher, userInputText);
  cout << subEncryptedText << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student fill this in
  double score = 0.0;
  for (int i = 0; i <= s.length() - 4; ++i) {
    string quadgm = s.substr(i, 4);
    score += scorer.getScore(quadgm);
  }
  return score;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student fill this in
  string userInputText, cleanedText;
  double getScoring = 0.0;

  cout << "Enter a string to score: ";
  getline(cin, userInputText);
  cleanedText += clean(userInputText);
  getScoring = scoreString(scorer, cleanedText);
  cout << "Englishness: " << getScoring << endl;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student fill this in
  vector<char> betterKey = genRandomSubstCipher();

  string betterDecrypt = applySubstCipher(betterKey, ciphertext);
  double betterScore = scoreString(scorer, (betterDecrypt));
  int i = 0;
  while (i < 1500) {
    vector<char> curKey = betterKey;
    int rand = Random::randInt(25);
    int rand2 = Random::randInt(25);
    while (rand == rand2) {
      rand2 = Random::randInt(25);
    }
    swap(curKey[rand], curKey[rand2]);
    string currDecryt = applySubstCipher(curKey, ciphertext);
    double newScore = scoreString(scorer, clean(currDecryt));

    if (newScore > betterScore) {
      betterScore = newScore;
      betterKey = curKey;
      betterDecrypt = currDecryt;
      i = 0;
    } else {
      ++i;
    }
  }

  return betterKey;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  // TODO: student fill this in
  string userInputText, bestText;
  vector<char> bestKey;

  cout << "Enter the text to substitution-cipher encrypt: ";
  getline(cin, userInputText);

  for (int i = 0; i < 20; ++i) {
    vector<char> key = decryptSubstCipher(scorer, clean(userInputText));
    string curText = applySubstCipher(key, userInputText);
    if (i == 0) {
      bestKey = key;
      bestText = curText;
    } else if (scoreString(scorer, clean(curText)) >
               scoreString(scorer, clean(bestText))) {
      bestText = curText;
      bestKey = key;
    }
  }

  cout << bestText << endl;
}

#pragma endregion SubstDec

void decryptSubstCipherFileCommand(const QuadgramScorer& scorer) {
  string userInputFileName, userOutputFileName, getLine, getText, bestText;
  vector<char> bestKey;
  cout << "Enter filename with text to substitution-cipher decrypt: ";
  getline(cin, userInputFileName);
  cout << "Enter filename to write results to: ";
  getline(cin, userOutputFileName);

  ifstream inFS(userInputFileName);
  while (getline(inFS, getLine)) {
    getText += getLine + '\n';
  }
  inFS.close();

  for (int i = 0; i < 20; ++i) {
    vector<char> key = decryptSubstCipher(scorer, clean(getText));
    string curText = applySubstCipher(key, getText);
    if (i == 0) {
      bestKey = key;
      bestText = curText;
    } else if (scoreString(scorer, clean(curText)) >
               scoreString(scorer, clean(bestText))) {
      bestText = curText;
      bestKey = key;
    }
  }
  ofstream outFS(userOutputFileName);
  outFS << bestText;
  outFS.close();
}
