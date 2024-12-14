/*
Project 2: Search
Class: CS 251 Fall 2024
Author: Freya Modi
*/

#include "include/search.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string cleanedToken = "";
  bool containsTheLetter = false;

  for (int i = 0; i < token.size(); ++i) {
    if (!ispunct(token[i])) {
      for (int j = i; j < token.size(); ++j) {
        if (!ispunct(token[j])) {
          cleanedToken += tolower(token[j]);
          if (isalpha(token[j])) {
            containsTheLetter = true;
          }
        } else if (cleanedToken.empty()) {
          i++;
        } else if (!cleanedToken.empty()) {
          cleanedToken += (token[j]);
        } else {
          break;
        }
      }
      break;
    }
  }

  for (int k = cleanedToken.size() - 1; k >= 0; --k) {
    if (!ispunct(cleanedToken[k])) {
      cleanedToken = cleanedToken.substr(0, k + 1);
      break;
    }
  }

  if (!containsTheLetter) {
    cleanedToken = "";
  }
  return cleanedToken;
}

set<string> gatherTokens(const string& text) {
  set<string> result;
  istringstream istream(text);
  string storeWord;
  while (istream >> storeWord) {
    string getToken = cleanToken(storeWord);
    if (!getToken.empty()) {
      result.insert(getToken);
    }
  }
  return result;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  ifstream inFile(filename);
  string getURL, getText;
  int count = 0;

  while (getline(inFile, getURL)) {
    if (!getline(inFile, getText)) {
      return -1;
    }
    set<string> getTokens = gatherTokens(getText);

    for (const string& tokens : getTokens) {
      index[tokens].insert(getURL);
    }
    count++;
  }
  return count;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  istringstream isstream(sentence);
  string getToken, cleanedToken = "";
  char modifier;
  set<string> results;
  bool findFirstTerm = true;

  while (isstream >> getToken) {
    modifier = ' ';
    if (getToken[0] == '+' || getToken[0] == '-') {
      modifier = getToken[0];
      getToken = getToken.substr(1);
    }

    cleanedToken = cleanToken(getToken);
    auto findInt = index.find(cleanedToken);
    set<string> getURLS;

    if (findInt != index.end()) {
      getURLS = findInt->second;
    }
    if (findFirstTerm) {
      results = getURLS;
      findFirstTerm = false;
    } else {
      if (modifier == '+') {
        set<string> intersection;
        set_intersection(results.begin(), results.end(), getURLS.begin(),
                         getURLS.end(),
                         inserter(intersection, intersection.begin()));
        results = intersection;
      } else if (modifier == '-') {
        set<string> difference;
        set_difference(results.begin(), results.end(), getURLS.begin(),
                       getURLS.end(), inserter(difference, difference.begin()));
        results = difference;
      } else {
        set<string> unionSet;
        set_union(results.begin(), results.end(), getURLS.begin(),
                  getURLS.end(), inserter(unionSet, unionSet.begin()));
        results = unionSet;
      }
    }
  }
  return results;
}

void searchEngine(const string& filename) {
  map<string, set<string>> getIndex;
  int numCounts = buildIndex(filename, getIndex);
  int getSize = getIndex.size();
  bool check = true;
  string userInput;

  ifstream inFile(filename);
  if (!inFile) {
    cout << "Invalid filename.\n";
  }

  cout << "Stand by while building index...\nIndexed " << numCounts
       << " pages containing " << getSize << " unique terms\n";

  while (check) {
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, userInput);
    if (userInput.empty()) {
      check = false;
      break;
    }
    if (check) {
      set<string> getResults = findQueryMatches(getIndex, userInput);
      cout << "Found " << getResults.size() << " matching pages\n";
      for (const string& printResults : getResults) {
        cout << printResults << endl;
      }
    }

  }
  cout << "Thank you for searching!\n";
}
