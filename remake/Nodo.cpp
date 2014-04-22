#include <cstddef>





#ifndef NODO_H
#define NODO_H
#include "Modelo.h"

class Nodo{
public:
	Estado* estado;
	Nodo* padre;
	Accion* accion;
	int costo;
	int profundidad;
	Nodo(){
		estado = NULL;
		padre = NULL;
		accion = NULL;
		costo = 0;
		profundidad = -1;
	}
	//Constructor equivalente a make_root_node()
	Nodo(Estado* e){
		estado = e;
		padre = NULL;
		accion = NULL;
		costo = 0;
		profundidad = 0;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado* e, Nodo *n, Accion* a){
		estado = e;
		padre = n;
		accion = a;
		costo = n -> costo + (e -> costo(a));
		profundidad = n -> profundidad + 1;
	}
	~Nodo(){
		delete estado;
		delete accion;
	}
	
};


class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (Nodo* izq, Nodo* der) const
	{
		if(izq -> costo >= der -> costo){
			return true;
		}else{
			return false;
		}
	}
};

#endif


