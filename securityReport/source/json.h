
#include <ostream>
#include <vector>
#include <map>
#include <memory>

using std::shared_ptr;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::initializer_list;
using std::map;

namespace Json {


	class Value{
	public:
		virtual void dumpTo(ostream &out, int indent) const = 0;
		static shared_ptr<Value> readFrom(istream &in);
	};


	class Number : public Value{
	// TODO: precision etc.
	private:
		double value;
	public:
		Number(double value): value(value) {}
		virtual void dumpTo(ostream &out, int indent) const;
		double getValue(){return value;};
		static shared_ptr<Number> readNumberFrom(istream &in);
	};


	class String : public Value{
	// FIXME: Unprintable characters
	private:
		string value;
		static void parseHexaStringSequence(istream &in, std::stringstream &ss);
	public:
		String(string value): value(value) {}
		virtual void dumpTo(ostream &out, int indent) const;
		string getValue() {return value;}

		static shared_ptr<String> readStringFrom(istream &in);
	};

	class Null: public Value{
	public:
		Null(){}
		static const Null value;
		virtual void dumpTo(ostream &out, int indent) const;
	};
	const auto null = std::make_shared<Null>();

	class Boolean: public Value{
	private:
		bool value;
	public:
		Boolean(bool value): value(value) {};
		virtual void dumpTo(ostream &out, int indent) const;
	};

	class Array: public Value{
	// TODO: safety
	private:
		vector<shared_ptr<Value>> values;
	public:
		Array(initializer_list<shared_ptr<Value>> values): values(values) {};
		Array(vector<shared_ptr<Value>> values): values(values) {};

		virtual void dumpTo(ostream &out, int indent) const;
		//static shared_ptr<Number> readNumberFrom(istream &in);
		static shared_ptr<Array> readArrayFrom(istream &in);
	};

	class Object: public Value{
	// TODO: safety
	private:
		map<string, shared_ptr<Value>> values;
	public:
		Object(map<string, shared_ptr<Value>> values): values(values) {};
		virtual void dumpTo(ostream &out, int indent) const;

		static shared_ptr<Json::Object> readObjectFrom(istream &in);
	};


	ostream& operator<<(ostream& os, const Value& value);

}