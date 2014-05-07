#include <cstddef>





#ifndef NODO_H
#define NODO_H
#include "Modelo15P.cpp"

class Nodo{
public:
	Nodo* padre;
	bitset<86> datos;
	Nodo(){
		padre = NULL;
		datos = bitset<86>(0);
	}
	//Constructor equivalente a make_root_node()
	Nodo(Estado15P e){
		bitset<64> bit;
		bitset<86> bit2;
		//Meter la ubicacion
		datos &= bitset<86>(e.ubicacion0.to_ulong());
		datos <<= 64;
		//Meter primera mitad del estado
		bit= bitset<64>((2^32)-1);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)));
		datos <<= 32;
		//Meter segunda mitad del estado
		e.matriz >>= 32;
		bit= bitset<64>((2^32)-1);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)));

		padre = n;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado15P e, Nodo *n, Accion15P a){
		bitset<64> bit;
		bitset<86> bit2;

		int profundidad = n.getProf(profundidad) + 1; 

		datos = bitset<86>(profundidad);
		datos <<= 2;
		bit2 = bitset<86>(a.accion.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(3)));
		datos <<= 4;

		//Meter la ubicacion
		bit2 = bitset<86>(e.ubicacion0.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(15)));
		datos <<= 64;
		//Meter primera mitad del estado
		bit= bitset<64>((2^32)-1);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)));
		datos <<= 32;
		//Meter segunda mitad del estado
		e.matriz >>= 32;
		bit= bitset<64>((2^32)-1);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)));

		padre = n;
	}

	bitset<8> getF(){

	}

	bitset<8> getProf(){

	}

	bitset<8> getCosto(){
		return profundidad;
	}

	bitset<2> getAccion(){

	}

	bitset<4> getUbicacion0(){

	}

	bitset<64> getEstado(){

	}

	void setF(bitset<8> d){

	}

	void setProf(bitset<8> d){

	}

	void setAccion(bitset<2> d){

	}

	void setUbicacion0(bitset<4> d){

	}

	void setEstado(bitset<64> d){
		
	}

};



/*
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

	int getCosto(){
		return profundidad;
	}
};
*/

class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (Nodo* izq, Nodo* der) const
	{

		return izq -> f > der -> f;

	}
};

#endif


