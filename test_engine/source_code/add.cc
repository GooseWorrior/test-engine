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
				else if (argv[i][j] == 'w') set[3] = true;
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
	string test;
	fstream f{target + "/object/suite"};
	while (f >> test) {
		if (ref == test) return 1;
	}
	return 0;
}

void create(string name, bool set[4], string target, string prog) {
	string cmd; 
	if (set[0]) {
		if (!sys(string("test -r ") + target + "/tests/" + name + ".args")) {
			cout << "argument file already exist:"<<prog<<":"<<name<<".args"<<endl; 
		} else {
			sys(string("touch ") + target + "/tests/" + name + ".args");
			cout<<"argument files created:"<<prog<<":"<<name<<".args"<<endl;	
			if (set[3]) {
				cout<<"do you wanna pre-edit "<<prog<<":"<<name<<".args? (y/n):";
				cin>>cmd;
				if (cmd == "y") sys(string("vi ") + target + "/tests/" + name + ".args"); 
			}
		}
	}
	if (set[1]) {
		if (!sys(string("test -r ") + target + "/tests/" + name + ".in")) {
			cout << "input file already exist:"<<prog<<":"<<name<<".in"<<endl; 
		} else {
			sys(string("touch ") + target + "/tests/" + name + ".in");
			cout<<"input file created:"<<prog<<":"<<name<<".in"<<endl;	
			if (set[3]) {
				cout<<"do you wanna pre-edit "<<prog<<":"<<name<<".in? (y/n):";
				cin>>cmd;
				if (cmd == "y") sys(string("vi ") + target + "/tests/" + name + ".in"); 
			}
		}
	}
	if (set[2]) {
		if (!sys(string("test -r ") + target + "/tests/" + name + ".out")) {
			cout << "output file already exist:"<<prog<<":"<<name<<".out"<<endl; 
		} else {
			sys(string("touch ") + target + "/tests/" + name + ".out");
			cout<<"output files created:"<<prog<<":"<<name<<".out"<<endl;		
			if (set[3]) {
				cout<<"do you wanna pre-edit "<<prog<<":"<<name<<".out? (y/n):";
				cin>>cmd;
				if (cmd == "y") sys(string("vi ") + target + "/tests/" + name + ".out"); 
			}
		}
	}
	if (check(name, target)) return;
	fstream f;
	f.open(target + "/object/suite", ios::app);
	f<<name<<endl;
	f.close();
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
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') create(argv[i], set, target, argv[1]);
	}
	return 0;
}
