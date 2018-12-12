#include "../Modem.hpp"
#include <iostream>
#include <vector>
#include "__m/SMS.hpp"
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;



int main(int argc, char** argv) {

	bool send = false, received = false;
	string msg, number, cls = "1";

	if(argc < 2) {
		cout << "Pass in commands\nExiting!" << endl;
		return 1;
	}

	for(int i=0;i<argc;i++) {
		if((string)argv[i] == "--send") {
			send = true;
		}
		else if((string)argv[i] == "--received") {
			received = true;
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
	else if(!received) {
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
	if(sms.good()) sms.send();
	else if(received) {
		while(1) {
			cout << "Checking for sms!" << endl;
			vector<SMS> messages = sms.get_messages();
			//write everything to file
			for(auto sms_entity : messages) {
				string __ = "messages/" + sms_entity.get_number();
				ofstream _(__.c_str(), ios::app);
				_ << sms_entity.get_message() << endl;
				_.close();
				sms_entity.remove();
			}
			cout << "Number of messages: " << messages.size() << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	}
	else {
		cout << "sms is not configured correctly!! ENDING" << endl;
		return 1;
	}
	

	return 0;
}


