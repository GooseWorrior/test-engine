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

void setup(int argc, char * argv[], bool set[6], int & count, string & target) {
	for (int i = 4; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 's') set[0] = true;
				else if (argv[i][j] == 'b') set[1] = true;
				else if (argv[i][j] == 'c') set[2] = true;
				else if (argv[i][j] == 'm') set[3] = true;
				else if (argv[i][j] == 'p') set[4] = true;
				else if (argv[i][j] == 'r') set[5] = true;
			}
			count++;
		} else {
			target = argv[i];
		}
	}
	if (!set[1] && !set[2] && !set[3] && !set[4] && !set[5]) set[0] = true;
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

void print_children(string directory) {
	FILE * f = popen(("ls " + directory).c_str(), "r");
	int counter = 0; 
	char str[255];
	while (fscanf(f, "%s", str) == 1) {
		cout<<"#"<<++counter<<": "<<str<<endl;
	}
	pclose(f);
}


int main(int argc, char * argv[]) {
	int count = 4;
	bool set[6] = {0};
	string cur_version, target;
	setup(argc, argv, set, count, target);
	if ((set[0] && set[1]) || (set[1] && set[2]) || (set[2] && set[3]) || (set[3] && set[4]) || (set[4] && set[5]) || (set[5] && set[0])) {
		cout<<"options conflict"<<endl;
		return 1;
	}
	if ((set[4] || set[5]) && argc != count + 1) {
		cout<<"incorrect number of argument"<<endl;
		return 1;
	}
	string cur_path = argv[1];
	string root = argv[2];
	string prog = argv[3];
	fstream rt_file{root + "/object/version"};
	rt_file >> cur_version;
	if (set[0]) {
		cout<<"the current version is: ";
		cout<< transform(cur_version)<<endl;
		cout<<"the version name is: ";
		if (transform(cur_version) == "root") cout<<"root"<<endl; 
		else cout<< pick_name(cur_version)<<endl;
	} else if (set[1]) {
		if (transform(cur_version) == "root") {
			cout<<"this version has no brother version (root)";
			return 0;
		} else {
			string upper = pick_upper(cur_path);
			print_children(upper); 
		}
	} else if (set[2]) {
		string c_path = cur_path + "next/";
		print_children(c_path);
	} else if (set[3]) {
		if (transform(cur_version) == "root") {
			cout<<"this version has no mother version (root)";
			return 0;
		} 
		string m_version = pick_pp(cur_version);
		cout<<"the mother version is: ";
		cout<< transform(m_version)<<endl;
		cout<<"the version name is: ";
		if (transform(m_version) == "root") cout<<"root"<<endl; 
		else cout<< pick_name(m_version)<<endl;
	} else if (set[4]) {
		string relative = decifer(target);
		string path = root + "/object/" + relative;
		sys("less " + path + prog + ".cc");
	} else if (set[5]) {
		if (transform(cur_version) == "root") {
			cout<<"cannot rename root version"<<endl;
			return 0;
		} else {
			string upper_path = pick_upper(cur_path);
			string upper = pick_upper(cur_version);
			string cur_d = cur_path.substr(0, int(cur_path.length() - 1));
			sys("mv " + cur_d + " " + upper_path + target);
			sys("rm " + root + "/object/version");
			sys("touch " + root + "/object/version");
			fstream next{root + "/object/version"};
			next << upper + target + "/"<< endl;
		}
	}
	return 0; 
}
