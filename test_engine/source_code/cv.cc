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

string decifer(string raw) {
	string ret, temp;
	int count = 1;
	for (auto i : raw) {
		if (i == ':') {
			if (count > 1) {
				ret += "next/" + temp + "/";
			}
			count++;
			temp.clear();
		} else {
			temp += i;
		}
	}
	if (count > 1) {
		ret += "next/" + temp + "/";
	}
	return ret;
}

string pick_name(string raw) {
	string temp, temp_2;
	for (auto i : raw) {
		if (i == '/') {
			temp_2 = temp;
			temp.clear();
		} else{
			temp += i;
		}
	}
	return temp_2;
}

string pick_upper(string raw) {
	int cutoff = 0, cutoff_prev = 0;
	for (int i = 0; i < int(raw.length()); i++) {
		if (raw[i] == '/') {
			cutoff_prev = cutoff;
			cutoff = i;
		}
	}
	return raw.substr(0, cutoff_prev + 1);
}

string pick_pp(string raw) {
	int cf = 0, cf_p = 0, cf_pp = 0, cf_ppp = 0; 
	for (int i = 0; i < int(raw.length()); i++) {
		if (raw[i] == '/') {
			cf_ppp = cf_pp;
			cf_pp = cf_p;
			cf_p = cf;
			cf = i;
		}
	}
	if (cf_ppp == 0) return "";
	else return raw.substr(0, cf_pp + 1);
}

bool cv(string & cur_version, string & cur_path, string root, string target) {
	if (target == ".") {
		return 0; 
	} else if (target == "..") {
		if (transform(cur_version) == "root") {
			cout<<"this version has no mother version (root)";
			return 1;
		} 
		cur_version = pick_pp(cur_version);
		cur_path = pick_pp(cur_path); 
	} else {
		if (sys("test -d " + cur_path + "next/" + target)) {
			return 1;
		} else {
			cur_version = cur_version + "next/" + target + "/";
			cur_path = cur_path + "next/" + target + "/";
		}
	}
	return 0;
}

int main(int argc, char * argv[]) {
	string cur_version;
	if (argc != 5) {
		cout<<"incorrect number of argument"<<endl;
		return 0;
	}
	string cur_path = argv[1];
	string root = argv[2];
	string prog = argv[3];
	string target = argv[4];
	fstream rt_file{root + "/object/version"};
	rt_file >> cur_version;
	string temp;
	for (auto i : target) {
		if (i == '/') {
			if (temp != "next") {
				if(cv(cur_version, cur_path, root, temp)) {
					cout<<"this path is not valid"<<endl;
					return 1;
				}	
			} 
			temp.clear();
		} else{
			temp += i;
		}
	}
	if (cv(cur_version, cur_path, root, temp)) {
		cout<<"this path is not valid"<<endl;
		return 1;
	}
	sys("rm " + root + "/object/version");
	sys("touch " + root + "/object/version");
	fstream next{root + "/object/version"};
	next << cur_version << endl;
	return 0;
}
