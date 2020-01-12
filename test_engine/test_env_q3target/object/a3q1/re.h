#ifndef RE_H
#define RE_H
#include <ostream>
#include <cstddef>
#include <string>

using std::string;

namespace CS246E {
	class RegExp {
		public:
			virtual ~RegExp();
			virtual bool matches(string ref) = 0;
	};
	
	class Concatenation : public RegExp {
		RegExp * first;
		RegExp * second;
	        public:
			Concatenation(RegExp * first, RegExp * second);
			~Concatenation();
			bool matches(string ref) override;
	};
	
	class Disjunction : public RegExp {
		RegExp * first;
		RegExp * second;
	        public:
	    		Disjunction(RegExp * first, RegExp * second);
	  	  	~Disjunction();
		    	bool matches(string ref) override;
	};
	
	class Star : public RegExp {
		RegExp * target;
		public:
			Star(RegExp * target);
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


