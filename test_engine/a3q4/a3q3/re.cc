#include "re.h"

using std::make_unique;

namespace CS246E {
	bool checkBalance(const string & s) {
		int bal = 0;
		for (auto i : s) {
			if (i == '(') bal++;
			if (i == ')') bal--;
			if (bal < 0) return false;
		}
		if (bal != 0) return false;
		else return true;
	}

	const string unwrapRecursion(const string & s) { 
		if (s.empty()) return s;
		if (s[0] != '(' || s[s.length() - 1] != ')') return s;
		string tmp = s.substr(1, s.length() - 2);
		if (!checkBalance(tmp)) return s;
		else return unwrapRecursion(tmp);
	}

	const string unwrap(const string & s) {
		if (!checkBalance(s)) throw;
		return unwrapRecursion(s);
	}

	bool tryConcatenation(const string & s, size_t & pos) {
		if (s.length() < 2) return false;
		const size_t len = s.length();
		if (s[0] != '(') {
			size_t i = 0;
			while (i + 1 < len && s[i + 1] == '*') i++;
			if (i + 1 == len) return false;
			else {
				pos = i;
				return true;
			}
		} else {
			int bal = 1;
			for (size_t i = 1; i < len; i++) {
				if (s[i] == '(') bal++;
				if (s[i] == ')') bal--;
				if (bal == 0) {
					while (i + 1 < len && s[i + 1] == '*') i++;
					if (i + 1 == len) return false;
					else {
						pos = i;
						return true;
					}
				}
			}
			return false; // double guard
		}	
	}
	bool tryDisjunction(const string & s, size_t & pos) {
		if (s.empty()) return false;
		size_t c = 0;
		int bal = 0;
		for (auto i : s) {
			if (i == '(') bal++;
			if (i == ')') bal--;
			if (i == '|' && bal == 0) {
				pos = c;
				return true;
			} 
			c++;
		}
		return false;
	}
	bool tryStar(const string & s, size_t & pos) {
		if (s.empty()) return false;
		size_t c = s.length() - 1;
		if (s[c] == '*') {
			pos = c;
			return true;
		}
		else return false;
	}
	unique_ptr<RegExp> parseConcatenation(const string & s) {
		size_t pos = 0;
		const string str = unwrap(s);
		if (tryConcatenation(str, pos)) {
			return make_unique<Concatenation> (parseDisjunction(str.substr(0, pos + 1)), parseDisjunction(str.substr(pos + 1)));
		} else {
		 	 return parseStar(str);   
		}
	}
	unique_ptr<RegExp> parseDisjunction(const string & s) {
		size_t pos = 0;
		const string str = unwrap(s);
		if (tryDisjunction(str, pos)) {
			return make_unique<Disjunction> (parseDisjunction(str.substr(0,pos)), parseDisjunction(str.substr(pos + 1)));
		} else {
			return parseConcatenation(s);
		}
	}
	unique_ptr<RegExp> parseStar(const string & s) {
		size_t pos = 0;
		const string str = unwrap(s);
		if (tryStar(str, pos)) return make_unique<Star> (parseDisjunction(str.substr(0, pos)));
		else return parseWord(s);
	}
	unique_ptr<RegExp> parseWord(const string & s) {
		const string str = unwrap(s);
		return make_unique<Word> (str);
	}
	ostream & operator<<(ostream & out, RegExp & re) {
		return re.print(out);
	}
}
