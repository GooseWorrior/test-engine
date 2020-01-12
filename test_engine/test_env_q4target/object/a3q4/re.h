#ifndef RE_H
#define RE_H

#include <iostream>
#include <cstddef>
#include <memory>
#include <string>
#include "core.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace CS246E {
	unique_ptr<RegExp> parseDisjunction(const string &s);
	unique_ptr<RegExp> parseConcatenation(const string &s);
	unique_ptr<RegExp> parseStar(const string &s);
	unique_ptr<RegExp> parseWord(const string &s);
	ostream & operator<<(ostream & out, RegExp & re);	
	bool containsMatch(RegExp * re, const string & s);	
}

#endif
