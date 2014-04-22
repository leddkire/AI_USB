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

	delete n; 
}