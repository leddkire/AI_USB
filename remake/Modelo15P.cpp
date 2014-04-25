
#include "Modelo.h"


#include <bitset>
#include <vector>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <queue>


#include <iostream>
using namespace std;

class Estado15P : public Estado
{
	
public:
	bitset<64> matriz;
	bitset<4> ubicacion0;

	~Estado15P(){}
	Estado15P(){
		matriz.reset();
		ubicacion0.reset();
	}
	Estado15P(bitset<64> matrizE, bitset<4> ubicacion){
		matriz = matrizE;
		ubicacion0 = ubicacion;
	}

	int costo(Accion* a){
		return 1;
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
	hash<bitset<64>> fHash; //Funcion de hash para el estado
	unordered_map<size_t,bitset<64>> mHashCerrados; //Mapa donde se colocaran los estados cerrados

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
	}
	//Funcion que determina la heuristica
	int h(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);
		bitset<64> estado = e -> matriz;
		int distancia = 0;
		int i2;
		int j2;
		for(int i=0 ; i < 4; i++){
			for(int j=0; j < 4; j++){
				bitset<4> valor; 
				for(int k=0; k< 4; k++){
					valor[3 - k] = estado[63 - i*16 - j*4 - k];
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

	virtual size_t calcularHash(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);
		return fHash(e -> matriz);

	}

	int estaCerrado(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);
		size_t hash = fHash(e -> matriz);
		return mHashCerrados.count(hash);

	}

	void insertarCerrado(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);
		size_t hash = fHash(e -> matriz);
		pair<size_t,bitset<64>> parHashEstado (hash,e->matriz);
		mHashCerrados.insert(parHashEstado);
	}
};

class Manhattan : public Heuristica{
public:
	unordered_map<size_t, int> costos;

	Manhattan() {};

	Manhattan(Estado* inicial) {
		generarHeuristicas(inicial);
	};

	void generarHeuristicas(Estado* modelo) {
		bitset<8> numero;
		bitset<8> posicion;
		bitset<8> preLlave;
		hash<bitset<8>> hash;
		int v, d, v2, d2, distancia;
		size_t key;

			//Iteracion que indica a cual numero le calculamos la distancia
			for(int i=1; i < 16; i++){
				numero = bitset<8>(i);
				numero = numero << 4;
				v = i/4;
				d = i % 4;
				//Iteracion que pasa por todas las posiciones posibles para un numero
				for(int j=0; j < 16; j++){
					posicion = bitset<8>(j);
					preLlave = numero ^ posicion;
					key = hash(preLlave);
					v2 = j/4;
					d2 = j%4;
					distancia = abs(v - v2) + abs(d - d2);
					costos.insert({key, distancia});
					printf("Posicion: %d - Distancia: %d\n", j, distancia);
				}
			}
		 
	};
};

class PDB : public Heuristica{
public:
	unordered_map<size_t, int> costosp1;
	unordered_map<size_t, int> costosp2;
	unordered_map<size_t, int> costosp3;
	PDB() {};

	PDB(Estado* inicial) {
		generarHeuristicas(inicial);
	};

	void generarHeuristicas(Estado* inicial) {
		Estado15P* ini = static_cast<Estado15P*>(inicial);
		Estado15P* patron1 = new Estado15P(bitset<64>(1048575),bitset<4>(0));
		Estado15P* patron2 = new Estado15P(bitset<64>(1048575),bitset<4>(0));
		Estado15P* patron3 = new Estado15P(bitset<64>(1048575),bitset<4>(0));
		patron1 -> matriz <<= 40;
		patron2 -> matriz <<= 20;

		cout << patron1 -> matriz << "\n";
		cout << ini -> matriz <<  "\n";

		patron1 -> matriz &= ini->matriz;

		cout << patron1 -> matriz << "\n";

		patron2 -> matriz &= ini->matriz;
		patron3 -> matriz &= ini->matriz;

		ofstream  archivop1;
		ofstream archivop2;
		ofstream archivop3;
		archivop1.open ("archivop1.txt");
		archivop2.open("archivop2.txt");
		archivop3.open("archivo3.txt");

		Modelo15P m = Modelo15P();


		queue<pair<Estado15P*,int>> por_revisar;
		unordered_map<size_t,Estado15P*> cerrados;
		por_revisar.push(pair<Estado15P*,int>(patron1,0));
		pair<Estado15P*,int> por_generar;
		hash<bitset<64>> fHash;
		vector<ParEstadoAccion> sucesores;
		size_t jhash;
		int elCero;
		//ARCHIVO 1
		while(!por_revisar.empty()){
			por_generar = por_revisar.front();
			por_revisar.pop();
			sucesores = m.succ(por_generar.first);
			jhash = m.calcularHash(por_generar.first);
			cerrados.insert({jhash,por_generar.first});

			//Aqui se guarda en el archivo
			archivop1 << jhash << " " << por_generar.second << "\n"; 
			
			for(int i=0;i < sucesores.size();i++){
				Accion15P* a15 = static_cast<Accion15P*>(sucesores[i].a);
				Estado15P* e15 = static_cast<Estado15P*>(sucesores[i].s);
				elCero = e15 -> ubicacion0.to_ulong();
				if(!(cerrados.count(m.calcularHash(e15) == 1))){

					if((a15 -> accion) == bitset<2>(0)){
						if( e15 -> matriz[63- elCero*4 - 16] == 0){
							por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(1)){
						if( e15 -> matriz[63- elCero*4 -4] == 0){
							por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(2)){
						if( e15 -> matriz[63- elCero*4 +4 ] == 0){
							por_revisar.push(pair<Estado15P*,int>( e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(3)){
						if( e15 -> matriz[63- elCero*4 + 16] == 0){
							por_revisar.push(pair<Estado15P*,int>( e15, por_generar.second));
						}
					}

						por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second + 1));
				}else{
					delete a15;
					delete e15;
				}
					
					
			}
		}
		cout << "paso";
		cerrados.clear();
		sucesores.clear();
		//ARCHIVO2
		while(!por_revisar.empty()){
			por_generar = por_revisar.front();
			por_revisar.pop();
			sucesores = m.succ(por_generar.first);
			jhash = m.calcularHash(por_generar.first);
			cerrados.insert({jhash,por_generar.first});

			//Aqui se guarda en el archivo
			archivop2<< jhash << " " << por_generar.second << "\n"; 
			cout << jhash << " " << por_generar.second << "\n"; 
			
			for(int i=0;i < sucesores.size();i++){
				Accion15P* a15 = static_cast<Accion15P*>(sucesores[i].a);
				Estado15P* e15 = static_cast<Estado15P*>(sucesores[i].s);
				elCero = e15 -> ubicacion0.to_ulong();
				if(!(cerrados.count(m.calcularHash(e15) == 1))){

					if((a15 -> accion) == bitset<2>(0)){
						if( e15 -> matriz[63- elCero*4 - 16] == 0){
							por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(1)){
						if( e15 -> matriz[63- elCero*4 -4] == 0){
							por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(2)){
						if( e15 -> matriz[63- elCero*4 +4 ] == 0){
							por_revisar.push(pair<Estado15P*,int>( e15, por_generar.second));
						}
					}else if((a15-> accion) == bitset<2>(3)){
						if( e15 -> matriz[63- elCero*4 + 16] == 0){
							por_revisar.push(pair<Estado15P*,int>( e15, por_generar.second));
						}
					}

						por_revisar.push(pair<Estado15P*,int>(e15, por_generar.second + 1));
				}
					
					
			}
		}

		//ARCHIVO3
			

			archivop1.close();
			archivop2.close();
			archivop3.close();
		};
		
	};
