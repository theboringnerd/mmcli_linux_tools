#include "Modem.hpp"

using namespace std;


int main() {
	vector<Modem> list = Modem::findAllModems();
	cout << "Number of modems found: " << list.size() << endl;
}
