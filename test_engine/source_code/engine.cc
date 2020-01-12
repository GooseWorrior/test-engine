#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/wait.h>

using namespace std;

// sys(arg) is a system call wrapper function

int sys(string arg) {
	int result = system(arg.c_str());
	return WEXITSTATUS(result);
}



void setup(int argc, char * argv[], bool set[3], int & count) {
	for (int i = 4; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'r') set[0] = true;
				else if (argv[i][j] == 'c') set[1] = true;
				else if (argv[i][j] == 'w') set[2] = true;
			}
			count++;
		}
	}
}

void cat(string file, string content) {
	fstream log;
	log.open(file, ios::app);
	log<<content<<endl;
	log.close();
}

void record(string options, string target, string file, string temp, string prog, bool set[3]) {
	long long a = 0;
	string name =  file + "_";
	sys(string("mv ") + target + "/log/count " + target + "/log/count_prev");
	sys(string("touch ") + target + "/log/count");
	fstream prev{target + "/log/count_prev"};
	fstream next{target + "/log/count"};
	prev>>a;
	a++;
	name += to_string(a) + ".log";
	next<<to_string(a)<<endl;
	sys(string("rm ") + target + "/log/count_prev");
	sys(string("touch ") + target + "/log/" + name);
	string log = target + "/log/" + name;
	cat(log, string("test #") + to_string(a) + ":");
	cat(log, "time:");
	sys(string("date >> ") + log);
	cat(log, "details:\n");
	cat(log, "Args:");
	if (!sys(string("test -r ") + target + "/tests/" + file + ".args")) sys(string("cat ") + target + "/tests/" + file + ".args >> " + log);
	cat(log, "Input:");
	if (!sys(string("test -r ") + target + "/tests/" + file + ".in")) sys(string("cat ") + target + "/tests/" + file + ".in >> " + log);
	if (set[2]) {
	      if (!sys("diff " + options + target + "/tests/" + file + ".out " + temp + " > " + "/dev/null")) {
		   cat(log, string("test passed: ") + file);
	      } else {
		cat(log, string("test failed: ") + file);
		sys("diff " + options + target + "/test/" + file + ".out " + temp + " >> " + log);
	      }
	} else if (!sys("diff " + options + target + "/tests/" + file + ".out " + temp + " > " + "/dev/null")) {
		cat(log, string("test failed: ") + file);
		cat(log, "Expected:");
		sys(string("cat ") + target + "/tests/" + file + ".out >> " + log);
		cat(log, "Actual:");
		sys(string("cat ") + temp + " >> " + log); 
	} else {
		cat(log, string("test passed:") + file);
	}
}
// test(file, prog) do the test on a specific set of files

void test(string file, string prog, string target, bool set[3]) {
	string arg_str, in_str, options;
	if (!sys(string("test ") + "-r " + target + "/tests/" + file + ".args"))  {
		fstream arg{target + "/tests/" + file + ".args"};
		string config;
		while (arg >> config) arg_str += " " + config;
	} 
	if (!sys(string("test ") + "-r " + target + "/tests/" + file + ".in")) {
		in_str += string(" < ") + target + "/tests/" + file + ".in";
	}
	FILE * f = popen("mktemp", "r");
	char temp[255];
	fscanf(f,"\%s", temp);
	fclose(f);
	sys(prog + arg_str + in_str + " > " + temp);
	if (set[2]) options += "-b ";
	if (set[1]) {
		options += "-y ";
		if (!sys("diff " + options + target + "/tests/" + file + ".out " + temp + " > " + "/dev/null")) {
			cout<<"test passed: "<<file<<endl;
		} else {
		    cout<<"test failed: "<<file<<endl;
		    sys("diff " + options + target + "/tests/" + file + ".out " + temp);
		}
	} else if (sys(string("diff ") + options + target + "/tests/" + file + ".out " + temp + " > " + "/dev/null")) {
		cout<<string("Test failed: ") + file<<endl;
		cout<<"Args:"<<endl;
		if (!arg_str.empty()) sys(string("cat ") + target + "/tests/" + file + ".args");
		cout<<"Input:"<<endl;
		if (!in_str.empty()) sys(string("cat ") + target + "/tests/" + file + ".in");
		cout<<"Expected:"<<endl;
		sys(string("cat ") + target + "/tests/" + file + ".out");
		cout<<"Actual:"<<endl;
		sys(string("cat ") + temp);
	} else {
		cout<<"test passed: "<<file<<endl;
	}
	if (set[0]) record(options, target, file, temp, prog, set);
	sys(string("rm ") + temp);	
}

int main(int argc, char * argv[]) {
	int count = 4;
	bool set[3] = {0};
	setup(argc, argv, set, count);
	if (argc != count) {
		// error case one
		cerr<<"incorrect number of command line arguments"<<endl;
		return 1;
	}
	string target = string("test_env_") + argv[1];
	fstream suite{argv[2]};
	string file;
	while (suite >> file) {
		if (sys(string("test ") + "-r " + target + "/tests/" + file + ".out")) {
	 	   	// error case two
			cerr<<"missing or unreadable .out files"<<endl;
	    		return 1;
		} else {
			test(file, argv[3], target, set);
		}
		file.clear();
	} 
	return 0;
}

