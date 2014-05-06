#include <cstddef>





#ifndef NODO_H
#define NODO_H
#include "Modelo.h"

class Nodo{
public:
	Estado* estado;
	Nodo* padre;
	Accion* accion;
	int profundidad;
	int f; //Valor sumado de el costo mas la heuristica del estado
	Nodo(){
		estado = NULL;
		padre = NULL;
		accion = NULL;
		profundidad = -1;
		f = -1;
	}
	//Constructor equivalente a make_root_node()
	Nodo(Estado* e){
		estado = e;
		padre = NULL;
		accion = NULL;
		profundidad = 0;
		f = 0;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado* e, Nodo *n, Accion* a){
		estado = e;
		padre = n;
		accion = a;
		profundidad = n -> profundidad + 1;
		f = 0;
	}

	
};


class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (Nodo* izq, Nodo* der) const
	{

		return izq -> f > der -> f;

	}
};

#endif


