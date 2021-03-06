
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


class Manhattan : public Heuristica{
public:
	unordered_map<size_t, int> costos;

	Manhattan() {};

	Manhattan(Estado* inicial) {
		generarHeuristicas(inicial);
	};

	void generarHeuristicas(Estado* modelo) {
		bitset<64> numero;
		bitset<64> posicion;
		bitset<64> preLlave;
		hash<bitset<64>> hash;
		int v, d, v2, d2, distancia;
		size_t key;

			//Iteracion que indica a cual numero le calculamos la distancia
			for(int i=1; i < 16; i++){
				numero = bitset<64>(i);
				numero = numero << 4;
				v = i/4;
				d = i % 4;
				//Iteracion que pasa por todas las posiciones posibles para un numero
				for(int j=0; j < 16; j++){
					posicion = bitset<64>(j);
					preLlave = numero ^ posicion;
					key = hash(preLlave);
					v2 = j/4;
					d2 = j%4;
					distancia = abs(v - v2) + abs(d - d2);
					costos.insert({key, distancia});
					//printf("Posicion: %d - Distancia: %d - Hash: %lu\n", j, distancia, key);
				}
			}
		 
	};

	int generarEstimado(Estado* sucesor) {
		Estado15P* suc = static_cast<Estado15P*>(sucesor);
		bitset<64> mascara = bitset<64>(15);
		bitset<64> temporal, prekey;
		int j = 0;
		int cosTotal=0;
		size_t key;
		hash<bitset<64>> funhash;
		//suc->imprimirEstado();
		for(int i = 60; i >= 0; i = i-4) {
			temporal = (suc -> matriz) >> i;
			//cout << "Probando!" << temporal << "\n";
			temporal &= mascara;
			temporal = bitset<64>(temporal.to_ulong());
			temporal <<= 4;
			prekey = temporal ^ bitset<64>(j);
			//cout << "que coño pasa: " << prekey << "\n";
			key = funhash(prekey);
			//printf("LEROLERO \n");
			//cout << costos.begin()->second;
			if (costos.count(key) == true){
				//printf("Cara de tetero\n");
				cosTotal = cosTotal + costos.find(key) -> second;
			}
			j++;
		}
		//printf("COSTOTOTAL: %d\n", cosTotal);
		return cosTotal;
	}
};


class Modelo15P : public Modelo
{
	
public:
	Estado15P* inicial;
	Estado15P* goal;
	hash<bitset<64>> fHash; //Funcion de hash para el estado
	hash<bitset<40>> fHashPDBA;
	hash<bitset<68>> fHashPDB;
	unordered_map<size_t,bitset<64>> mHashCerrados; //Mapa donde se colocaran los estados cerrados
	Manhattan heuris;
	unordered_map<size_t,int> map1;
	unordered_map<size_t,int> map2;
	unordered_map<size_t,int> map3;

	Modelo15P(){
		bitset<64> estadoGoal;

		inicial = NULL;
		goal = NULL;
		int i = 1;
		do{
			estadoGoal = estadoGoal << 4;
			estadoGoal = estadoGoal ^ bitset<64>(i);
		
		i++;
		}while(i < 16);
		Estado15P* goal = new Estado15P(estadoGoal, bitset<4>(0));
		Manhattan* heuris = new Manhattan(goal);
	}

	Modelo15P(Estado* i, Estado* g){
		inicial = static_cast<Estado15P*>(i);
		goal = static_cast<Estado15P*>(g);
		heuris = Manhattan(goal);
	}

	Modelo15P(Estado* i, Estado* g, unordered_map<size_t,int> m1,
									unordered_map<size_t,int> m2,
									unordered_map<size_t,int> m3){
		inicial = static_cast<Estado15P*>(i);
		goal = static_cast<Estado15P*>(g);
		map1 = m1;
		map2 = m2;
		map3 = m3;
	}

	Estado15P leer(int fd){

	};

	Estado* init(){
		Estado15P ini = Estado15P();
		inicial = &ini;
		return inicial;
	};

	int heuristicaEstado(Estado* s) {
		Estado15P* est = static_cast<Estado15P*>(s);
		return heuris.generarEstimado(est);
	}

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
	bitset<40> insertarEnPatron(bitset<40> p, bitset<4> v){
		p <<= 4;
		for(int j = 0; j<4 ; j++){
			p[j] = v[j];
		}
	

		return p;
	}


	//Funcion que determina la heuristica
	int h(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);

		bitset<40> patron1 = bitset<40>(0);
		bitset<40> patron2 = bitset<40>(0);
		bitset<40> patron3 = bitset<40>(0);

		bitset<64> temp = bitset<64>(0);
		temp.set();
		temp &= e->matriz;

		//La heuristica esta basada en una pdb dividida en tres patrones
		// de cinco valores.
		bitset<4> valor;
		valor.set();
		bitset<4> indice;
		int valorI;

