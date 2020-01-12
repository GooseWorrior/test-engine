#include <iostream>
#include <string>
#include "re.h"
using namespace std;

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
    cout << boolalpha << re->matches(s) << endl;
  }
}
