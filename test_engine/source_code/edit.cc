#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <iomanip>
#include <cstdio>

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
			}
			count++;
		}
	}
}

bool check(string ref, string target) {
	string test;
	fstream f{target + "/object/suite"};
	while (f >> test) {
		if (ref == test) return 1;
	}
	return 0;
}

void edit(string name, bool set[4], string target, string prog) {
	string cmd; 
	if (!set[0] && !set[1] && !set[2]) {
		if (sys("test -r " + target + "/tests/" + name)) cout<<"this file doesn't exist"<<endl;
		else {
			if (set[3]) {
				cout<<"do you wanna edit "<<prog<<":"<<name<<"? (y/n):";
				cin>>cmd;
				if (cmd != "y") return; 
			}
			sys("vi " + target + "/tests/" + name);
		}
	} else if (!check(name, target)) {
		cout<<"this test doesn't exist:"<<prog<<":"<<name<<endl;
		return;
	}
	if (set[0]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".args")) {
			cout << "this argument file doesn't exist:"<<prog<<":"<<name<<".args"<<endl; 
		} else {	
			if (set[3]) {
				cout<<"do you wanna edit "<<prog<<":"<<name<<".args? (y/n):";
				cin>>cmd;
				if (cmd != "y") return; 
			}
			sys(string("vi ") + target + "/tests/" + name + ".args"); 
		}
	}
	if (set[1]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".in")) {
			cout <<"this input file doesn't exist:"<<prog<<":"<<name<<".in"<<endl; 
		} else {
			if (set[3]) {
				cout<<"do you wanna edit "<<prog<<":"<<name<<".in? (y/n):";
				cin>>cmd;
				if (cmd != "y") return;
			}
			sys(string("vi ") + target + "/tests/" + name + ".in"); 
		}
	}
	if (set[2]) {
		if (sys(string("test -r ") + target + "/tests/" + name + ".out")) {
			cout << "this output file doesn't exist:"<<prog<<":"<<name<<".out"<<endl; 
		} else {		
			if (set[3]) {
				cout<<"do you wanna edit "<<prog<<":"<<name<<".out? (y/n):";
				cin>>cmd;
				if (cmd != "y") return;
			}
			sys(string("vi ") + target + "/tests/" + name + ".out"); 
		}
	}
}

int main (int argc, char * argv[]) {
	int count = 2;
	bool set[4] = {0};
	string target = string("test_env_") + argv[1];
	setup(argc, argv, set, count);
	if (argc == count) {
		cerr<<"lack of argument, you need more argument to specify the name of the test"<<endl;
		return 1;
	}
	if(argc > count + 1) set[3] = true;
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') edit(argv[i], set, target, argv[1]);
	}
	return 0;
}
