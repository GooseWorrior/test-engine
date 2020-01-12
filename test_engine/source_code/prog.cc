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

void setup(int argc, char * argv[], bool set[3], int & count, string & name, string & target) {
	bool flag = true;
	for (int i = 4; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'r') set[0] = true;
				else if (argv[i][j] == 'w') set[1] = true;
				else if (argv[i][j] == 'n') set[2] = true;
			}
			count++;
		} else if (flag){
			target = argv[i];
			name = argv[i];
			flag = false;
		} else {
			name = argv[i];
		}
	}
	if (!set[0] && !set[1] && !set[2]) set[1] = true;
} 

string transform(string raw) {
	string ret = "root", temp;
	for (auto i : raw) {
		if (i == '/') {
			if (temp != "next") ret += ":" + temp;
			temp.clear();
		} else{
			temp += i;
		}
	}
	return ret;
}

int main(int argc, char * argv[]) {
	int count = 4;
	bool set[3] = {0};
	string name;
	string target;
	setup(argc, argv, set, count, name, target);
	if (set[0] && (set[1] || set[2])) {
		cout<<"options conflict"<<endl;
		return 1;
	}
	if (argc != count + (set[0] || set[1]) + set[2]) {
		cout<<"incorrect number of argument"<<endl;
		return 1;
	}
	string cur_path = argv[1];
	string root = argv[2];
	string prog = argv[3];
	if (set[0]) {
		cout<<target<<endl;
		if (sys("test -r " + cur_path + target)) cout<<"this file doesn't exist"<<endl;
		else sys("less " + cur_path + target);
	} else if (set[2]) {
		if (!sys("test -d " + cur_path + "next/" + name)) {
			cout<<name<<"this version already exist (plz choose another name)"<<endl;
			return 1;
		}
		string nxt = cur_path + "next/" + name;
		string new_version;
		sys("mkdir " + nxt);
		sys("mkdir " + nxt + "/log");
		sys("mkdir " + nxt + "/next");
		sys("cp " + cur_path + "* " + nxt + " 2> /dev/null");
		sys("rm " + nxt + "/active " + nxt + "/suite " + nxt + "/version " + nxt + "/log 2> /dev/null");
		if (set[1]) {
		        if (sys("test -r " + nxt + "/" + target)) cout<<"this file doesn't exist"<<endl;
			else sys("vi " + nxt + "/" + target);
		}
		fstream prev{root + "/object/version"};
		prev >> new_version;
	       	new_version += "next/" + name + "/";
		sys("rm " + root + "/object/version");
		sys("touch " + root + "/object/version");
		fstream next{root + "/object/version"};
		next << new_version << endl;
		string trans_version = transform(new_version);
		cout<<"a new version has been created: "<<trans_version<<endl;
	} else if (set[1]) {
		if (sys("test -r " + cur_path + target)) {
			cout<<"this file doesn't exist"<<endl;
			string cmd;
			cout<<"do you wanna create a new file? (y/n):";
			cin>>cmd;
			if (cmd != "y") return 0;
		}
		sys("vi " + cur_path + target);	
	}
}
