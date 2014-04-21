
#include "Modelo.h"


#include <bitset>
#include <vector>
#include <cstddef>
#include <cmath>


#include <iostream>
using namespace std;

class Estado15P : public Estado
{
	
public:
	bitset<64> matriz;
	bitset<4> ubicacion0;

	Estado15P(){
		matriz.reset();
		ubicacion0.reset();
	}
	Estado15P(bitset<64> matrizE, bitset<4> ubicacion){
		matriz = matrizE;
		ubicacion0 = ubicacion;
	}

	int costo(Accion* a){

		int distancia = 1;
		int i2;
		int j2;
		for(int i=0 ; i < 4; i++){
			for(int j=0; j < 4; j++){
				bitset<4> valor; 
				for(int k=0; k< 4; k++){
					valor[3 - k] = matriz[63 - i*16 - j*4 - k];
				}
				if(valor == 0){
					continue;
				}
				//Transformamos el valor
				unsigned long int valorI = valor.to_ulong();
				i2 = valorI / 4;
				j2 = (valorI % 4);
				distancia = distancia + (abs(i - i2) + abs(j - j2));
			}

		}


		return distancia;
	}

	void imprimirEstado(){
		for(int i = 15; i >= 0; i--){
			if(i % 4 == 0){
				for(int j = 3 ; j >=0  ; j--){
					cout << matriz[i*4 + j];
				}
				cout  << "  " << "\n";
			}else{
				for(int j = 3 ; j >=0  ; j--){
					cout << matriz[i*4 + j];
				}
				cout << "  ";
			}

		}
	}
};

class Accion15P : public Accion
{
	
public:
	bitset<2> accion;

	Accion15P(){
		accion.reset();
	}

	Accion15P(bitset<2> a){
		accion = a;
	}

	void imprimirAccion(){

		cout << accion.to_ulong();
	}
};

class Modelo15P : public Modelo
{
	
public:
	Estado15P* inicial;
	Estado15P* goal;

	Modelo15P(){
		inicial = NULL;
		goal = NULL;
	}

	Modelo15P(Estado* i, Estado* g){
		inicial = static_cast<Estado15P*>(i);
		goal = static_cast<Estado15P*>(g);
	}

	Estado15P leer(int fd){

	};

	Estado* init(){
		Estado15P ini = Estado15P();
		inicial = &ini;
		return inicial;
	};

	bool is_goal(Estado* s){
		Estado15P* estado = static_cast<Estado15P*>(s);
		if (estado -> matriz == goal -> matriz){
			return true;
		}
		return false;
	};

	vector<ParEstadoAccion> succ(Estado* s){
		int posCero;
		Estado* estadoGenerado;
		Estado15P* estado = static_cast<Estado15P*>(s);
		vector<ParEstadoAccion> sucesores;
		bitset<64> estadoComp;
		Accion15P a15;
		Accion* a;
		
		posCero = estado -> ubicacion0.to_ulong();

		//Revisa cada posible accion (Arriba, Abajo, Izquierda, Derecha)
		//Y efectua el cambio de estado adecuado para luego crear el nodo.
		if(posCero >= 4){
			a = new Accion15P(bitset<2>(0));
			estadoGenerado = operar(s,a);
			sucesores.push_back(ParEstadoAccion(a,estadoGenerado));	
		}

		if(posCero <= 11){
			a = new Accion15P(bitset<2>(3));
			
			estadoGenerado = operar(s,a);
			sucesores.push_back(ParEstadoAccion(a,estadoGenerado));
		}

		if(posCero % 4 != 0){
			a =  new Accion15P(bitset<2>(1));
			
			estadoGenerado = operar(s,a);
			sucesores.push_back(ParEstadoAccion(a,estadoGenerado));
		}

		if(posCero % 4 != 3){
			a = new Accion15P(bitset<2>(2));
			
			estadoGenerado = operar(s,a);
			sucesores.push_back(ParEstadoAccion(a,estadoGenerado));
		}

		return sucesores;
	};
	//Funcion que determina el costo
	int costo(Estado* s, Accion* a){
		//Falta la heuristica.
		return 1;
	};
//INCOMPLETO
	Estado* operar(Estado* s, Accion* a){
		Estado15P estadoGenerado;
		Estado* eG;
		Estado15P* estado = static_cast<Estado15P*>(s);
		Accion15P* acc = static_cast<Accion15P*>(a);
		bitset<2> accion = acc -> accion;
		bitset<64> estadoComp = estado -> matriz;
		int posCero = estado -> ubicacion0.to_ulong();
//Hay que optimizar estas operaciones
		//Hacia arriba
		if(accion == bitset<2>(0)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 16 - j];
				estadoComp.reset(63 - (posCero*4) + 16 - j);
			}
			eG = new Estado15P(estadoComp, bitset<4>(posCero-4));

		}
		//Hacia abajo
		else if(accion == bitset<2>(3)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 16 - j];
				estadoComp.reset(63 - (posCero*4) - 16 - j);
			}
			eG = new Estado15P(estadoComp, bitset<4>(posCero+4));

		}
		//Izquierda
		else if(accion == bitset<2>(1)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 4 - j];
				estadoComp.reset(63 - (posCero*4) + 4 - j);
			}
			eG = new Estado15P(estadoComp, bitset<4>(posCero-1));

		}
		//Derecha
		else if(accion == bitset<2>(2)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 4 - j];
				estadoComp.reset(63 - (posCero*4) - 4 - j);
			}
			eG = new Estado15P(estadoComp, bitset<4>(posCero+1));
		}
		return eG;
	}

	bool esAccionInversa(Accion* anterior, Accion* aEvaluar){
		Accion15P* anterior15 = static_cast<Accion15P*>(anterior);
		Accion15P* aEvaluar15 = static_cast<Accion15P*>(aEvaluar);
		bitset<2> b1 = anterior15 -> accion;
		bitset<2> b2 = aEvaluar15 -> accion;
		if(~(b1) == b2){
			return true;
		}
		return false;
	}
};