#ifndef MODEM_H_INCLUDED
#define MODEM_H_INCLUDED

#include "USSD.hpp"

using namespace std;


class Modem {
	private:
		//Define commands which go into the program from here
		string enableModem = "-e";

		string linux_system(string), index;
		static string pipe_terminal(string);
		static vector<string> parse(string, char);
		static string get_index(string);
		static string trim(string);

		string ussdCommand = "--3gpp-ussd-initiate=";
		string ussdCommandResponds = "--3gpp-ussd-respond="; 
		string cancelUssdCommands = "--3gpp-ussd-cancel";
	public:
		Modem() {}

		static Modem init(string modem);

		bool good();

		void dial(string, vector<string>), enable(), setIndex(string index);


};

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
	__.size() > 0 ? __.push_back(trim(input.substr(s__, input.length() -1))) : __.push_back(input);
	return __;
}


string Modem::get_index(string input) {
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
	string __ = get_index(parse(pipe_terminal(lm__))[0]);
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

void Modem::dial(string command, vector<string> vals = {}) {
	cout << "Modem is dialing with command: " << command << endl;
	//[n-n-n]
	if(command[0] == '[' and command[command.length() -1] == ']') {
		command = command.erase(0, 1);
		command = command.erase(command.length() -1, 1);
		cout << "Parsed commands - " << command << endl;
		vector<string> __ = parse(command, '-');
		//for(auto i: __) cout << "ci: " << i << endl;
		string r__ = "";
		switch(__.size()) {
			case 0:
				cout << "Not a valid list of commands" << endl;
				return;
				break;
			case 1:
				r__ = this->linux_system(this->ussdCommand + "\"" + __[0] + "\"");
				cout << "USSD command responds: " << r__ << endl;
				break;
			default:
				r__  = this->linux_system(this->ussdCommand + "\"" + __[0] + "\"");
				cout << "USSD command responds: " << r__ << endl;
				for(int i=1, count =0;i<__.size();++i) {
					if(__[i] == "%") {
						cout << "Needing variable input... " << endl;
						if(vals.empty()) {
							cout << "No values passed, ending" << endl;
							break;
						}
						__[i] = vals[count];
						cout << "Set command value input: " << __[i] << endl;
						++count;
					}	
					r__ = this->linux_system(this->ussdCommandResponds + "\"" + __[i] + "\"");
					cout << "USSD responds to prev query: " << r__ << endl;
				}
				break;
		}
		this->linux_system(this->cancelUssdCommands);
		cout << "Finished passing query" << endl;

	} else {
		cout << "Not valid USSD command in dialing modem" << endl;
		return;
	}
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
