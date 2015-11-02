#include <iostream>
#include <vector>

using namespace std;

int main() {

	vector<int> v {1, 2, 3, 4};
	vector<int>::iterator it;

	if( it == v.end() ) 
		cout << "T" << endl;
	else
		cout << "F" << endl;


	if( it == nullptr ) 
		cout << "T" << endl;
	else
		cout << "F" << endl;

	return 0;

}
