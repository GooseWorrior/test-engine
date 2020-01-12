#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <sys/wait.h>

using namespace std;

int sys(string cmd) {
	int ret = system(cmd.c_str());
	return WEXITSTATUS(ret);
}

void setup(int argc, char * argv[], bool set[4], int & count) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'a') set[0] = true;
				else if (argv[i][j] == 'i') set[1] = true;
				else if (argv[i][j] == 'o') set[2] = true;
				else if (argv[i][j] == 'w') set[3] = true;
				else if (argv[i][j] == 'c') set[4] = true;
			}
			count++;
		}
	}
	if (!set[0] && !set[1] && !set[2]) {
		set[0] = true;
		set[1] = true;
		set[2] = true;
	}
}

bool check(string ref, string target) {
	fstream suite{target + "/object/suite"};
	string test;
	while (suite >> test) {
		if (test == ref) return 1;
	} 
	return 0;
}

void del(string name, bool set[5], string target, string prog) {
	string cmd; 
	if (set[0]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".args")) {
			cout << "argument file do not exist:"<<prog<<":"<<name<<".args"<<endl; 
		} else {
			if (set[3]) sys(string("rm -i ") + target + "/tests/" + name + ".args");
			else sys(string("rm -f ") + target + "/tests/" + name + ".args");
			cout<<"test files deleted: "<<prog<<":"<<name<<".args"<<endl;	
		}
	}
	if (set[1]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".in")) {
			cout << "input file do not exist:"<<prog<<":"<<name<<".in"<<endl; 
		} else {
			if (set[3]) sys(string("rm -i ") + target + "/tests/" + name + ".in");
			else 	sys(string("rm -f ") + target + "/tests/" + name + ".in");
			cout<<"test files deleted:"<<prog<<":"<<name<<".in"<<endl;	
		}
	}
	if (set[2]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".out")) {
			cout << "output file do not exist:"<<prog<<":"<<name<<".out"<<endl; 
		} else {
			if (set[3]) sys(string("rm -i ") + target + "/tests/" + name + ".out");
			else 	sys(string("rm -f ") + target + "/tests/" + name + ".out");
			cout<<"test files deleted:"<<prog<<":"<<name<<".out"<<endl;	
		}
	}
}

void clean(string name, string target, string prog) {
	if (sys(string("test -r ") + target + "/tests/" + name + ".out")) {
		cout<<"fatal error: missing ouput file: "<<prog<<":"<<name<<".out"<<endl;
		if (!sys(string("test -r ") + target + "/tests/" + name + ".args"))
			sys(string("test -r ") + target + "/tests/" + name + ".args");
		if (!sys(string("test -r ") + target + "/tests/" + name + ".in"))
			sys(string("test -r ") + target + "/tests/" + name + ".in");
		sys(string("mv ") + target + "/object/suite " + target + "/object/suite_prev");
		sys(string("touch ") + target + "/object/suite");
		fstream prev{target + "/object/suite_prev"};
		fstream next{target + "/object/suite"};
		string line;
		while (getline(prev, line)) {
			if (line != name) {
				next<<line<<endl;
			}
		}
		sys(string("rm ") + target + "/object/suite_prev");
		cout<<"test:"<<prog<<":"<<name<<" has been completly deleted"<<endl;
	}
}

int main (int argc, char * argv[]) {
	int count = 2;
	bool set[5] = {0};
	string target = string("test_env_") + argv[1];
	setup(argc, argv, set, count);
	if (argc == count) {
		cerr<<"lack of argument, you need more argument to specify the name of the test"<<endl;
		return 1;
	}
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-' && !check(argv[i], target)) {
			cout<< "delete failed, relative test files do not exist:"<<argv[1]<<":"<<argv[i]<<endl;
			continue;
		}
		if (argv[i][0] != '-') {
			del(argv[i], set, target, argv[1]);
			if (set[4]) clean(argv[i], target, argv[1]);
		}
	}
	return 0;
}
