#include <cmath>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <set> //to create a unique elements list
#include <sstream>  //to be able to use stringstream
//ttps://www.eecis.udel.edu/~breech/progteam/stringstream.html
using namespace std;
std::vector<std::vector<int> > getTruthTable(int n) {
	// n is the size of the element set
	// This will produce all the possible combinations of truth tables for that size
	int rows = (int)pow(2, n);
	int value = 0;
	std::vector<int> row;
	std::vector<std::vector<int> > truthValues;
	for (int i = 0; i < rows; i++) {
		for (int j = n - 1; j >= 0; j--) {
			value = (i / (int)pow(2, j)) % 2;
			row.push_back(value);
			//cout << value << " ";
		}
		truthValues.push_back(row);
		row.clear();
		//cout << endl;
	}

	return truthValues;
}
	int main()
	{
		int sizeOfSet; 
		cout << "Enter the size of the set you want to generate the truth table combinations of : ";
		std::cin >> sizeOfSet;
		std::vector<std::vector<int> > truthTableCombinations = getTruthTable(sizeOfSet);

		std::vector<std::vector<int> >::iterator it_ex; // iterator for the "outer" structure
		std::vector<int>::iterator it_in; // iterator for the "inner" structure

		for (it_ex = truthTableCombinations.begin(); it_ex != truthTableCombinations.end(); it_ex++)
		{
			
			for (it_in = it_ex->begin(); it_in != it_ex->end(); it_in++)
				cout << *it_in << " ";
			cout  << endl;
		}
	}
