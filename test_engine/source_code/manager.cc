#include <sys/wait.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int sys(string cmd) {
	int ret = system(cmd.c_str());
	return WEXITSTATUS(ret);
}

void prog_update() {
	sys("rm bin/prog_lst");
	sys("touch bin/prog_lst");
	fstream prog_lst{"bin/prog_lst"};
	FILE * list = popen("ls", "r");
	vector<string> los;
	char file[255] = {0};
	while (fscanf(list, "%s", file) == 1) {
		los.push_back(file);
	}
	for (auto i : los) {
	        if (!sys("test -r " + i + "/" + i + ".cc")) {
			prog_lst << i << endl;
		}
	}
        pclose(list);
}

void version_update(string prog, string env, string & version) {
	if (sys("test -d " + prog) || sys("test -d " + env)) {
		version.clear();
	} else {
		fstream f{env + "/object/version"};
		string temp;
		if (!(f >> temp)) version.clear();
		else version = temp;
	}
}

int check_env(string prog, string env) {
	int ret = 0;
	if (sys(string("test -d ") + env)) {
		cout<<"environment error: #"<<ret + 1<<": test environment doesn't exist"<<endl;
		ret++;
	} else {
		if (sys(string("test -d ") + env + "/object")) {
			cout<<"environment error: #"<<ret + 1<<": object directory do not exist"<<endl;
			ret++;
		} else if (sys(string("test -r ") + env + "/object/" + prog + ".cc") ||
			   sys(string("test -r ") + env + "/object/" + prog) ||
			   sys(string("test -r ") + env + "/object/suite") ||
			   sys(string("test -r ") + env + "/object/active")) {
			cout<<"environment error: #"<<ret + 1<<": object damaged"<<endl;
			ret++; 
		}
		if (sys(string("test -r") + env + "/suite")) {
			cout<<"environment error: #"<<ret + 1<<": no suite file or unreadable suite file"<<endl;
			ret++;
		} 
		if (sys(string("test -d ") + env + "/log")) {
			cout<<"environment error: #"<<ret + 1<<": log directory do not exist"<<endl;
			ret++;
		} else if (sys(string("test -r ") + env + "/log/count")) {
			cout<<"environment error：#"<<ret + 1<<": log damaged"<<endl;
			ret++;
		}
	} 
	return ret;
}

int check_sc(string prog) {
	int ret = 0;
	if (sys(string("test -d ") + prog)) {
		ret++;
		cout<<"source error: #"<<ret<<": source program directive not found"<<endl;
	} else if (sys(string("test -r ") + prog + "/" + prog + ".cc") ||
		   sys(string("test -r ") + prog + "/" + prog)) {
		ret++;
		cout<<"source error: #"<<ret<<": source directive damaged"<<endl;
	}
	return ret;
}

int check_sys() {
	return 0;
}

int check_all(string prog, string env) {
	int pred_1 = check_sys();
	int pred_2 = check_sc(prog);
	int pred_3 = check_env(prog, env);
	return pred_1 || pred_2 || pred_3;
}

void set_default(string prog) {
	sys(string("mkdir ") + prog);
	sys(string("cp bin/template/default/default.cc ") + prog + "/" + prog + ".cc");
	sys(string("cp bin/template/default/default ") + prog + "/" + prog);
        cout<<"default has been set"<<endl;
}


