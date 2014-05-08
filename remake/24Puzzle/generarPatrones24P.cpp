#include "Modelo24P.cpp"
#include <bitset>
#include <iostream>

using namespace std;
int main(){

	bitset<125> estadoGoal = bitset<125>();
	int i = 1;
	do{
		estadoGoal = estadoGoal << 5;
		estadoGoal = estadoGoal ^ bitset<125>(i);
		
		i++;
	}while(i < 25);

	cout << estadoGoal << endl;
	Estado24P goal = Estado24P(estadoGoal, bitset<5>(0));


	cout<< "Generando tablas... \n";
	PDB p = PDB(goal);

}