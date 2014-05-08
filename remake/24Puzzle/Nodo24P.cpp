#include <cstddef>





#ifndef NODO_H
#define NODO_H
#include "Modelo24P.cpp"

class Nodo{
public:
	Nodo* padre;
	bitset<148> datos;
	Nodo(){
		padre = NULL;
		datos = bitset<148>(0);
	}
	//Constructor equivalente a make_root_node()
	Nodo(Estado24P e){
		bitset<125> bit;
		bitset<148> bit2;
		//Meter la ubicacion
		
		datos = bitset<148>(e.ubicacion0.to_ulong());
		datos <<= 32;
		
		//Meter primer cuarto del estado
		bit= bitset<125>(4294967295);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(4294967295)));
		datos <<= 31;

		//Meter segundo cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));
		datos <<= 31;

		//Meter tercer cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));
		datos <<= 31;

		//Meter ultimo cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));

		padre = NULL;
	}
	//Constructor equivalente a make_node(Estado,Nodo,Accion)
	Nodo (Estado24P e, Nodo *n, Accion24P a){
		bitset<125> bit;
		bitset<148> bit2;

		//Meter la profundidad
		int profundidad = n -> getProf().to_ulong() + 1; 
		datos = bitset<148>(profundidad);
		datos <<= 2;

		//Meter accion
		bit2 = bitset<148>(a.accion.to_ulong());
		datos = bit2 | (datos & ~(bitset<148>(3)));
		//cout << "\n Datos despues de meter accion: " << datos << "\n";
		datos <<= 5;

		//Meter la ubicacion
		bit2 = bitset<148>(e.ubicacion0.to_ulong());
		datos = bit2 | (datos & ~(bitset<148>(31)));

		//cout << "Datos despues de meter ubicacion: " << datos << "\n";
		datos <<= 32;

		//Meter primer cuarto del estado
		bit= bitset<125>(4294967295);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(4294967295)));
		datos <<= 31;

		//Meter segundo cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));
		datos <<= 31;

		//Meter tercer cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));
		datos <<= 31;

		//Meter ultimo cuarto del estado
		bit= bitset<125>(2147483647);
		
		bit &= e.matriz;
		bit2 = bitset<148>(bit.to_ulong());
		
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));

		padre = n;
	}

	bitset<8> getF(){
		bitset<148> temp;
		temp = datos >> 140 & bitset<148>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<8> getProf(){
		bitset<148> temp;
		temp = datos >> 132 & bitset<148>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<8> getCosto(){
		bitset<148> temp;
		temp = datos >> 132 & bitset<148>(255);
		return bitset<8>(temp.to_ulong());
	}

	bitset<2> getAccion(){
		bitset<148> temp;
		temp = datos >> 130 & bitset<148>(3);
		return bitset<2>(temp.to_ulong());
	}

	bitset<5> getUbicacion0(){
		bitset<148> temp;
		temp = datos >> 125 & bitset<148>(31);
		return bitset<5>(temp.to_ulong());
	}

	bitset<125> getEstado(){

		bitset<148> temp;
		bitset<148> datos2;
		bitset<125> temp2;
		temp = datos & bitset<148>(2147483647);
		temp2 = bitset<125>(temp.to_ulong());
		temp2 <<= 31;
		//cout << "Primera mitad: " << temp2 << "\n";
		datos2 = datos >> 31;
		temp = datos2 & bitset<148>(2147483647);

		temp2 = (bitset<125>(temp.to_ulong())) | (temp2 & ~(bitset<125>(2147483647)));

		datos2 = datos >> 31;
		temp2 <<= 31;
		temp = datos2 & bitset<148>(2147483647);

		temp2 = (bitset<125>(temp.to_ulong())) | (temp2 & ~(bitset<125>(2147483647)));

		datos2 = datos >> 32;
		temp2 <<= 32;
		temp = datos2 & bitset<148>(4294967295);
		//cout << "Segunda mitad:"<< temp << "\n";
		temp2 = (bitset<125>(temp.to_ulong())) | (temp2 & ~(bitset<125>(4294967295)));
		return temp2;
	}

	void setF(bitset<8> d){
		bitset<148> temp = bitset<148>(d.to_ulong());
		datos = (temp << 140) | (datos & ~(bitset<148>(255)<< 140));
	}

	void setProf(bitset<8> d){

		bitset<148> temp = bitset<148>(d.to_ulong());
		datos = (temp << 132) | (datos & ~(bitset<148>(255)<< 132));
	}

	void setAccion(bitset<2> d){

		bitset<148> temp = bitset<148>(d.to_ulong());
		datos = (temp << 130) | (datos & ~(bitset<148>(3)<< 130));
	}

	void setUbicacion0(bitset<5> d){

		bitset<148> temp = bitset<148>(d.to_ulong());
		datos = (temp << 125) | (datos & ~(bitset<148>(255)<< 125));
	}

	void setEstado(bitset<125> d){
		bitset<125> bit= bitset<125>(4294967295);
		bit &= d;
		bitset<148> bit2 = bitset<148>(bit.to_ulong());
		bit2 <<= 93;
		datos = bit2 | (datos & ~(bitset<148>(4294967295)) << 93);

		//Meter segundo cuarto del estado
		d >>= 31;
		bit= bitset<125>(2147483647);
		bit &= d;
		bit2 = bitset<148>(bit.to_ulong());
		bit2 <<= 62;
		datos = bit2 | (datos & ~(bitset<148>(2147483647)) << 62);

		//Meter tercer cuarto del estado
		d >>= 31;
		bit= bitset<125>(2147483647);
		bit &= d;
		bit2 = bitset<148>(bit.to_ulong());
		bit2 <<= 31;
		datos = bit2 | (datos & ~(bitset<148>(2147483647)) << 31);

		//Meter ultimo cuarto del estado
		d >>= 31;
		bit= bitset<125>(2147483647);
		bit &= d;
		bit2 = bitset<148>(bit.to_ulong());
		datos = bit2 | (datos & ~(bitset<148>(2147483647)));
		
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


