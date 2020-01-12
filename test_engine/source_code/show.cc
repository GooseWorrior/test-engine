#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <cstdio>
#include <iomanip> 

using namespace std;

int sys(string cmd) {
	int ret = system(cmd.c_str());
	return WEXITSTATUS(ret);
}

void print_form(string test, bool set[5], string target) {
	setiosflags(ios::right);
	cout<<setw(15)<<test;
	if (set[0]) {
		if(!sys(string("test -r ") + target + "/tests/" + test + ".args")) cout<<setw(15)<<"true";
		else cout<<setw(15)<<" ";
	}
	if (set[1]) {
		if(!sys(string("test -r ") + target + "/tests/" + test + ".in")) cout<<setw(15)<<"true";
		else cout<<setw(15)<<" ";
	}
	if (set[2]) {
		if(!sys(string("test -r ") + target + "/tests/" + test + ".out")) cout<<setw(15)<<"true";
		else cout<<setw(15)<<" ";
	}
	cout<<endl;
}

void setup(int argc, char * argv[], bool set[5], int & count) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'a') set[0] = true;
				else if (argv[i][j] == 'i') set[1] = true;
				else if (argv[i][j] == 'o') set[2] = true;
				else if (argv[i][j] == 'd') set[3] = true;
				else if (argv[i][j] == 'l') set[4] = true;
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

void detail(bool set[5], string name, string target) {
	if (set[0]) {
		cout<<name + ".args:"<<endl;
		if (!sys(string("test -r ") + target + "/tests/" + name + ".args")) sys(string("cat ") + target + "/tests/" + name + ".args");
		else cout<<"argument file not found"<<endl;
	}
	if (set[1]) {
		cout<<name + ".in:"<<endl;
		if (!sys(string("test -r ") + target + "/tests/" + name + ".in")) sys(string("cat ") + target + "/tests/" + name + ".in");
		else cout<<"input file not found"<<endl;
	}
	if (set[2]) {
	       cout<<name + ".out:"<<endl;	
		if (!sys(string("test -r ") + target + "/tests/" + name + ".out")) sys(string("cat ") + target + "/tests/" + name + ".out");
		else cout<<"output file not found"<<endl;
	}
}

int main (int argc, char * argv[]) {
	int count = 2;
	bool set[5] = {0};
	string target = string("test_env_") + argv[1];
	setup(argc, argv, set, count);
	if (argc > count && !set[3]) {
		cerr<<"unidentified argument (you must use -d option to check details)"<<endl;
		return 1;
	}
	if (set[3]) {
		for (int i = 2; i < argc; i++) {
		   if(argv[i][0] != '-') detail(set, argv[i], target);
		}
		return 0;
	}
	string test, list;
	if (set[4]) list = "active";
	else list = "suite";
	fstream suite{target + "/object/" + list};
	setiosflags(ios::right);
	if (!(suite >> test)) {
		cout<<"no test found"<<endl;
	} else {
		cout<<setw(15)<<" ";
		if (set[0]) cout<<setw(15)<<".args";
		if (set[1]) cout<<setw(15)<<".in";
		if (set[2]) cout<<setw(15)<<".out";
		cout<<endl;
		print_form(test, set, target);
		while (suite >> test) {		
			print_form(test, set, target);
		}
		FILE * f = popen((string("wc -l ") + target + "/object/" + list).c_str(), "r");
		char result[255];
		fscanf(f, "\%s", result);
		pclose(f);
		if (set[4]) cout<<"total number of active test: "<<result<<endl;
		else cout<<"total number of test: "<<result<<endl;
	}
	return 0;
}
