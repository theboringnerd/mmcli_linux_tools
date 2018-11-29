#include "../Modem.hpp"
#include <iostream>
#include <vector>
#include "__m/SMS.hpp"

using namespace std;



int main(int argc, char** argv) {
	//change from modem file without removing the entire libraries
	Modem modem = Modem::init("E3131A");

	if(modem.good()) cout << "Modem found and ready!" << endl;
	else cout << "Modem not found or not ready!" << endl;

	SMS sms = SMS::init(modem);
	
	sms.set_msg("hello world");
	sms.set_number("652156811");
	sms.set_class("0");
	sms.prepare();
	if(sms.good()) sms.send();
	else {
		cout << "sms is not configured correctly!! ENDING" << endl;
		return 1;
	}

	return 0;
}


