#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "re.h"

using namespace std;
using CS246E::parseDisjunction;
using CS246E::containsMatch;

void matchfile(string s, istream & in, int argc, string name) {
	string tmp;
	auto re = parseDisjunction(s);
	while (getline(in, tmp)) {
		if (containsMatch(re.get(), tmp)) {
			if (argc > 3) cout<<name<<":";
			cout<<tmp<<endl;
		}
	}
}


int main(int argc, char * argv[]) {
	if (argc < 2) throw;
	if (argc == 2) {
		matchfile(argv[1], cin, argc, "");
	}	
	for (int i = 2; i < argc; ++i)	{
		fstream file{argv[i]};
		matchfile(argv[1],file, argc, argv[i]);
	}
	return 0;
}
