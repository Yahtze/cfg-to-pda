#include "cfg-to-gnf.cpp"
#include "header.h"
#include <charconv>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool GNFcheck() {
  for (auto i : rule) {
    for (auto j : i.second) {
      int n = j.size();
      if (n == 1) {
        if (!islower(j[0])) {
          if (j[0] == '$')
            continue;
          return false;
        }
      }
      else if (n > 1) {
        if (!islower(j[0]))
          return false;
        for (int k = 1; k < n; k++) {
          if (!isupper(j[k]))
            return false;
        }
      }
    }
  }
  return true;
}

void toPDA() {
  cout << "(q0,$,z) -> {(p,Sz)}" << endl;
  map<char, vector<string>> pdaS;
  for (string s : rule['S']) {
    if (s.size() > 1)
      pdaS[s[0]].push_back(s.substr(1, s.size() - 1));
    else
      pdaS[s[0]].push_back("$");
  }
  for (auto j : pdaS) {
    cout << "(q," << j.first << ",S) -> {";
    for (int k = 0; k < j.second.size(); k++) {
      if (k == j.second.size() - 1)
        cout << "(q, " << j.second[k] << ")}";
      else
        cout << "(q, " << j.second[k] << "),";
    }
    cout << endl;
  }
  for (auto i : nonTerminal) {
    if (i != 'S') {
      map<char, vector<string>> pda;
      for (string s : rule[i]) {
        if (s.size() > 1)
          pda[s[0]].push_back(s.substr(1, s.size() - 1));
        else
          pda[s[0]].push_back("$");
      }

      for (auto j : pda) {
        cout << "(q," << j.first << "," << i << ") -> {";
        for (int k = 0; k < j.second.size(); k++) {
          if (k == j.second.size() - 1)
            cout << "(q, " << j.second[k] << ")}";
          else
            cout << "(q, " << j.second[k] << "),";
        }
        cout << endl;
      }
    }
  }
  cout << "(q,$,z) -> {(q1,$)}" << endl;
}
bool equal(string A, string B) {
  bool s = 1;
  if (A.length() == B.length()) {
    for (int i = 0; i < A.length(); i++) {
      if (A[i] != B[i]) {
        s = 0;
        break;
      }
    }
  } else
    s = 0;
  return s;
}
bool inlist(string N, char m) {
  bool t = 0;
  for (int i = 0; i < N.length(); i++)
    if (N[i] == m)
      t = 1;
  return t;
}

