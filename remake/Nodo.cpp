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
	int f; //Valor sumado de el costo mas la heuristica del estado
	Nodo(){
		estado = NULL;
		padre = NULL;
		accion = NULL;
		costo = 0;
		profundidad = -1;
		f = -1;
	}
	//Constructor equivalente a make_root_node()
	Nodo(Estado* e){
		estado = e;
		padre = NULL;
		accion = NULL;
		costo = 0;
		profundidad = 0;
		f = 0;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado* e, Nodo *n, Accion* a){
		estado = e;
		padre = n;
		accion = a;
		costo = n -> costo + (e -> costo(a));
		profundidad = n -> profundidad + 1;
		f = costo;
	}
	~Nodo(){
		delete estado;
		delete accion;
		padre = NULL;
	}
	
};


class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (Nodo* izq, Nodo* der) const
	{
		if(izq -> f >= der -> f){
			return true;
		}else{
			return false;
		}
	}
};

#endif


