
#ifndef MODELO15
#define MODELO15


#include <bitset>
#include <vector>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <queue>


#include <iostream>
using namespace std;

class Accion15P
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


class Estado15P
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

	int costo(Accion15P a){
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

class ParEstadoAccion{
	
public:
	Accion15P a;
	Estado15P s;

	ParEstadoAccion(Accion15P ac, Estado15P st){
		a = ac;
		s = st;

	}

};


class Manhattan {
public:
	unordered_map<size_t, int> costos;

	Manhattan() {};

	Manhattan(Estado15P inicial) {
		generarHeuristicas(inicial);
	};

	void generarHeuristicas(Estado15P modelo) {
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
				}
			}
		 
	};

	int generarEstimado(Estado15P suc) {
		bitset<64> mascara = bitset<64>(15);
		bitset<64> temporal, prekey;
		int j = 0;
		int cosTotal=0;
		size_t key;
		hash<bitset<64>> funhash;
		for(int i = 60; i >= 0; i = i-4) {
			temporal = suc.matriz >> i;
			temporal &= mascara;
			temporal = bitset<64>(temporal.to_ulong());
			temporal <<= 4;
			prekey = temporal ^ bitset<64>(j);
			key = funhash(prekey);

			if (costos.count(key) == true){
				cosTotal = cosTotal + costos.find(key) -> second;
			}
			j++;
		}
		return cosTotal;
	}
};


class Modelo15P
{
	
public:
	Estado15P inicial;
	Estado15P goal;
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

