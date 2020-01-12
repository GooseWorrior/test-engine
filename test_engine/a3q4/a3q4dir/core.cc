#include "core.h"

using std::move;

namespace CS246E {
	///////////// RegExp /////////////
	RegExp::~RegExp() {}
	ostream & RegExp::print(ostream & out) { return out; }
	///////////// Concatenation /////////////
	Concatenation::Concatenation(unique_ptr<RegExp> first, unique_ptr<RegExp> second): RegExp{}, first{move(first)}, second{move(second)} {}
	Concatenation::~Concatenation() {}
	bool Concatenation::matches(string ref) {
		for (size_t i = 0; i <= ref.size(); i++) {
			if (first->matches(ref.substr(0, i)) && second->matches(ref.substr(i))) return true;
		}
		return false;
	}
	ostream & Concatenation::print(ostream & out) {
		out<<"Concatenation (";
		first->print(out);
		out<<",";
		second->print(out);
		out<<")";
		return out;
	}	
	///////////// Disjunction /////////////
	Disjunction::Disjunction(unique_ptr<RegExp> first, unique_ptr<RegExp> second): RegExp{}, first{move(first)}, second{move(second)} {}
	Disjunction::~Disjunction() {}
	bool Disjunction::matches(string ref) {
		return (first->matches(ref) || second->matches(ref)); 
	}
	ostream & Disjunction::print(ostream & out) {
		out<<"Disjunction (";
		first->print(out);
	        out<<",";
		second->print(out);
		out<<")";
		return out;	
	}
	///////////// Star /////////////
	Star::Star(unique_ptr<RegExp> target): RegExp{}, target{move(target)} {}
	Star::~Star() {}
	bool Star::matches(string ref) {
		if (ref.empty()) return true;
		else {
			for (size_t i = 1; i <= ref.size(); ++i) {
				if (target->matches(ref.substr(0, i)) && this->matches(ref.substr(i))) return true; 
			}	
			return false;
		}
	}
	ostream & Star::print(ostream & out) {
		out<<"Star (";
		target->print(out);
		out<<")";
		return out;
	}
	///////////// Word ///////////// 
	Word::Word(string theWord): RegExp{}, theWord{theWord} {}
	Word::~Word() {}
	bool Word::matches(string ref) {
		return theWord == ref;	
	}
	ostream & Word::print(ostream & out) {
		out<<theWord;
		return out;
	}

	//////////// Begin /////////////
	Begin::~Begin() {}
	bool Begin::matches(string ref) {
		return ref == string(1, '\200');
	}
	ostream & Begin::print(ostream & out) {
		return out<<"Begin ()";
	}	

	///////////  End  /////////////
	End::~End() {}
	bool End::matches(string ref) {
		return ref == string(1, '\201');
	}
	ostream & End::print(ostream & out) {
		return out<<"End ()";
	}

	/////////// Dot  /////////////
	Dot::~Dot() {}
	bool Dot::matches(string ref) {
		return ref.length() == 1;
	}
	ostream & Dot::print(ostream & out) {
		return out<<"Dot ()";
	}
}


