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
	return 1;
}

void setup(int argc, char * argv[], bool set[4], int & count, string & path) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'd') set[0] = true;
				else if (argv[i][j] == 'c') set[1] = true;
				else if (argv[i][j] == 'm') set[2] = true;
				else if (argv[i][j] == 't') set[3] = true;
			}
			count++;
		} else {
			path = string(argv[i]);
		}
	}
} 

bool check(string ref, string target) {
	string test;
	fstream f{target + "/object/suite"};
	while (f >> test) {
		if (test == ref) return true;
	}
	return false;
}

void add_test(string test, string env, string path) {
	bool set[3] = {0};
	if (sys("test -r " + path + "/" + test + ".out")) set[0] = true;
	if (sys("test -r " + path + "/" + test + ".in")) set[1] = true;
	if (sys("test -r " + path + "/" + test + ".args")) set[2] = true;
	if (set[0]) {
	      cout<<".out file not found, test:"<<test<<" is invalid"<<endl;
	      return;
	}
	fstream f;
	f.open(env + "/object/suite", ios::app);
	f << test << endl;
	f.close();
	if (!set[0]) sys("cp " + path + "/" + test + ".out " + env + "/tests/" + test + ".out");
	if (!set[1]) sys("cp " + path + "/" + test + ".in " + env + "/tests/" + test + ".in");
	if (!set[2]) sys("cp " + path + "/" + test + ".args " + env + "/tests/" + test + ".args");
}

void import(string test, string env, string path, string prog) {
	if (check(test, env)) {
		cout<<"this test is already exist:"<<prog<<":"<<test<<endl;
	} else {
		add_test(test, env, path);
	}
}

int main(int argc, char * argv[]) {
	int count = 2;
	bool set[4] = {0};
	string path, name, env;
	setup(argc, argv, set, count, path);
	name = argv[1];
	env = "test_env_" + name;
	if (!set[0] && !set[1] && !set[2] && !set[3]) {
		cout<<"no argument specified"<<endl;
		return 1;
	}
	if (count + 1 != argc) {
		cout<<"incorrect number of argument"<<endl;
		return 1;
	} 
	if (set[0] && set[1]) {
		cout<<"option conflict"<<endl;
		return 1;
	}
	if (!sys("test -d " + name) && !set[3]) {
		cout<<"already has a source code directory, import failed"<<endl;
		return 1;
	}
	if (set[2]) {
		if (sys(string("test -d bin/template/") + path)) {
			cout<<"template not found"<<endl;
			return 1;
		} else {
			sys(string("cp -r bin/template/") + path + " " + name);
			sys(string("mv " + name + "/" + path + " " + name + "/" + name));
			sys(string("mv " + name + "/" + path + ".cc " + name + "/" + name + ".cc"));
		}
	} else if (set[0]){
		if (sys(string("test -d ") + path)) {
			cout<<"specified directory not found (or not a directory)"<<endl;
			return 1;
		} else {
			sys(string("cp -r ") + path + " " + name);
		}
	} else if (set[1]) {
		if (sys(string("test -r ") + path)) {
			cout<<"specified file not found or unreadable"<<endl;
			return 1;
		} else {
			sys("mkdir " + name);
			sys("cp " + path + " " + name + "/" + name + ".cc");
			sys("g++ -std=c++14 -Wall " + name + "/" + name + ".cc -o " + name + "/" + name);
		}
	} else if (set[3]) {
		if (sys("test -d " + name)) {
			cout<<"no source directory"<<endl;
			return 1;
		} else if (sys("test -d " + env)) {
			cout<<"no test environmen"<<endl;
			return 1;
		}
		if (sys("test -r " + path + "/suite.txt")) {
			cout<<"suite.txt file not found or unreadable (you need suite.txt in source directory)"<<endl;
			return 1;
		}
		fstream file{path + "/suite.txt"};
		string test;
		while(file >> test) {
			import(test, env, path, name);
		}
	}
}
