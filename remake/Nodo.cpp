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
		
		datos = bitset<86>(e.ubicacion0.to_ulong());
		datos <<= 32;
		
		//Meter primera mitad del estado
		bit= bitset<64>(4294967295);
		
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<86>(4294967295)));
		datos <<= 32;

		//Meter segunda mitad del estado
		e.matriz >>= 32;
		bit= bitset<64>(4294967295);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(4294967295)));

		padre = NULL;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado15P e, Nodo *n, Accion15P a){
		bitset<64> bit;
		bitset<86> bit2;

		int profundidad = n -> getProf().to_ulong() + 1; 
		datos = bitset<86>(profundidad);
		datos <<= 2;
		bit2 = bitset<86>(a.accion.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(3)));
		//cout << "\n Datos despues de meter accion: " << datos << "\n";
		datos <<= 4;

		//Meter la ubicacion
		bit2 = bitset<86>(e.ubicacion0.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(15)));

		//cout << "Datos despues de meter ubicacion: " << datos << "\n";
		datos <<= 32;
		//Meter primera mitad del estado
		bit= bitset<64>(4294967295);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(4294967295)));
		datos <<= 32;
		//Meter segunda mitad del estado
		e.matriz >>= 32;
		bit= bitset<64>(4294967295);
		bit &= e.matriz;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(4294967295)));
		//cout << "Datos despues de meter estado: " << datos << "\n";
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
		bitset<86> datos2;
		bitset<64> temp2;
		temp = datos & bitset<86>(4294967295);
		temp2 = bitset<64>(temp.to_ulong());
		temp2 <<= 32;
		//cout << "Primera mitad: " << temp2 << "\n";
		datos2 = datos >> 32;
		temp = datos2 & bitset<86>(4294967295);
		//cout << "Segunda mitad:"<< temp << "\n";
		temp2 = (bitset<64>(temp.to_ulong())) | (temp2 & ~(bitset<64>(4294967295)));
		return temp2;
	}

	void setF(bitset<8> d){
		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 78) | (datos & ~(bitset<86>(255)<< 78));
	}

	void setProf(bitset<8> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 70) | (datos & ~(bitset<86>(255)<< 70));
	}

	void setAccion(bitset<2> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 68) | (datos & ~(bitset<86>(3)<< 68));
	}

	void setUbicacion0(bitset<4> d){

		bitset<86> temp = bitset<86>(d.to_ulong());
		datos = (temp << 64) | (datos & ~(bitset<86>(255)<< 64));
	}

	void setEstado(bitset<64> d){
		bitset<64> bit= bitset<64>(4294967295);
		bit &= d;
		bitset<86> bit2 = bitset<86>(bit.to_ulong());
		bit2 <<= 32;
		datos = bit2 | (datos & ~(bitset<86>(4294967295)) << 32);

		//Meter segunda mitad del estado
		d >>= 32;
		bit= bitset<64>(4294967295);
		bit &= d;
		bit2 = bitset<86>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<86>(4294967295)));
		
	}

};



class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (Nodo* izq, Nodo* der) const
	{

		return izq -> getF().to_ulong() > der -> getF().to_ulong();

	}
};

#endif


