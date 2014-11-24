#include <sstream>
#include <iomanip>
#include "json.h"
#include <cctype>
#include <locale>
#include <iconv.h>
#include <iostream>


#define LENGTH(a) (sizeof(a)/sizeof((a)[0]))

using std::endl;
using std::pair;
using std::hex;
using std::setw;
using std::make_shared;
using std::stringstream;
using std::shared_ptr;


void writeIndentation(ostream &out, int indent) {
	for(int i=0; i<indent; i++){
		out << "\t";
	}
}


ostream &::Json::operator<<(ostream &os, Json::Value const &value) {
	value.dumpTo(os, 0);
	return os;
}

void Json::Number::dumpTo(ostream &out, int indent) const{
	out << value;
}

void Json::String::dumpTo(ostream &out, int indent) const {
	out << "\"";
	for(char c: value){
		switch (c){
			case '\"':
				out << "\\\"";
				break;
			case '\\':
				out << "\\\\";
				break;
			case '/':
				out << "/";
				break;
			case '\b':
				out << "\\b";
				break;
			case '\f':
				out << "\\f";
				break;
			case '\n':
				out << "\\n";
				break;
			case '\r':
				out << "\\r";
				break;
			case '\t':
				out << "\\t";
				break;
			default:
				if(isprint(c)){
					out << c;
				}else{
					stringstream ss;
					ss << hex << setw(4) << (0xffff&(uint32_t)c);
					out << "\\u" << ss;
				}
		}
	}
	out << "\"";
}

void Json::Null::dumpTo(ostream &out, int indent) const {
	out << "null";
}

void Json::Boolean::dumpTo(ostream &out, int indent) const {
	out << (value ? "true" : "false");
}

void Json::Array::dumpTo(ostream &out, int indent) const {
	out << "[" << endl;
	bool first = true;
	for(shared_ptr<Value> v: values){
		if(!first){
			out << "," << endl;
		}
		writeIndentation(out, indent+1);
		out << *v;
		first = false;
	}
	out << endl;
	writeIndentation(out, indent);
	out << "]";
}

void Json::Object::dumpTo(ostream &out, int indent) const {
	out << "{" << endl;
	bool first = true;
	for(pair<string, shared_ptr<Value>> const & kv: values){
		if(!first){
			out << "," << endl;
		}
		writeIndentation(out, indent+1);
		out << Json::String(kv.first);
		out << ":";
		kv.second->dumpTo(out, indent+1);
		first = false;
	}
	out << endl;
	writeIndentation(out, indent);
	out << "}";
}

void consumeWhitespace(istream &in) {
	while(iswspace(in.peek())){
		char whitespace;
		in.read(&whitespace, 1);
	}
}

void checkNotEof(istream &in) {
	if(in.eof()){
		throw std::string("Unexpected EOF.");
	}
}


inline void consumeExpectedCharacter(istream &in, char expectedCharacter) {
	char actual;
	in.read(&actual, 1);
	checkNotEof(in);
	if(actual != expectedCharacter){
		throw std::string("Unexpected character: ")+ actual +std::string(" (expecting")+expectedCharacter + std::string(")");
	}

}

inline void consumeQuote(istream &in){
	consumeExpectedCharacter(in, '\"');
}

shared_ptr<Json::Number> Json::Number::readNumberFrom(istream &in) {
	// TODO: proccess errors
	// TODO: locale
	double x;
	consumeWhitespace(in);
	in >> x;
	return make_shared<Json::Number>(x);
}

shared_ptr<Json::String> Json::String::readStringFrom(istream &in) {
	consumeWhitespace(in);
	consumeQuote(in);
	stringstream ss;
	char c;
	while(!in.read(&c, 1).eof()){
		//std::cout << "Read char: " << c << std::endl;
		switch (c){
			case '"':
				return make_shared<Json::String>(ss.str());
			case '\\':
				char c2;
				in.read(&c2, 1);
				checkNotEof(in);
				switch (c2){
					case '"':
					case '\\':
					case '/':
						ss << c2;
						break;
					case 'b':
						ss << '\b';
						break;
					case 'f':
						ss << '\f';
						break;
					case 'n':
						ss << '\n';
						break;
					case 'r':
						ss << '\r';
						break;
					case 't':
						ss << '\t';
						break;
					case 'u': {
						parseHexaStringSequence(in, ss);
						break;
					}
					default:
						throw std::string("Unexpected character in string escape sequence: ")+c2;
				}
				break;
			default:
				ss << c;
		}
	}

}

shared_ptr<Json::Value> tryOrFail(istream &in, const string expect, shared_ptr<Json::Value> value) {
	for(char e: expect){
		char a;
		in >> a;
		checkNotEof(in);
		if(e != a){
			throw std::string("Error when expecting token '")+expect+"'. Expected char: '"+e+"'. Actual char: '"+a+"'.";
		}
	}
	return value;
}