int main() {
  string input;
  cout << "\nCONVERT FROM CFG TO PDA\n\n";
  cout << "Instructions: " << endl;
  cout << "(1) Use $ for null" << endl;
  cout << "(2) Each rule is seperated by a comma. End the production rules "
          "with a dot"
       << endl;
  cout << "(3) No spaces in between the input" << endl;
  cout << "(4) LHS contains only one uppercase letter" << endl;
  cout << "(*) Sample: S->abAB,A->aAB|$,B->b.\n\n";
  cout << "Please enter the production rules here: " << endl;
  cin >> input;
  bool check = 0;
  int g1 = input.length() - 1;
  int g2 = input.length() - 2;
  int num_LHS = 0;
  if (input[g1] != '.' || input[g2] == ',') {
    check = 1;
    cout << "Wrong input form" << endl;
  } else {
    for (int i = 0; i < g1; i++) {
      if (check == 1)
        break;
      char a = input[i];
      if (a == '#' || a == '.' || a == '-' || a == '>' || a == '(' ||
          a == ')') {
        check = 1;
        cout << "Wrong input form" << endl;
        break;
      }
      if (!isupper(a)) {
        check = 1;
        cout << "Wrong input form" << endl;
        break;
      }
      num_LHS++;
      char RHS = a;
      i++;
      a = input[i];
      if (a != '-') {
        check = 1;
        cout << "Wrong input form" << endl;
        break;
      }
      i++;
      a = input[i];
      if (a != '>') {
        check = 1;
        cout << "Wrong input form" << endl;
        break;
      }
      i++;
      a = input[i];
      if (a == '#' || a == '.' || a == '-' || a == '>' || a == '(' ||
          a == ')') {
        check = 1;
        cout << "Wrong input form" << endl;
        break;
      }
      int start = i;
      int count = 0;
      int count_or = 0;
      while (a != ',') {
        if (a == '#' || a == '.' || a == '-' || a == '>' || a == '(' ||
            a == ')') {
          check = 1;
          cout << "Wrong input form" << endl;
          break;
        }

        int d = i;
        int v = i;
        if ((a == '|' && input[d + 1] == '|') ||
            (a == '|' && input[v - 1] == '|') ||
            (a == '|' && input[v - 1] == ',') || (input[v - 1] == '.') ||
            (a == '|' && input[v - 1] == '>')) {
          check = 1;
          cout << "Wrong input form" << endl;
          break;
        }
        if (a == '|') {
          count_or++;
          string str;
          if (count_or > 1) {
            str = input.substr(start, count - 1);
          } else
            str = input.substr(start, count);
          rule[RHS].insert(str);
          count = 0;
          cout << str << endl;
          start = i + 1;
        }
        if (a == ',') {
          if (input[d + 1] == ',' || ((a == ',') && (i == g2))) {
            check = 1;
            cout << "Wrong input form" << endl;
            break;
          }
        }
        if (a != '|' || a != ',' || a != '.')
          count++;
        i++;
        a = input[i];
        if (a == '.' && i == g1)
          break;
        if (check == 1)
          break;
      }
      string str;
      if (count_or == 0)
        str = input.substr(start, count);
      else
        str = input.substr(start, count - 1);
      rule[RHS].insert(str);
      cout << str << endl;
    }
  }

  string see1 = "";
  string sub1;
  for (int i = 0; i < input.length(); i++) {
    i += 3;
    while (input[i] != ',' && input[i] != '.') {
      if (input[i] == '|')
        i++;
      while (input[i] != ',' && input[i] != '|' && input[i] != '.') {
        sub1 += input[i];
        i++;
      }
      for (int j = 0; j < sub1.length(); j++) {
        if (!inlist(see1, sub1[j]) && isupper(sub1[j]))
          see1 += sub1[j];
      }
      sub1 = "";
    }
  }

  bool missing = 0;
  int z = see1.length();
  for (int i = 0; i < input.length(); i++) {
    if (inlist(see1, input[i]))
      z--;
    while (input[i] != ',' && input[i] != '.')
      i++;
  }
  if (z > 0) {
    cout << "Oops! you're missing some rules" << endl;
    missing = 1;
  }
  bool x = 0;
  bool n = 0, b = 0;
  string s;
  string a;
  string problem;
  for (int i = 0; i < input.length(); i++) {
    if (n == 1 && b == 1)
      break;
    n = 0, b = 0;
    a += input[i];
    i += 3;
    while (input[i] != ',' && input[i] != '.') {
      if (input[i] == '|')
        i++;
      while (input[i] != '|' && input[i] != ',' && input[i] != '.') {
        s += input[i];
        i++;
      }
      if (equal(s, a)) {
        n = 1;
        problem += s;
      }
      if (equal(s, "$"))
        b = 1;
      if (n == 1 && b == 1)
        break;
      s = "";
      a = "";
    }
  }
  if (n == 1 && b == 1) {
    cout << "Infinite loop for production " << problem << endl;
    x = 1;
  }
  cout << "*-----------------------*" << endl;
  for (auto i : rule) {
    nonTerminal.insert(i.first);
  }
  for (auto i : nonTerminal) {
    for (auto j : rule[i]) {
      for (auto k : j) {
        if (islower(k) && terminal.find(k) == terminal.end())
          terminal.insert(k);
      }
    }
  }
  if (x == 0 && z == 0) {
    if (GNFcheck())
      cout << "The grammar is in GNF\n";
    else {
      cout << "The grammar is not in GNF\nConverting~\n";
      toGNF();
      cout << "To Greibach normal form (GNF):\n";
      for (auto i : rule) {
        cout << i.first << " -> ";
        for (string s : i.second)
          cout << s << ", ";
        cout << endl;
      }
    }
    cout << "*-----------------------*" << endl;
    cout << "To Pushdown Automata (PDA): \n";
    toPDA();
  }
  return 0;
}
