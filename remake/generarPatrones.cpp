#include "Modelo15P.cpp"
#include <bitset>

using namespace std;
int main(){

	cout << bitset<4>(17) << "\n";
	bitset<64> estadoGoal = bitset<64>();
	int i = 1;
	do{
		estadoGoal = estadoGoal << 4;
		estadoGoal = estadoGoal ^ bitset<64>(i);
		
		i++;
	}while(i < 16);


	Estado15P* goal = new Estado15P(estadoGoal, bitset<4>(0));


	cout<< "Generando tablas... \n";
	PDB p = PDB(goal);

}