		for(int i = 15; i >= 0; i--){

			//extraer el valor
			for(int j = 0; j<4 ; j++){
				valor[j] = temp[j];
			}

			//extraer la posicion
			indice = bitset<4>(i);
			//Determinar en que bitset colocar
			//Son tres casos
			// 1<=valor <6
			// 6<= valor <11
			// 11<= valor <16
			valorI = valor.to_ulong();
			if(valorI >= 1 && valorI < 6){
				patron1 = insertarEnPatron(patron1,valor);
				patron1 = insertarEnPatron(patron1,indice);
			}else if(valorI >= 6 && valorI < 11){
				patron2 = insertarEnPatron(patron2,valor);
				patron2 = insertarEnPatron(patron2,indice);
			}else if(valorI >= 11 && valorI < 16){
				patron3 = insertarEnPatron(patron3,valor);
				patron3 = insertarEnPatron(patron3,indice);
			}
			temp >>= 4;

		}

		int v1;
		int v2;
		int v3;

		v1 = map1.at(fHashPDBA(patron1));
		v2 = map2.at(fHashPDBA(patron2));
		v3 = map3.at(fHashPDBA(patron3));



		//Aqui se calcula el hash de los tres y se busca en las tablas
		// para los valores.
		return v1 + v2 + v3;
		/*
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
	*/
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

