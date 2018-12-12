#ifndef SMS_H_INCLUDED
#define SMS_H_INCLUDED

#include "../../Modem.hpp"

using namespace std;



class SMS {
	private:
		string msg, number, command, index, cls;
		Modem modem;

	public:
		SMS();
		static SMS init(Modem modem);
		bool good(), prepare();
		void send(), set_msg(string msg), set_number(string number), set_class(string cls);
		void set_modem(Modem modem), remove();
		string get_message(), get_number();
		vector<SMS> get_messages();
		vector<string> split(string, char);

		string trim(string), terminal_message(string, string);
};


SMS::SMS() {
	//set sms class to normal by default
	this->cls = "1";
}

string SMS::get_message() {
	return this->msg;
}

string SMS::get_number() {
	return this->number;
}

void SMS::set_modem(Modem modem) {
	this->modem = modem;
}

void SMS::set_class(string cls) {
	this->cls = cls;
}

SMS SMS::init(Modem modem) {
	SMS sms;
	cout << "Initializing SMS modem" << endl;
	sms.set_modem(modem);
	return sms;
}

bool SMS::good() {
	return this->modem.good() and !this->msg.empty() and !this->number.empty() and !this->command.empty() and !this->index.empty();
}

void SMS::send() {
	string __ = "mmcli -s " + this->index + " --send";
	string reply = Modem::pipe_terminal(__);
	cout << "Command replied with: " << reply << endl;
}

bool SMS::prepare() {
	if(this->modem.get_index().empty()) return false;
	
	this->command = "mmcli -m " + this->modem.get_index() + " --messaging-create-sms=\"text='" + this->msg + "',number='" + this->number + "',class='" + this->cls + "'\" | grep org";
	cout << "COMMAND: " << this->command << endl;

	string respond = Modem::pipe_terminal(this->command);
	cout << "Respond: " << respond << endl;
	
	string __ = Modem::parse(respond)[0];
	size_t s__ = __.rfind("/") +1;
	this->index = respond.substr(s__, (__.length() - s__));
	cout << "SMS index: " << this->index << endl;

	return true;
}

void SMS::set_msg(string msg) {
	this->msg = msg;
}

void SMS::set_number(string number) {
	this->number = number;
}


vector<string> SMS::split(string input, char del) {
	vector<string> __;
	size_t ___ = 0, _ = input.find(del);
	
	while(_ != string::npos) {
		__.push_back(input.substr(___, (_ - ___)));
		___ = _ + 1;
		_ = input.find(del, ___);
	}
	string s_ = input.substr(___, (input.length() - ___));
	if(!s_.empty()) __.push_back(input.substr(___, (input.length() - ___)));
	return __;
}

string SMS::trim(string input) {
	while(input[0] == ' ') {
		input.erase(0, 1);
	}
	while(input.back() == ' ') {
		input.erase(input.back(), 1);
	}

	return input;
}

string SMS::terminal_message(string keyword, string index) {
	string __ = "mmcli -m " + this->modem.get_index() + " -s " + index + " | grep -i " + keyword;
	__ = Modem::pipe_terminal(__);
	__ = trim(__.substr(__.find(" '"), (__.length() - __.find(" '"))));
	__.erase(0, 1);
	__.erase(__.length() -2);

	return __;
}

vector<SMS> SMS::get_messages() {
	string __ = "mmcli -m " + this->modem.get_index() + " --messaging-list-sms | grep received";
	string _ = Modem::pipe_terminal(__);
	cout << _ << endl;
	//if(_.find("\n")!=string::npos) cout << "Found some carriage" << endl;
	vector<string> ___ = split(_, '\n');
	vector<SMS> messages;
	for(auto i: ___) {
		SMS sms;
		//cout << "split: " << i << endl;
		//cout << Modem::parse(i)[0] << endl;
		_ = Modem::parse(i)[0];
		size_t s__ = _.rfind("/") +1;
		string index = i.substr(s__, (_.length() - s__));
		sms.modem = this->modem;
		sms.index = index;
		//cout << "SMS index: " << index << endl;

		//read the messages
		string text = terminal_message("text", index);
		cout << "TEXT:" << text << endl;
		sms.msg = text;
		
		string number = terminal_message("number", index);
		cout << "NUMBER:" << number << endl << endl;
		sms.number = number;

		messages.push_back(sms);
		/*
		string pdu_type = terminal_message("PDU type", index);
		cout << "PDU TYPE: " << pdu_type << endl << endl;
		*/
	}
	return messages;
}

void SMS::remove() {
	cout << "Deleting message with index: " << this->index << endl;
	string __ = "mmcli -m " + this->modem.get_index() + " --messaging-delete-sms " + this->index;
	cout << Modem::pipe_terminal(__) << endl;
}

#endif
