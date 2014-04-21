#include <vector>

#ifndef ALGO
#define ALGO

#include "Nodo.cpp"
using namespace std;

class AlgoritmoBusqueda{
public:
	Modelo* modelo;

	vector<Nodo> succ(Nodo n){
		vector<ParEstadoAccion> estadosAccion;
		vector<Nodo> sucesores;
		Estado* test1;
		Accion* test2;
		estadosAccion = modelo -> succ(n.estado);
		for(int i = 0; i < estadosAccion.size(); i++){
			/*test1 = estadosAccion[i].s;
			test1 -> imprimirEstado();
			test2 = estadosAccion[i].a;
			test2 -> imprimirAccion();*/
			sucesores.push_back(Nodo(estadosAccion[i].s, &n, estadosAccion[i].a));
		}
		return sucesores;
	}
	virtual int buscar(Nodo inicial) = 0;
};

#endif