		inicial = Estado15P();
		goal = Estado15P();
		int i = 1;
		do{
			estadoGoal = estadoGoal << 4;
			estadoGoal = estadoGoal ^ bitset<64>(i);
		
		i++;
		}while(i < 16);
		Estado15P goal = Estado15P(estadoGoal, bitset<4>(0));
		Manhattan heuris = Manhattan(goal);
	}

	Modelo15P(Estado15P i, Estado15P g){
		inicial = i;
		goal = g;
	}

	Modelo15P(Estado15P i, Estado15P g, unordered_map<size_t,int> m1,
									unordered_map<size_t,int> m2,
									unordered_map<size_t,int> m3){
		inicial = i;
		goal = g;
		map1 = m1;
		map2 = m2;
		map3 = m3;
	}

	Estado15P leer(int fd){

	};

	bool is_goal(bitset<64> estado){
		if (estado == goal.matriz){
			return true;
		}
		return false;
	};

	vector<ParEstadoAccion> succ(bitset<64> matriz, bitset<4> ubicacion0){
		int posCero;
		Estado15P eG;
		vector<ParEstadoAccion> sucesores;
		bitset<64> estadoComp;
		bitset<64> valor;
 		posCero = ubicacion0.to_ulong();
 		estadoComp = matriz;
 		Accion15P a;

		//Revisa cada posible accion (Arriba, Abajo, Izquierda, Derecha)
		//Y efectua el cambio de estado adecuado para luego crear el nodo.
		//Hacia arriba
		if(posCero >= 4){
			estadoComp = matriz;
			a = Accion15P(bitset<2>(0));
			valor = (estadoComp >> 64 - posCero*4 + 12) & bitset<64>(15);
			estadoComp = (valor << 64 - posCero*4 -4)|(estadoComp & ~(bitset<64>(15)<<(64 - posCero*4 +12)));

			eG = Estado15P(estadoComp, bitset<4>(posCero-4));
			sucesores.push_back(ParEstadoAccion(a,eG));
		}
		//Hacia abajo
		if(posCero <= 11){
			estadoComp = matriz;
			a = Accion15P(bitset<2>(3));
			valor = (estadoComp >> 64 - posCero*4 - 20) & bitset<64>(15);
			estadoComp = (valor << 64 - posCero*4 - 4)|(estadoComp & ~(bitset<64>(15)<<(64 - posCero*4- 20)));
			
			eG = Estado15P(estadoComp, bitset<4>(posCero+4));
			sucesores.push_back(ParEstadoAccion(a,eG));	
		}
		//Izquierda
		if(posCero % 4 != 0){
			estadoComp = matriz;
			a =  Accion15P(bitset<2>(1));
			valor = (estadoComp >> 64 - posCero*4) & bitset<64>(15);
			estadoComp = (valor << 64 - posCero*4 - 4)|(estadoComp & ~(bitset<64>(15)<<(64 - posCero*4)));
			eG = Estado15P(estadoComp, bitset<4>(posCero-1));
			sucesores.push_back(ParEstadoAccion(a,eG));

			
		}
		//Derecha
		if(posCero % 4 != 3){
			estadoComp = matriz;
			a = Accion15P(bitset<2>(2));
			valor = (estadoComp >> 64 - posCero*4 - 8) & bitset<64>(15);
			estadoComp = (valor << 64 - posCero*4 -4)|(estadoComp & ~(bitset<64>(15)<<(64 - posCero*4- 8)));
			eG = Estado15P(estadoComp, bitset<4>(posCero+1));
			sucesores.push_back(ParEstadoAccion(a,eG));
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
	int h(bitset<64> matriz, bitset<4> ubicacion0){
		

		bitset<40> patron1 = bitset<40>(0);
		bitset<40> patron2 = bitset<40>(0);
		bitset<40> patron3 = bitset<40>(0);

		bitset<64> temp = bitset<64>(0);
		temp.set();
		temp &= matriz;

		//La heuristica esta basada en una pdb dividida en tres patrones
		// de cinco valores.
		bitset<4> valor;
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

		int distancia = 0;
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
	*/
	}
//INCOMPLETO
	Estado15P operar(bitset<64> matriz, bitset<4> ubicacion0, bitset<2> accion){
		Estado15P estadoGenerado;
		Estado15P eG;
		bitset<64> estadoComp = matriz;
		int posCero = ubicacion0.to_ulong();
//Hay que optimizar estas operaciones
		//Hacia arriba
		if(accion == bitset<2>(0)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 16 - j];
				estadoComp.reset(63 - (posCero*4) + 16 - j);
			}
			eG = Estado15P(estadoComp, bitset<4>(posCero-4));

		}
		//Hacia abajo
		else if(accion == bitset<2>(3)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 16 - j];
				estadoComp.reset(63 - (posCero*4) - 16 - j);
			}
			eG = Estado15P(estadoComp, bitset<4>(posCero+4));

		}
		//Izquierda
		else if(accion == bitset<2>(1)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 4 - j];
				estadoComp.reset(63 - (posCero*4) + 4 - j);
			}
			eG = Estado15P(estadoComp, bitset<4>(posCero-1));

		}
		//Derecha
		else if(accion == bitset<2>(2)){
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 4 - j];
				estadoComp.reset(63 - (posCero*4) - 4 - j);
			}
			eG = Estado15P(estadoComp, bitset<4>(posCero+1));
		}
		return eG;
	}

	bool esAccionInversa(bitset<2> b1, bitset<2> b2){
		if(~(b1) == b2){
			return true;
		}
		return false;
	}

	virtual size_t calcularHash(bitset<64> matriz){
		return fHash(matriz);

	}

	virtual size_t calcularHashPDB(Estado15P e){
		long int valor = e.ubicacion0.to_ulong(), valorMatriz = ((e.matriz).to_ulong());
		bitset<68> forHash = bitset<68>(valorMatriz);
		bitset<68> valor0 = bitset<68>(valor);
		valor0 <<= 64;
		valor0 |=forHash;
		return fHashPDB(valor0);
	}

	size_t calcularHashPDBArchivo(Estado15P e/*, int ignorar*/){

		bitset<40> patron = bitset<40>(0);


		bitset<64> temp = bitset<64>(0);
		temp.set();
		temp &= e.matriz;

		bitset<4> valor;
		bitset<4> indice;
		int valorI;

		for(int i = 15; i >= 0; i--){
			//extraer el valor
			for(int j = 0; j<4 ; j++){
				valor[j] = temp[j];
			}
			if(valor != bitset<4>(0) /*&& valor != bitset<4>(ignorar)*/){
				//extraer la posicion
				indice = bitset<4>(i);
				patron = insertarEnPatron(patron,valor);
				patron = insertarEnPatron(patron,indice);
			}
			temp >>= 4;
		}
		return fHashPDBA(patron);

	}

	int estaCerrado(bitset<64> matriz){
		size_t hash = fHash(matriz);
		return mHashCerrados.count(hash);

	}

	void insertarCerrado(bitset<64> matriz){
		size_t hash = fHash(matriz);
		pair<size_t,bitset<64>> parHashEstado (hash,matriz);
		mHashCerrados.insert(parHashEstado);
	}
};

class compararB{
public:
	bool operator() (pair<Estado15P,bitset<6>> izq, pair<Estado15P,bitset<6>> der) const
	{

		return izq.second.to_ulong() > der.second.to_ulong();

	}

};



class PDB {
public:
	unordered_map<size_t, int> costosp1;
	unordered_map<size_t, int> costosp2;
	unordered_map<size_t, int> costosp3;
	PDB() {};

	PDB(Estado15P inicial) {
		generarHeuristicas(inicial);
	};

	bitset<4> extraerValor(bitset<64> matriz, int i){
		bitset<4> valor;
		for(int j = 0; j < 4 ; j++){
			valor[j] = matriz[i];
			i++;
		}
		return valor;
	}



