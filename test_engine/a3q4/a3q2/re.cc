#include "re.h"

using std::move;

namespace CS246E {
	///////////// RegExp /////////////
	RegExp::~RegExp() {}
	///////////// Concatenation /////////////
	Concatenation::Concatenation(unique_ptr<RegExp> first, unique_ptr<RegExp> second): RegExp{}, first{move(first)}, second{move(second)} {}
	Concatenation::~Concatenation() {}
	bool Concatenation::matches(string ref) {
		for (size_t i = 0; i <= ref.size(); i++) {
			if (first->matches(ref.substr(0, i)) && second->matches(ref.substr(i))) return true;
		}
		return false;
	}
	///////////// Disjunction /////////////
	Disjunction::Disjunction(unique_ptr<RegExp> first, unique_ptr<RegExp> second): RegExp{}, first{move(first)}, second{move(second)} {}
	Disjunction::~Disjunction() {}
	bool Disjunction::matches(string ref) {
		return (first->matches(ref) || second->matches(ref)); 
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
	///////////// Word ///////////// 
	Word::Word(): RegExp{}, theWord{theWord} {}
	Word::Word(string theWord): RegExp{}, theWord{theWord} {}
	Word::~Word() {}
	bool Word::matches(string ref) {
		return theWord == ref;	
	}
}


