#include <vector>

#ifndef ALGO
#define ALGO

#include "Nodo.cpp"
using namespace std;

class AlgoritmoBusqueda{
public:
	Modelo15P modelo;
	
	vector<Nodo*> succ(Nodo* n){
		vector<ParEstadoAccion> estadosAccion;
		vector<Nodo*> sucesores;

		estadosAccion = modelo.succ(n ->getEstado(), n ->getUbicacion0());
		for(int i = 0; i < estadosAccion.size(); i++){

			sucesores.push_back(new Nodo(estadosAccion[i].s, n, estadosAccion[i].a));
			
		}
		return sucesores;
	}
	virtual int buscar(Nodo* inicial) = 0;
};

#endif