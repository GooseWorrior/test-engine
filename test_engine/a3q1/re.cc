#include "re.h"

namespace CS246E {
	///////////// RegExp /////////////
	RegExp::~RegExp() {}
	///////////// Concatenation /////////////
	Concatenation::Concatenation(RegExp * first, RegExp * second): RegExp{}, first{first}, second{second} {}
	Concatenation::~Concatenation() {
		delete first;
		delete second;
	}
	bool Concatenation::matches(string ref) {
		for (size_t i = 0; i <= ref.size(); i++) {
			if (first->matches(ref.substr(0, i)) && second->matches(ref.substr(i))) return true;
		}
		return false;
	}
	///////////// Disjunction /////////////
	Disjunction::Disjunction(RegExp * first, RegExp * second): RegExp{}, first{first}, second{second} {}
	Disjunction::~Disjunction() {
		delete first;
		delete second;
	}
	bool Disjunction::matches(string ref) {
		return (first->matches(ref) || second->matches(ref)); 
	}
	///////////// Star /////////////
	Star::Star(RegExp * target): RegExp{}, target{target} {}
	Star::~Star() {
		delete target;
	}
	bool Star::matches(string ref) {
		if (ref.empty()) return true;
		else {
			for (size_t i = 1; i <= ref.size(); ++i) {
				if (target->matches(ref.substr(0, i)) && this->matches(ref.substr(i))) return true; 
			}	
			return false;
		}
	}
	///////////// Word ///////////// 
	Word::Word(): RegExp{} {}
	Word::Word(string theWord): RegExp{}, theWord{theWord} {}
	Word::~Word() {}
	bool Word::matches(string ref) {
		return theWord == ref;	
	}
}


