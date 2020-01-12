#ifndef CORE_H
#define CORE_H
#include <ostream>
#include <cstddef>
#include <memory>
#include <string>

using std::unique_ptr;
using std::string;
using std::ostream;

namespace CS246E {
	class RegExp {
		public:
			virtual ~RegExp();
			virtual bool matches(string ref) = 0;
			virtual ostream & print(ostream & out) = 0; // no need to consider OCP here
	};
	
	class Concatenation : public RegExp {
		unique_ptr<RegExp> first;
		unique_ptr<RegExp> second;
		public:
			Concatenation(unique_ptr<RegExp> first, unique_ptr<RegExp> second);
			~Concatenation();
			bool matches(string ref) override;
			ostream & print(ostream & out) override;
	};
	
	class Disjunction : public RegExp {
		unique_ptr<RegExp> first;
		unique_ptr<RegExp> second;
	    	public:
	    		Disjunction(unique_ptr<RegExp> first, unique_ptr<RegExp> second);
	    		~Disjunction();
		    	bool matches(string ref) override;
			ostream & print(ostream & out) override;
	};
	
	class Star : public RegExp {
		unique_ptr<RegExp> target;
		public:
			Star(unique_ptr<RegExp> target);
			~Star();
			bool matches(string ref) override;
			ostream & print(ostream & out) override;
	};
	
	class Word : public RegExp {
		string theWord;
		public:
			Word();
			Word(string theWord);
			~Word();	  
			bool matches(string ref) override;
			ostream & print(ostream & out) override;

	};
}

#endif