	virtual size_t calcularHashPDB(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);
		long int valor = e -> ubicacion0.to_ulong(), valorMatriz = ((e -> matriz).to_ulong());
		bitset<68> forHash = bitset<68>(valorMatriz);
		bitset<68> valor0 = bitset<68>(valor);
		valor0 <<= 64;
		valor0 ^=forHash;
		return fHashPDB(valor0);
	}

	size_t calcularHashPDBArchivo(Estado* s){
		Estado15P* e = static_cast<Estado15P*>(s);

		bitset<40> patron = bitset<40>(0);


		bitset<64> temp = bitset<64>(0);
		temp.set();
		temp &= e->matriz;

		bitset<4> valor;
		valor.set();
		bitset<4> indice;
		int valorI;

		for(int i = 15; i >= 0; i--){
			//extraer el valor
			for(int j = 0; j<4 ; j++){
				valor[j] = temp[j];
			}
			if(valor != bitset<4>(0)){
				//extraer la posicion
				indice = bitset<4>(i);
				patron = insertarEnPatron(patron,valor);
				patron = insertarEnPatron(patron,indice);
			}
			temp >>= 4;
		}
		return fHashPDBA(patron);

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
		bitset<64> patronson = bitset<64>(15);
		unordered_map<size_t,bitset<1>> ya_escritos;

		patron1 -> matriz <<= 40;
		patron2 -> matriz <<= 20;


		patron1 -> matriz &= ini->matriz;
		patron2 -> matriz &= ini->matriz;
		patron3 -> matriz &= ini->matriz;

		ofstream  archivop1;
		ofstream archivop2;
		ofstream archivop3;
		archivop1.open ("archivop1.txt");
		archivop2.open("archivop2.txt");
		archivop3.open("archivop3.txt");

		Modelo15P m = Modelo15P();


		queue<pair<Estado15P*,int>> por_revisar;
		unordered_multimap<size_t,Estado15P*> cerrados;
		
		pair<Estado15P*,int> por_generar;
		int posDelCero;
		vector<ParEstadoAccion> sucesores;
		size_t jhash;
		int elCero, i = 0;
		por_revisar.push(pair<Estado15P*,int>(patron1,0));
		jhash = m.calcularHashPDBArchivo(patron1);
		cerrados.insert({m.calcularHashPDB(patron1),patron1});
		archivop1 << jhash << " " << 0 << "\n";
		//ARCHIVO 1
		while(!por_revisar.empty()){
			//i++;
			//printf("Cantidad de abiertos: %d \n", i);
			por_generar = por_revisar.front();
			por_revisar.pop();
			
			sucesores = m.succ(por_generar.first);
			
			//Aqui se guarda en el archivo
			
			//printf("Estado por generar:\n"); 
			//por_generar.first -> imprimirEstado();
			
			for(int i=0;i < sucesores.size();i++){
				Accion15P* a15 = static_cast<Accion15P*>(sucesores[i].a);
				Estado15P* e15 = static_cast<Estado15P*>(sucesores[i].s);
				bitset<64> temporal;
				int costo = 0;
				elCero = e15 -> ubicacion0.to_ulong();

				if(cerrados.count(m.calcularHashPDB(e15)) == 0){
					//printf("Sucesor:\n");
					//e15 -> imprimirEstado();
					if((a15 -> accion) == bitset<2>(0)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(1)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						//posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(2)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(3)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}
					jhash = m.calcularHashPDBArchivo(e15);
					cerrados.insert({m.calcularHashPDB(e15),e15});

					if(ya_escritos.count(jhash) == 0){
						ya_escritos.insert({jhash,bitset<1>(0)});
						archivop1 << jhash << " " << costo << "\n";
					}

					
					
				}else{
					delete a15;
					delete e15;
				}
					
				}	
			
		}
		cout << "paso2\n";
		cerrados.clear();
		sucesores.clear();
		ya_escritos.clear();
		queue<pair<Estado15P*,int>>().swap(por_revisar);
		vector<ParEstadoAccion>().swap(sucesores);

		//ARCHIVO2
		por_revisar.push(pair<Estado15P*,int>(patron2,0));
		jhash = m.calcularHashPDBArchivo(patron2);
		cerrados.insert({m.calcularHashPDB(patron2),patron2});
		archivop2 << jhash << " " << 0 << "\n";
		//ARCHIVO 1
		while(!por_revisar.empty()){
			//i++;
			//printf("Cantidad de abiertos: %d \n", i);
			por_generar = por_revisar.front();
			por_revisar.pop();
			
			sucesores = m.succ(por_generar.first);
			
			//Aqui se guarda en el archivo
			
			//printf("Estado por generar:\n"); 
			//por_generar.first -> imprimirEstado();
			
			for(int i=0;i < sucesores.size();i++){
				Accion15P* a15 = static_cast<Accion15P*>(sucesores[i].a);
				Estado15P* e15 = static_cast<Estado15P*>(sucesores[i].s);
				bitset<64> temporal;
				int costo = 0;
				elCero = e15 -> ubicacion0.to_ulong();

				if(cerrados.count(m.calcularHashPDB(e15)) == 0){
					//printf("Sucesor:\n");
					//e15 -> imprimirEstado();
					if((a15 -> accion) == bitset<2>(0)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(1)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(2)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(3)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}
					jhash = m.calcularHashPDBArchivo(e15);
					cerrados.insert({m.calcularHashPDB(e15),e15});
					if(ya_escritos.count(jhash) == 0){
						ya_escritos.insert({jhash,bitset<1>(0)});
						archivop2 << jhash << " " << costo << "\n";
					}
					
				}else{
					delete a15;
					delete e15;
				}
					
				}	
			
		}
		cout << "paso al 3\n";
		cerrados.clear();
		sucesores.clear();
		ya_escritos.clear();
		queue<pair<Estado15P*,int>>().swap(por_revisar);
		vector<ParEstadoAccion>().swap(sucesores);

		//ARCHIVO3
		por_revisar.push(pair<Estado15P*,int>(patron3,0));
		jhash = m.calcularHashPDBArchivo(patron3);
		cerrados.insert({m.calcularHashPDB(patron3),patron3});
		archivop3 << jhash << " " << 0 << "\n";
		//ARCHIVO 1
		while(!por_revisar.empty()){
			//i++;
			//printf("Cantidad de abiertos: %d \n", i);
			por_generar = por_revisar.front();
			por_revisar.pop();
			
			sucesores = m.succ(por_generar.first);
			
			//Aqui se guarda en el archivo
			
			//printf("Estado por generar:\n"); 
			//por_generar.first -> imprimirEstado();
			
			for(int i=0;i < sucesores.size();i++){
				Accion15P* a15 = static_cast<Accion15P*>(sucesores[i].a);
				Estado15P* e15 = static_cast<Estado15P*>(sucesores[i].s);
				bitset<64> temporal;
				int costo = 0;
				elCero = e15 -> ubicacion0.to_ulong();

				if(cerrados.count(m.calcularHashPDB(e15)) == 0){
					//printf("Sucesor:\n");
					//e15 -> imprimirEstado();
					if((a15 -> accion) == bitset<2>(0)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(1)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 - 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(2)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 4) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}else if((a15-> accion) == bitset<2>(3)){
						//printf("La accion fue: ");
						//cout << a15 -> accion << "\n";
						posDelCero= (63- elCero*4 + 16) - 3;
						//printf("POS DEL CERO %d \n", posDelCero);
						temporal = (e15-> matriz) >> posDelCero;
						temporal &= patronson;
						posDelCero= temporal.to_ulong();
						//printf("QUE HABIA DONDE ELCERO: %d\n", posDelCero);
						if( posDelCero == 0){
							//printf("La posicion del cero ES blanco\n");
							costo = por_generar.second;
							por_revisar.push(pair<Estado15P*,int>( e15, costo));
						}else{
							//printf("La posicion del cero no es blanco\n");
							costo = por_generar.second + 1;
							por_revisar.push(pair<Estado15P*,int>(e15, costo));
						}
					}
					jhash = m.calcularHashPDBArchivo(e15);
					cerrados.insert({m.calcularHashPDB(e15),e15});
					if(ya_escritos.count(jhash) == 0){
						ya_escritos.insert({jhash,bitset<1>(0)});
						archivop3 << jhash << " " << costo << "\n";
					}
					
				}else{
					delete a15;
					delete e15;
				}
					
				}	
			
		}
		cout << "paso a terminar\n";
		cerrados.clear();
		queue<pair<Estado15P*,int>>().swap(por_revisar);
		sucesores.clear();
		vector<ParEstadoAccion>().swap(sucesores);

			archivop1.close();
			archivop2.close();
			archivop3.close();
		};
		
	};
