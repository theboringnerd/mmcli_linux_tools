#ifndef MODEM_H_INCLUDED
#define MODEM_H_INCLUDED
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;


class Modem {
	private:
		//Define commands which go into the program from here
		string enableModem = "-e";

		string linux_system(string), index;
		static string parse_for_index(string);
		static string trim(string);
		void setIndex(string index);


		string ussdCommand = "--3gpp-ussd-initiate=";
		string ussdCommandResponds = "--3gpp-ussd-respond="; 
		string cancelUssdCommands = "--3gpp-ussd-cancel";
	public:
		Modem() {}

		static Modem init(string modem);

		static vector<string> parse(string, char);
		bool good();

		static vector<Modem> findAllModems();
		static string get_modem_name(string);

		void enable();
		static string pipe_terminal(string);
		
		string get_index();

};


string Modem::get_modem_name(string input) {
	vector<string> _ = Modem::parse(input, ' ');
	return _[_.size() -1];
}


vector<Modem> Modem::findAllModems() {
	cout << "Going to find all modems" << endl;
	//cout << "would initialize modem: " << modem << endl;
	//find the modem in the linux terminal first
	string lm__ = "mmcli -L | grep Modem";
	string __ = Modem::pipe_terminal(lm__);

	vector<Modem> ml;

	size_t car = __.find("\n");
	if(vector<string>___; car != string::npos) {
		cout << "Found carriage at: " << car << endl;
		___ = Modem::parse(__, '\n');
		for(auto i: ___) cout << "Modem found: " << Modem::get_modem_name(i) << endl;
		for(auto i: ___) 
			ml.push_back(Modem::init(Modem::get_modem_name(i)));
	} else {
		cout << "No carriage found!" << endl;
	}

	return ml;
}

string Modem::pipe_terminal(string command) {
	string data;
	FILE * stream;
	const int max_buffer = 1024;
	char buffer[max_buffer];
	command.append(" 2>&1");

	stream = popen(command.c_str(), "r");
	if (stream) {
		while (!feof(stream)) if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		pclose(stream);
	}
	return data;
}


string Modem::get_index() {
	return this->index;
}

string Modem::trim(string input) {
	while(input[0] == ' ') {
		input.erase(0, 1);
	}
	while(input.back() == ' ') {
		input.erase(input.back(), 1);
	}

	return input;
}


vector<string> Modem::parse(string input, char del = ' ') {
	auto s__ = 0;
	vector<string> __;
	for(auto cp__ = input.find(del, s__); cp__ != string::npos;) {
		__.push_back(trim(input.substr(s__, cp__ - s__)));
		s__ = cp__ +1;
		cp__ = input.find(del, s__);
	}
	string lo = trim(input.substr(s__, input.length() -1));
	if(!lo.empty())
		__.size() > 0 ? __.push_back(lo) : __.push_back(input);
	return __;
}


string Modem::parse_for_index(string input) {
	size_t s__ = input.rfind("/") + 1;
	return input.substr(s__, (input.length() - s__));
}

void Modem::setIndex(string index) {
	this->index = index;
}

Modem Modem::init(string modem) {
	cout << "would initialize modem: " << modem << endl;
	//find the modem in the linux terminal first
	string lm__ = "mmcli -L | grep " + modem;
	string __ = parse_for_index(parse(pipe_terminal(lm__))[0]);
	cout << "index set at: " << __ << endl;
	Modem __m;
	__m.setIndex(__);
	return __m;
}

string Modem::linux_system(string command) {
	command = "mmcli -m " + this->index + " " + command;
	cout << "LS: " << command << endl;
	return pipe_terminal(command);
}

bool Modem::good() {
	return !this->index.empty();
}


void Modem::enable() {
	try {
		string __ = Modem::linux_system(this->enableModem);
		cout << "responds: " << __ << endl;

		cout << "Modem has been enabled" << endl;
	}
	catch(exception& e) {
		cout << "Error: " << e.what() << endl;
	}
}

#endif
