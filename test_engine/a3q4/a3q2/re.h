#ifndef RE_H
#define RE_H
#include <ostream>
#include <cstddef>
#include <memory>
#include <string>

using std::unique_ptr;
using std::string;

namespace CS246E {
	class RegExp {
		public:
			virtual ~RegExp();
			virtual bool matches(string ref) = 0;
	};
	
	class Concatenation : public RegExp {
		unique_ptr<RegExp> first;
		unique_ptr<RegExp> second;
		public:
			Concatenation(unique_ptr<RegExp> first, unique_ptr<RegExp> second);
			~Concatenation();
			bool matches(string ref) override;
	};
	
	class Disjunction : public RegExp {
		unique_ptr<RegExp> first;
		unique_ptr<RegExp> second;
	    public:
	    	Disjunction(unique_ptr<RegExp> first, unique_ptr<RegExp> second);
	    	~Disjunction();
	    	bool matches(string ref) override;
	};
	
	class Star : public RegExp {
		unique_ptr<RegExp> target;
		public:
			Star(unique_ptr<RegExp> target);
			~Star();
			bool matches(string ref) override;
	};
	
	class Word : public RegExp {
		string theWord;
		public:
			Word();
			Word(string theWord);
			~Word();	  
			bool matches(string ref) override;
	};
}

#endif


