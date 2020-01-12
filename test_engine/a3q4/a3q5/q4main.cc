#include <iostream>
#include <string>
#include "re.h"
using namespace std;

/*bool containsMatch(CS246E::RegExp *re, const string &s) {
  string s2 = '\200' + s + '\201';
  // then proceed using s2
  return false;
}*/

int main() {
  auto re = make_unique<CS246E::Concatenation> (
    make_unique<CS246E::Star>(
      make_unique<CS246E::Disjunction>(
        make_unique<CS246E::Word>("cat"),
        make_unique<CS246E::Word>("dog")
      )
    ),
    make_unique<CS246E::Word>("bird")
  );

  string s;
  while (getline(cin, s)) {
    if (s == "q") break;
    cout << boolalpha << containsMatch(re.get(), s) << endl;
  }
}
