#include "models/USSD.hpp"

using namespace std;

vector<USSD> ussd_list;


/* Sample input = /modem/ussd/cashin/{rules which govern this}
 * /modem/sms/send/{rules which govern this}
 * /modem/sms/inbox/{rules which govern this}
 * /modem/sms/facts/{rules whcih govern this}
 *
 *
 * */


vector<string> split(string input, char del = ' ') {
	vector<string> ___;
	size_t _ = input.find(del);
	auto __ = 0;
	while(_ != string::npos) {
		___.push_back(
				input.substr(__, (_ - __))
			     );
		__ = _ + 1;
		_ = input.find(del, __);
	}
	___.push_back(input.substr(__, (_ - __)));
	return ___;
}


int main(int argc, char** argv) {

	if(argc < 2) {
		cout << "Usage----\n--commands []\n--inputs []" << endl;
		return 1;
	}

	string commands, inputs;
	

	//parses cli args for changes and notes the changes
	for(auto i=1;i<argc;i++) {
		if((string)argv[i] == "--commands") {
			commands = argv[i+1];
			++i;
			continue;
		}
		else if((string)argv[i] == "--inputs") {
			inputs = argv[i+1];
			++i;
			continue;
		}
	}

	cout << "Command: " << commands << endl;
	vector<string>v__;	
	if(!inputs.empty()) {
		cout << "Inputs: " << inputs << endl;
		v__ = split(inputs, ':');
		for(auto i: v__) cout << "input: " << i << endl;
	}
	else cout << "No inputs!" << endl;


	USSD ussdCommands = USSD::init();
	
	/*
	for(auto i : ussdCommands.getCommands()) {
		cout << "commands: " << i.first << endl;
		for(auto j: i.second) {
			cout << "code: " << j.first << endl;
			cout << "modem: " << j.second << endl;
		}
	} */

	//string commands = "mobile_money_balance";
	
	//dial = (string, vector<string>)
	if(inputs.empty()) ussdCommands.dial(commands);
	else ussdCommands.dial(commands,v__);

	return 0;
}
