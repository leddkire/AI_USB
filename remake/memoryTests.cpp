#include "Modelo15P.cpp" 


#include "aEstrella.cpp"
#include "idaEstrella.cpp"

int main(){

	Nodo* n;
	Estado* e;
	Accion* a;

	e = new Estado15P();
	a = new Accion15P();
	n = new Nodo(e);

	vector<Nodo*> nodos;

	nodos.push_back(n);
	e = new Estado15P();
	n = new Nodo(e);
	nodos.push_back(n);
	e = new Estado15P();
	n = new Nodo(e);
	nodos.push_back(n);
	e = new Estado15P();
	n = new Nodo(e);
	nodos.push_back(n);
	e = new Estado15P();
	n = new Nodo(e);

	for(int i = 0 ;i <nodos.size(); i++){
		delete nodos[i];
	}
}