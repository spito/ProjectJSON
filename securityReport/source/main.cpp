#include <iostream>
#include "json.h"

using std::shared_ptr;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::make_shared;
namespace J = Json;

void help(char* name){
	cout << "Usage: " << endl <<
			"\t" << name << " read-string" << "\tparses a JSON string and outputs its content" << endl <<
			"\t" << name << " read-json" << "\tparses a JSON value and outputs it reserializad. Note thatsome special characters are wrongly escaped to Unicode." << endl;
	cout << endl << endl << "We assume that the input is a valid UTF-8." << endl;
}

int main(int argc, char** argv) {
	if(argc == 2){
		auto name = std::string(argv[1]);
		if(name == "read-string") {
			try{
				auto jstring = J::String::readStringFrom(cin);
				cout << jstring->getValue();
			}catch(std::string error){
				cerr << "Error: " << error << endl;
				return 1;
			}
		}else if(name == "read-number"){
			try{
				auto jnumber = J::Number::readNumberFrom(cin);
				printf("%.5f", jnumber->getValue());
			}catch(std::string error){
				cerr << "Error: " << error << endl;
				return 1;
			}
		}else if(name == "read-json"){
			try {
				auto rn = J::Value::readFrom(cin);
				cout << "rn: " << *rn << endl;
			}catch(std::string error){
				cerr << "Error: " << error << endl;
				return 1;
			}
		}else if(name == "test"){
			auto num = make_shared<J::Number>(3.14159265358972);
			auto s = make_shared<J::String>("5.\" ' \1 0");
			auto t = make_shared<J::Boolean>(true);
			auto f = make_shared<J::Boolean>(false);
			auto a = make_shared<J::Array>(J::Array({s, t, f, num}));
			auto o = make_shared<J::Object>(J::Object({{"num", num}, {"t", t}, {"a", a}}));
			cout << *num << endl << *s << endl << endl;
			cout << J::null << " " << *t << " " << *f << endl;
			cout << "a: " << *a << endl;
			cout << "o: " << *o << endl;
		}else{
			cerr << "unknown action: " << name;
			help(argv[0]);
			return 1;
		}
	}else{
		help(argv[0]);
	}
	return 0;
}