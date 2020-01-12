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

void setup(int argc, char * argv[], bool set[3], int & count) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 0; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 's') set[0] = true;
				else if (argv[i][j] == 'a') set[1] = true;
				else if (argv[i][j] == 'd') set[2] = true;
			}
			count++;
		} 
	}
} 

void set_tpl(int argc, char * argv[], string & tpl) {
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			tpl = argv[i];
		}
	}	
}

void add_tpl(string name, string tpl) {
	if (!sys("test -d bin/template/" + tpl)) {
		cout<<"this template name has already been used, try another one (or use -s [empty] to show all template names)"<<endl;
		return;
	} else {
		sys("cp -r " + name + " bin/template/" + tpl);
		fstream list;
		list.open("bin/template/tpl_lst", ios::app);
		list << tpl << endl;
	}
}

void del_tpl(string tpl) {
	if (sys("test -d bin/template/" + tpl)) {
		cout<<"this template does not exist, delete failed"<<endl;
		return;
	} else {
		sys("mv bin/template/tpl_lst bin/template/tpl_lst_prev");
		sys("touch bin/template/tpl_lst");
		fstream prev{"bin/template/tpl_lst_prev"};
		fstream next{"bin/template/tpl_lst"};
		string file;
		while (prev >> file) {
			if (file != tpl) {
				next << file << endl;
			}
		}
		sys("rm bin/template/tpl_lst_prev");
		sys("rm -r bin/template/" + tpl);
	}
}

void print_template(int argc, char * argv[], int count) {
	if (count == argc) {
		fstream list{"bin/template/tpl_lst"};
		int counter = 1;
		string file;
		while (list >> file) {
			cout<<"template #"<<counter<<": "<<file<<endl;
		}
	} else {
		for (int i = 2; i < argc; i++) {
			if (argv[i][0] != '-') {
				if (sys("test -d bin/template/" + string(argv[i]))) {
					cout<<"this template not found: "<<argv[i]<<endl;
				} else {
					if (sys(string("test -r bin/template/") + argv[i] + "/" + argv[i] + ".cc")) {
						cout<<"template damaged: "<<argv[i]<<endl;
					} else {
						cout<<"template: "<<argv[i]<<".cc:"<<endl;
						sys(string("cat bin/template/") + argv[i] + "/" + argv[i] + ".cc");
					}
				}
			}
		}
	}
}

int main(int argc, char * argv[]) {
	int count = 2;
	bool set[3] = {0};
	string tpl, name;
	setup(argc, argv, set, count);
	name = argv[1];
	if ((set[1] || set[2]) && count + 1 != argc) {
		cout<<"incorrect number of argument"<<endl;
		return 1;
	}
	if ((set[1] && set[2]) || (set[0] && (set[1] || set[2]))) {
		cout<<"option conflict"<<endl;
		return 1;
	}
	if (set[0]) {
		print_template(argc, argv, count);
	} else if (set[1]) {
		set_tpl(argc, argv, tpl);
		add_tpl(name, tpl);
	} else if (set[2]) {
		set_tpl(argc, argv, tpl);
		del_tpl(tpl);
	}
}
