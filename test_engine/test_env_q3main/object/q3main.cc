#include <iostream>
#include <map>
#include <string>
#include <deque>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using namespace std;

int main () {
  std::map<string, deque<int>> allDeques;
  string cmdString, dequeName;

  while (cin >> cmdString >> dequeName) {
    if (cmdString == "newDefault") {
      allDeques[dequeName];
    }
    if (cmdString == "newInit") {
      deque<int> m {2, 3, 5, 7, 11, 13, 17, 19};
      allDeques[dequeName] = m;
    }
    else if (cmdString == "=") {
      string otherName;
      cin >> otherName;
      allDeques[dequeName] = allDeques[otherName];
    }
    else if (cmdString == "[]=") {
      int index, val;
      cin >> index >> val;
      allDeques[dequeName][index] = val;
    }
    else if (cmdString == "[]") {
      int index;
      cin >> index;
      cout << allDeques[dequeName][index] << endl;
    }
    else if (cmdString == "at") {
      int index;
      cin >> index;
      try {
        cout << allDeques[dequeName].at(index) << endl;
      }
      catch (...) {
        cout << "Exception" << endl;
      }
    }
    else if (cmdString == "empty") {
      cout << allDeques[dequeName].empty() << endl;
    }
    else if (cmdString == "size") {
      cout << allDeques[dequeName].size() << endl;
    }
    else if (cmdString == "push_front") {
      int value;
      cin >> value;
      allDeques[dequeName].push_front(value);
    }
    else if (cmdString == "push_back") {
      int value;
      cin >> value;
      allDeques[dequeName].push_back(value);
    }
    else if (cmdString == "pop_front") {
      allDeques[dequeName].pop_front();
    }
    else if (cmdString == "pop_back") {
      allDeques[dequeName].pop_back();
    }

    for (auto &x: allDeques) {
      cout << x.first << ":";
      for (auto &y: x.second) {
        cout << " " << y;
      }
      cout << endl;
    }
  }
}
