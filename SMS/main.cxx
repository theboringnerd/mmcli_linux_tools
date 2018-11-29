#include "../Modem.hpp"
#include <iostream>
#include <vector>
#include "__m/SMS.hpp"

using namespace std;



int main(int argc, char** argv) {

	bool send = false;
	string msg, number, cls = "1";

	if(argc < 2) {
		cout << "Pass in commands\nExiting!" << endl;
		return 1;
	}

	for(int i=0;i<argc;i++) {
		if((string)argv[i] == "--send") {
			send = true;
		}
		else if((string)argv[i] == "--message") {
			msg = (string)argv[i+1];
			i++;
			continue;
		}
		else if((string)argv[i] == "--number") {
			number = (string)argv[i+1];
			i++;
			continue;

		}
		else if((string)argv[i] == "--class") {
			cls = (string)argv[i+1];
			i++;
			continue;
		
		}
	}

	if(send) {
		if(!msg.empty())
			if(!number.empty())
				cout << "Commands received all good!" << endl;
			else {
				cout << "Number not set!" << endl;
				return 1;
			}
		else {
			cout << "Message not set!" << endl;
			return 1;
		}
	}
	else {
		cout << "Not a valid input operation!" << endl;
		return 1;
	}
	
	
	Modem modem = Modem::init("E3131A");

	if(modem.good()) cout << "Modem found and ready!" << endl;
	else cout << "Modem not found or not ready!" << endl;

	SMS sms = SMS::init(modem);

	sms.set_msg(msg);
	sms.set_number(number);
	sms.set_class(cls);
	sms.prepare();
	if(sms.good()) {
		if(!sms.send())
			cout << "Error sending message!" << endl;
	}
	else {
		cout << "sms is not configured correctly!! ENDING" << endl;
		return 1;
	}
	

	return 0;
}