	void generarPatron(ofstream& fd, Estado15P patron, int ignorar){
		bitset<64> patronson = bitset<64>(15);
		unordered_map<size_t,bitset<1>> ya_escritos;
		Modelo15P m = Modelo15P();
		priority_queue<pair<Estado15P,bitset<6>>, vector<pair<Estado15P,bitset<6>>>, compararB> por_revisar;
		unordered_multimap<size_t,bitset<1>> cerrados;
		bitset<64> temporal;
		Estado15P e15;
		Accion15P a15;
		
		pair<Estado15P,bitset<6>> por_generar;
		short costoI;
		vector<ParEstadoAccion> sucesores;
		size_t jhash;
		size_t ehash;
		size_t invhash;
		short posDelCero;
		int elCero, i = 0;
		bitset<6> costo = bitset<6>(0);
		por_revisar.push(pair<Estado15P,bitset<6>>(patron,costo));

		cerrados.insert({m.calcularHashPDB(patron),bitset<1>(0)});

		//ARCHIVO 1
		while(!por_revisar.empty()){

			por_generar = por_revisar.top();
			
			
			jhash = m.calcularHashPDBArchivo(por_generar.first);
							
			if(ya_escritos.count(jhash) == 0){
				ya_escritos.insert({jhash,bitset<1>(0)});
				fd << jhash << " " << por_generar.second.to_ulong() << "\n";
			}
			invhash = m.calcularHash(por_generar.first.matriz);
			
			sucesores = m.succ(por_generar.first.matriz, por_generar.first.ubicacion0);
	
			por_revisar.pop();

			for(int i=0;i < sucesores.size();i++){
				
				Estado15P e15 = sucesores[i].s;
				Accion15P a15 = sucesores[i].a;
				jhash= m.calcularHash(e15.matriz);


				if(cerrados.count(m.calcularHashPDB(e15)) == 0){

					if(jhash == invhash){
						costo = por_generar.second;
						por_revisar.push(pair<Estado15P,bitset<6>>(e15, costo));
					}else{
						costoI = por_generar.second.to_ulong();
						costo = bitset<6>(costoI + 1);
						por_revisar.push(pair<Estado15P,bitset<6>>(e15, costo));
					}

						cerrados.insert({m.calcularHashPDB(e15),bitset<1>(0)});
				}

			}
			sucesores.clear();
			vector<ParEstadoAccion>().swap(sucesores);
		}
		cerrados.clear();
		sucesores.clear();
		ya_escritos.clear();
		priority_queue<pair<Estado15P,bitset<6>>, vector<pair<Estado15P,bitset<6>>>, compararB>().swap(por_revisar);
		vector<ParEstadoAccion>().swap(sucesores);
	}

	void designarValorBlanco(Estado15P e, bitset<4> ignorar){
		bitset<4> valor;
		//Se ignora el cero verdadero, por eso recorre solo 15 posiciones y no 16
		for(int i = 0; i <15; i++){

			valor = extraerValor(e.matriz, i*4);

			if(valor.to_ulong() == 0){
				for(int j = 0; j<4; j++){
					e.matriz[i*4+j] = ignorar[j];
				}
			}
		}


	}

	void generarHeuristicas(Estado15P ini) {
		Estado15P patron1 = Estado15P(bitset<64>(1048575),bitset<4>(0));
		Estado15P patron2 = Estado15P(bitset<64>(1048575),bitset<4>(0));
		Estado15P patron3 = Estado15P(bitset<64>(1048575),bitset<4>(0));

		bitset<4> ignorar1;
		bitset<4> ignorar2;
		bitset<4> ignorar3;

		patron1.matriz <<= 40;
		patron2.matriz <<= 20;

		patron1.matriz &= ini.matriz;
		patron2.matriz &= ini.matriz;
		patron3.matriz &= ini.matriz;

		patron1.imprimirEstado();
		cout << "\n";
		patron2.imprimirEstado();
		cout << "\n";
		patron3.imprimirEstado();

		ofstream  archivop1;
		ofstream archivop2;
		ofstream archivop3;
		archivop1.open ("archivop1.txt");
		archivop2.open("archivop2.txt");
		archivop3.open("archivop3.txt");

		


		
		//ARCHIVO 1
		generarPatron(archivop1,patron1, ignorar1.to_ulong());
		cout << "paso1\n";
		//ARCHIVO2
		generarPatron(archivop2,patron2, ignorar2.to_ulong());
		cout << "paso2\n";
		
		//ARCHIVO3
		generarPatron(archivop3,patron3, ignorar3.to_ulong());
		cout << "paso3\n";

			archivop1.close();
			archivop2.close();
			archivop3.close();
		};
		
	};

#endif
