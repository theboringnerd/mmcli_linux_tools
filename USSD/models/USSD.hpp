#ifndef USSD_H_INCLUDED
#define USSD_H_INCLUDED

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "MODEM.hpp"

using namespace std;


class USSD {
	public:
		USSD() {};

		static USSD init();

		map<string,map<string, string>> getCommands();
		void setCommand(string, map<string, string>);

		void dial(string, vector<string>);
	
	private:
		string trim(string input);
		map<string, map<string, string>> command;
		vector<string> parse(string);
};

string USSD::trim(string input) {
	while(input[0] == ' ') {
		input.erase(0, 1);
	}
	while(input.back() == ' ') {
		input.erase(input.back(), 1);
	}

	return input;
}


void USSD::dial(string command, vector<string> val= {}) {
	map<string, string> __c = this->command[command];

	if(__c.empty()) {
		cout << "Dialing command not found: " << command << endl;
		return;
	} 

	string com = __c.begin()->first;
	string mod__ = __c.begin()->second;
	//cout << "command: " << com << endl;
	//cout << "modem: " << mod__ << endl;

	//if modem, parse commands
	Modem modem = Modem::init(mod__);
	if(!modem.good()) {
		cout << "Modem: " << mod__ << " not found!" << endl;
		return;
	}

	cout << "Done initializing modem!" << endl;


	modem.enable();
	modem.dial(com, val);
}

vector<string> USSD::parse(string input) {
	auto s__ = 0;
	vector<string> __;
	for(auto cp__ = input.find(",", s__); cp__ != string::npos;) {
		__.push_back(this->trim(input.substr(s__, cp__ - s__)));
		s__ = cp__ +1;
		cp__ = input.find(",", s__);
	}
	__.push_back(this->trim(input.substr(s__, input.length() -1)));
	return __;
}

void USSD::setCommand(string key, map<string, string>value) {
	this->command.insert(make_pair(key, value));
}


USSD USSD::init() {
	USSD ussd;
	//This stream should come dynamically from settings
	ifstream readfile("files/ussd_confs.txt");
	if(!readfile.good()) {
		cout << "Configuration file not good!" << endl;
		return ussd;
	}

	string __;
	vector<string> v__;
	while(getline(readfile, __)) v__.push_back(__);	

	for(auto i: v__) {
		vector<string> v1__ = ussd.parse(i);
		ussd.setCommand(v1__[0], {{v1__[1], v1__[2]}});
	}
	readfile.close();

	return ussd;
}

map<string,map<string,string>> USSD::getCommands() {
	return this->command;
}

#endif
