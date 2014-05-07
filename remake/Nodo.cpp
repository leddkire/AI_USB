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
		bitset<86> temp;
		temp = datos >> 78 & bitset<86>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<8> getProf(){
		bitset<86> temp;
		temp = datos >> 70 & bitset<86>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<8> getCosto(){
		bitset<86> temp;
		temp = datos >> 70 & bitset<86>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<2> getAccion(){
		bitset<86> temp;
		temp = datos >> 68 & bitset<86>(3);
		return bitset<2>(temp.to_ulong());
	}

	bitset<4> getUbicacion0(){
		bitset<86> temp;
		temp = datos >> 64 & bitset<86>(15);
		return bitset<4>(temp.to_ulong());
	}

	bitset<64> getEstado(){
		bitset<86> temp;
		bitset<64> temp2;
		temp = datos & bitset<86>((2^32)-1);
		temp2 = bitset<64>(temp.to_ulong());
		temp2 <<= 32;
		datos >>= 32;
		temp = datos & bitset<86>((2^32)-1);
		temp2 = (bitset<64>(temp.to_ulong())) | (temp2 & ~(bitset<64>((2^32) - 1)));
		return temp2;
	}

	void setF(bitset<8> d){
		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 78) | (datos & ~(bitset<8>(255)<< 78));
	}

	void setProf(bitset<8> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 70) | (datos & ~(bitset<8>(255)<< 70));
	}

	void setAccion(bitset<2> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 68) | (datos & ~(bitset<2>(3)<< 68));
	}

	void setUbicacion0(bitset<4> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 64) | (datos & ~(bitset<8>(255)<< 64));
	}

	void setEstado(bitset<64> d){
		bitset<64> bit= bitset<64>((2^32)-1);
		bit &= d;
		bitset<86> bit2 = bitset<86>(bit.to_ulong());
		bit2 <<= 32;
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)) << 32);

		//Meter segunda mitad del estado
		d >>= 32;
		bit= bitset<64>((2^32)-1);
		bit &= d;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(2^32-1)));
		
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


