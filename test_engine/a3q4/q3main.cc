#include <iostream>
#include <string>
#include "re.h"
using namespace std;

int main() {
  string s;
  while (getline(cin, s)) {
    auto re = CS246E::parseDisjunction(s);
    cout << *re << endl;
    while (getline(cin, s)) {
      if (s == "q") break;
      cout << boolalpha << re->matches(s) << endl;
    }
  }
}
