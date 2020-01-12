#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// wc(f, set, total, name) is the core of the program, which counts the number of
// words, lines, and characters in a plane text file, and prints the result in order.

void wc(istream & f, bool set[3], int total[4], string name) {
	int w = 0, l = 0, c = 0, g = 0; // g is a flag parameter that marks the words' end
	char a = '0';
	if (!(f >> noskipws >> a)) return;
	c++;
	if (isspace(a)) g = 1; // use string::c_type::isspace(char) to locate ws
	while (f >> a) {
		c++;
		if (a == '\n') l++;
		if (g == 0 && isspace(a)) {
			g = 1;
			w++;
		} else if (g == 1 && !isspace(a)) {
			g = 0;
		}
	}
	if (g == 0) {
		w++; // consider words at the end of the file
	}
	if (set[0]) cout<<l<<" "; 
	if (set[1]) cout<<w<<" ";
	if (set[2]) cout<<c<<" ";
	cout<<name<<endl;
	// keep track of the total number of l, w, c
	total[0] += l;
	total[1] += w;
	total[2] += c;
	total[3] += 1;
}


// setup(argc, argv, set, count) setup the basic setting of the program including
// number of arguments and types of options.

void setup(int argc, char * argv[], bool set[3], int & count) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; j < int(string(argv[i]).length()); j++) {
				if (argv[i][j] == 'c') set[2] = true;
				else if (argv[i][j] == 'l') set[0] = true;
				else if (argv[i][j] == 'w') set[1] = true;
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

int main(int argc, char * argv[]) {
	int total[4] = {0}, count = 1;
	bool set[3] = {0}; 
	setup(argc, argv, set, count); // initialize the program
	if (argc == 1) {		
		wc(cin, set, total, ""); // read from stdin (case one)
	} else {	
		if (argc == count) {
		   wc(cin, set, total, ""); // read from stdin (case two)
		} else {
			for (int i = 1; i < argc; i++) {
				fstream f{argv[i]};
				wc(f, set, total, argv[i]); // read from files
                        }
			if (total[3] > 1) {
				// print the total numbers
				if (set[0]) cout<<total[0]<<" ";
				if (set[1]) cout<<total[1]<<" ";
				if (set[2]) cout<<total[2]<<" ";
				cout<<"total"<<endl;
			}
		}
	}
	return 0;
}
