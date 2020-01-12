#include <iostream>
#include <string>
#include "re.h"
using namespace std;

int main() {
  CS246E::RegExp *re = new CS246E::Concatenation{new CS246E::Star{new CS246E::Disjunction{new CS246E::Word{"cat"}, new CS246E::Word {"dog"}}}, new CS246E::Word{"bird"}};

  string s;
  while (getline(cin, s)) {
    cout << boolalpha << re->matches(s) << endl;
  }

  delete re;
}