int main (int argc, char * argv[]) {
	string cmd_raw, prog, args, env, cmd, version;
	if (argc != 2) return 1;
	prog = string(argv[1]);
	env = string("test_env_") + prog;
	while (1) {
		prog_update();
		version_update(prog, env, version);
		cout << ">>>";
		cmd.clear();
		getline(cin, cmd_raw);
		stringstream input{cmd_raw};
		input>>cmd;
		args = cmd_raw.substr(cmd.length());
		if (cmd == "boot") {
			if (!sys("test -d " + env)) {
				cout<<"test already exist"<<endl;
				continue;
			}
			cout<<"booting ..."<<endl;
			if (sys("test -d " + prog)) set_default(prog);
			if (check_sc(prog)) {
				cout<<"fatal error, boot failed"<<endl;
				continue;
			}
			sys("mkdir " + env);
			sys("mkdir " + env + "/object " + env + "/log " + env + "/tests");
			sys("mkdir " + env + "/object/next");
			sys("mkdir " + env + "/object/log");
			sys(string("cp ") + prog + "/* " + env + "/object");
			sys("touch " + env + "/object/suite");
			sys("touch " + env + "/object/version");
			sys("touch " + prog + "/info"); 
			sys("cp " + env + "/object/suite " + env + "/object/active");
			sys("echo 0 > " + env + "/log/count");
			cout<<"done"<<endl;
		} else if (cmd == "test") {
			if (check_all(prog, env)) continue;
			sys(string("./bin/engine ") + prog + " " + env + "/object/active " + env + "/object/" + version + prog + args);
		} else if (cmd == "add" || cmd == "delete" || cmd == "show" || cmd == "edit" || cmd == "suite" || cmd == "export") {
			if (check_all(prog, env)) continue;
			sys(string("./bin/") + cmd + " " + prog + args);
		} else if (cmd == "import" || cmd == "templt" || cmd == "man") {
			if (check_sys()) continue;
			sys("./bin/" + cmd + " " + prog + args);
		} else if (cmd == "prog" || cmd == "cv" || cmd == "dv" || cmd == "diff" || cmd == "commit" ||
			   cmd == "debug" || cmd == "version") {
		       if (check_all(prog, env)) continue;
		       sys("./bin/" + cmd + " " + env + "/object/" + version +  " " + env + " " + prog + args); 
		} else if (cmd == "cp" || cmd == "mv" || cmd == "rm" || cmd == "cat" || cmd == "ls" ) {
			stringstream args_raw{args};
			string args_token;
			string args_new = " ";
			bool flag = true;
			while (args_raw >> args_token) {
				if (args_token == "log"|| args_token == "suite" || args_token == "version" ||
			            args_token == "active" || args_token == "next" || (args_token.size() >= 2 && args_token.substr(0, 2) == "..")) flag = false;
			        if (args_token[0] != '-') args_token = env + "/object/" + version + args_token;
				args_new += args_token + " ";
			}
			if (args_new.size() == 1 && cmd == "ls") sys(cmd + " " + env + "/object/" + version + " | egrep -v 'log|suite'| egrep -v 'active|version' | egrep -v 'next'");
			else if (flag) sys(cmd + args_new);
			else {
				cout<<"you must work in the working directory."<<endl;
		       		cout<<"some of the arguments indicates that you intends to change internal files."<<endl;
				cout<<"or change some thing outside the working directory."<<endl;
				cout<<"bash command is suppressed."<<endl;
			}
		} else if (cmd == "load") {
			cout<<"you are about to overwrite all the .out files of the active tests? (y/n):";
			cin>>cmd;
			if (cmd != "y") continue; 
			fstream f{env + "/object/active"};
			string test;
			while (f >> test) {
				string arg = " ";
				string tmp;
				if (!sys("test -r " + env + "/tests/" + test + ".args")) {
					fstream argf{env + "/tests/" + test + ".args"};
					while (argf >> tmp) arg += tmp + " ";
				}
				if (sys("test -r " + env + "/tests/" + test + ".in")) {
					cout<<"missing .in files for test: "<<prog<<":"<<test<<endl;
					continue;
				}
				sys(env + "/object/" + version + prog + arg + " < " + env + "/tests/" + test + ".in > " + env + "/tests/" + test + ".out");
			}
		} else if (cmd == "compile") {
			sys("g++ -std=c++14 -Wall " + env + "/object/" + version + prog + ".cc -o" + env + "/object/" + version + prog);// could be improved to support makefile
		} else if (cmd == "delenv") {
			if (sys("test -d " + env)) {
				cout<<"environment not found, delete failed"<<endl;
				continue;	
			}
			cout<<"do you really want to remove test environment for "<<prog<<" permanently? (y/n):";
			cin>>cmd;
			if (cmd == "n") continue;
			else sys(string("rm") + " -rf " + env);
			cout<<"test environment removed"<<endl;
			cout<<"you can use boot to setup a new environment"<<endl;
		} else if (cmd == "delsc") {
			if (sys("test -d " + prog)) {
				cout<<"source directory not found, delete failed"<<endl;
				continue;	
			}
			if (!sys("test -d " + env)) {
				cout<<"environment exist, delete failed"<<endl;
				continue;
			}
			cout<<"do you really want to remove source files for "<<prog<<" permanently? (y/n):";
			cin>>cmd;
			if (cmd == "n") continue;
			else sys(string("rm -rf ") + prog);
			cout<<"source files removed"<<endl;
			cout<<"you can use import or boot to setup a new source derectory"<<endl;
		} else if (cmd == "delall") {
			cout<<"do you want to clear all relative contents to "<<prog<<"? (y/n):";
			cin>>cmd;
			if (cmd == "n") continue;
			else {
				if (!sys("test -d " + env)) sys("rm -rf " + env);
				if (!sys("test -d " + prog)) sys("rm -rf " + prog);
				cout<<"all contents cleared"<<endl;
			}
		} else if (cmd == "alter") {
		       input >> cmd;
	       	       prog = cmd;
	               env = string("test_env_") + prog;
		       cout<<"current program has been changed to:"<<prog<<endl;	       
		} else if (cmd == "proj") {
			fstream prog_lst{"bin/prog_lst"};
			string proj_it;
			int counter = 1;
			cout<<"exist program:\n"<<endl;
			while (prog_lst >> proj_it) {
				cout<<"program #"<<counter<<": "<<proj_it<<" ";
				if (!sys("test -d " + env)) cout<<"(active)"<<endl;
				else cout<<endl;
				counter++;
			}	
		} else if (cmd == "check") {
			if (!check_all(prog, env)) {
				cout<<"check done: no error found"<<endl;
			}
		} else if (cmd == "chkenv") {
		  	if (!check_env(prog, env)) {
				cout<<"test environment check done: no error found"<<endl;
			}
		} else if (cmd == "chksc") {
			if (!check_sc(prog)) {
				cout<<"source check done: no error found"<<endl;
			}	
		} else if (cmd == "chksys") {
			if (!check_sys()) {
				cout<<"system check done: no error found"<<endl;
			}	
		} else if (cmd == "quit") {
			cout<<"goodbye!"<<endl;
			return 1;
		} else if (!cmd.empty()){
			cout<<"invalid command"<<endl;
		}	
	}
}
