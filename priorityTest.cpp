#include <queue>
#include <iostream>

using namespace std;

class Nodo{
	
public:

	int costo;

	Nodo(int c){
		costo = c;
	}

};

class compare{

public:
	bool operator() (Nodo *izq, Nodo *der) const
	{
		if(izq -> costo > der -> costo){
			return true;
		}else{
			return false;
		}
	}
};

int main(){
	priority_queue<Nodo*, vector<Nodo*>, compare> q;
	Nodo blas = Nodo(7);
	Nodo primero = Nodo(1);
	Nodo segundo = Nodo(5);
	Nodo tercero = Nodo (6);
	Nodo cuarto = Nodo (3);
	Nodo bla = Nodo(1);

	q.push(&blas);
	q.push(&primero);
	q.push(&segundo);
	q.push(&tercero);
	q.push(&cuarto);
	q.push(&bla);

	while(!q.empty()){
		Nodo* lol = q.top();
		cout << lol -> costo;
		q.pop();
	}

}