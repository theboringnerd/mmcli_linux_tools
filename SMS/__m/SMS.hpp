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
		void set_modem(Modem modem);
};


SMS::SMS() {
	//set sms class to normal by default
	this->cls = "1";
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


#endif