shared_ptr<Json::Value> Json::Value::readFrom(istream &in) {
	consumeWhitespace(in);
	char c = in.peek();
	checkNotEof(in);
	switch(c){
		case '\"':
			return String::readStringFrom(in);
		case '[':
			return Array::readArrayFrom(in);
		case '{':
			return Object::readObjectFrom(in);
		case 't':
			return tryOrFail(in, "true", make_shared<Json::Boolean>(true));
		case 'f':
			return tryOrFail(in, "false", make_shared<Json::Boolean>(false));
		case 'n':
			return tryOrFail(in, "null", Json::null);
		default:
			if(isdigit(c) || (c == '-')){
				return Number::readNumberFrom(in);
			}
			throw std::string("Unexpected character when deciding input type: ")+c;
	}
}

int hexDigitToInt(char c){
	if(('a' <= c) && (c <= 'f')){
		return c-'a'+10;
	}
	if(('A' <= c) && (c <= 'F')){
		return c-'A'+10;
	}
	if(('0' <= c) && (c <= '9')){
		return c-'0'+0;
	}
	throw std::string("bad hexa character");
}

int charToInt(char c){
	return 0xff & ((int)c);
}

void swap(char &c1, char &c2){
	char tmp = c1;
	c1 = c2;
	c2 = tmp;
}

void Json::String::parseHexaStringSequence(istream &in, stringstream &ss) {
	char src[2];
	for(int i=0; i<2; i++){
		char cs[2];
		in.read(cs, 2);
		if(!in.good()){
			throw std::string("Bad \\uXXXX escape sequence");
		}
		src[i] = (char) (hexDigitToInt(cs[0])*16 + hexDigitToInt(cs[1]));
	}

	swap(src[0], src[1]);	// fix byte order - big endian vs. little endian

	size_t inSize = 2;
	iconv_t ic = iconv_open("UTF-8", "UCS-2"); // TODO: autoclose
	if (ic == (iconv_t)(-1)){
		throw std::string("Can't convert characters");
	}

	char* pSrc = src;
	while(inSize > 0){
		size_t outSize = 16;
		char out[outSize];
		char* pOut = out;
		size_t res = iconv(ic, &pSrc, &inSize, &pOut, &outSize);
		if(res == (size_t)-1){
			switch (errno){
				case E2BIG:
					// continue
					break;
				case EILSEQ:
					iconv_close(ic);
					throw std::string("Unexpected rejected characters - EILSEQ");
				case EINVAL:
					iconv_close(ic);
					throw std::string("Unexpected rejected characters - EINVAL");
				default:
					iconv_close(ic);
					throw std::string("Unknown conversion errno.");
			}
		}else{
			size_t copySize = LENGTH(out) - outSize;
			for(int i=0; i<copySize; i++){
				ss << out[i];
			}
		};
	}
	iconv_close(ic);
}

shared_ptr<Json::Array> Json::Array::readArrayFrom(istream &in) {
	vector<shared_ptr<Value>> values;
	consumeWhitespace(in);
	consumeExpectedCharacter(in, '[');
	checkNotEof(in);
	do{
		consumeWhitespace(in);
		if(in.peek() != ']'){
			values.push_back(Value::readFrom(in));
		}
		consumeWhitespace(in);
		checkNotEof(in);
		switch (in.peek()){
			case ',':
				consumeExpectedCharacter(in, ',');
				break; // continue to next element
			case ']':
				break; // will be stopped
			default:
				throw std::string("Unexpected character.");
		}
	}while(in.peek() != ']');
	consumeExpectedCharacter(in, ']');
	checkNotEof(in);

	return std::make_shared<Json::Array>(Json::Array(values));
}

shared_ptr<Json::Object> Json::Object::readObjectFrom(istream &in) {
	map<string, shared_ptr<Value>> props;

	consumeWhitespace(in);
	consumeExpectedCharacter(in, '{');
	checkNotEof(in);

	loop:
	do{
		consumeWhitespace(in);
		if(in.peek() != '}'){
			auto key = String::readStringFrom(in)->getValue();
			consumeWhitespace(in);
			consumeExpectedCharacter(in, ':');
			consumeWhitespace(in);
			auto value = Value::readFrom(in);
			props[key] = value;
			consumeWhitespace(in);
			checkNotEof(in);
			switch(in.peek()){
				case ',':
					consumeExpectedCharacter(in, ',');
					break;
				case '}':
					break; // will be stopped
				default:
					throw std::string("Unexpected character");
			}

		}
	}while(in.peek() != '}');

	consumeExpectedCharacter(in, '}');

	checkNotEof(in);

	return std::make_shared<Json::Object>(Json::Object(props));
}
