#include <iostream>

using namespace std;

int main() {
	long val = 0x00000001;
	if (*((char*) &val) == 1) {
		cout << "litte" << endl;;
	}
	cin >> val;

}