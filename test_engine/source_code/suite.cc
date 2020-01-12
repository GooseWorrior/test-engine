#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <cstdio>

using namespace std;

int sys(string cmd) {
	int ret = system(cmd.c_str());
	return WEXITSTATUS(ret);
}

bool check(string ref, string target) {
	fstream suite{target + "/object/suite"};
	string test;
	while (suite >> test) {
		if (test == ref) return 1;
	} 
	return 0;
}

void setup(int argc, char * argv[], bool set[2], int & count) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'a') set[0] = true;
				else if (argv[i][j] == 'n') set[1] = true;
			}
			count++;
		} 
	}
} 

int main(int argc, char * argv[]) {
	int count = 2;
	bool set[2] = {0};
	string target = string("test_env_") + argv[1];
	setup(argc, argv, set, count);
	if ((set[0] || set[1]) && argc > count) {
		cout<<"unidentified argument"<<endl;
		return 1;
	}
	if (set[0] && set[1]) {
		cout<<"conflict options"<<endl;
		return 1;
	}
	sys("rm " + target + "/object/active");
	sys("touch " + target + "/object/active");
	if (set[1]) return 0;
	else if (set[0]) {
		sys("cp " + target + "/object/suite " + target + "/object/active");
	} else {
		fstream active{target + "/object/active"};
		for (int i = 2; i < argc; i++) {
			if (argv[i][0] != '-' && check(argv[i], target)) {
				active << argv[i] << endl;
			}
		}
	}
}
