#include <iostream>
#include <string>
#include <map>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

int main () {
  using std::map;
  map<string, map<int, int>> allMaps;
  string cmdString, mapName;

  while (cin >> cmdString >> mapName) {
    if (cmdString == "newDefault") {
      allMaps[mapName];
    }
    if (cmdString == "newInit") {
       map<int, int> m {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
      allMaps[mapName] = m;
    }
    else if (cmdString == "=") {
      string otherName;
      cin >> otherName;
      allMaps[mapName] = allMaps[otherName];
    }
    else if (cmdString == "[]=") {
      int key, val;
      cin >> key >> val;
      allMaps[mapName][key] = val;
    }
    else if (cmdString == "[]") {
      int key;
      cin >> key;
      cout << allMaps[mapName][key] << endl;
    }
    else if (cmdString == "at") {
      int key;
      cin >> key;
      try {
        cout << allMaps[mapName].at(key) << endl;
      }
      catch (...) {
        cout << "Exception" << endl;
      }
    }
    else if (cmdString == "empty") {
      cout << allMaps[mapName].empty() << endl;
    }
    else if (cmdString == "size") {
      cout << allMaps[mapName].size() << endl;
    }
    else if (cmdString == "erase") {
      int key;
      cin >> key;
      allMaps[mapName].erase(key);
    }
    else if (cmdString == "clear") {
      allMaps[mapName].clear();
    }
    else if (cmdString == "count") {
      int key;
      cin >> key;
      cout << allMaps[mapName].count(key) << endl;
    }

    for (auto &x: allMaps) {
      cout << x.first << ":";
      for (auto &y: x.second) {
        cout << " (" << y.first << "," << y.second << ")";
      }
      cout << endl;
    }
  }
}
