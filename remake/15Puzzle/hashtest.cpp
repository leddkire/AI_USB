
#include <iostream>
#include <bitset>
using namespace std;

int main(){
	
	hash<bitset<64>> hbit;
	bitset<64> test;
	test = bitset<64>(4321);
	size_t hashtest = hbit(test);
	cout << hashtest << "\n";


}