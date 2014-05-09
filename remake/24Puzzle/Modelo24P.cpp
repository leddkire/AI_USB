#ifndef MODELO24
#define MODELO24


#include <bitset>
#include <vector>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <queue>


#include <iostream>
using namespace std;

class Accion24P
{
	
public:
	bitset<2> accion;

	Accion24P(){
		accion.reset();
	}

	Accion24P(bitset<2> a){
		accion = a;
	}

	void imprimirAccion(){

		cout << accion.to_ulong();
	}
};


class Estado24P
{
	
public:
	bitset<125> matriz;
	bitset<5> ubicacion0;
	size_t valorHash;

	Estado24P(){
		matriz.reset();
		ubicacion0.reset();
	}
	Estado24P(bitset<125> matrizE, bitset<5> ubicacion){
		matriz = matrizE;
		ubicacion0 = ubicacion;
	}

	int costo(Accion24P a){
		return 1;
	}

	void imprimirEstado(){
		for(int i = 24; i >= 0; i--){
			if(i % 5 == 0){
				for(int j = 4 ; j >=0  ; j--){
					cout << matriz[i*5 + j];
				}
				cout  << "  " << "\n";
			}else{
				for(int j = 4 ; j >=0  ; j--){
					cout << matriz[i*5 + j];
				}
				cout << "  ";
			}

		}
	}

};

struct Hash_Estado_t {
    size_t operator()(const size_t p) const {
    	return p;
    }
    bool operator()(const size_t p, const size_t q) const {
    	return p == q;
    }
};


class ParEstadoAccion{
	
public:
	Accion24P a;
	Estado24P s;

	ParEstadoAccion(Accion24P ac, Estado24P st){
		a = ac;
		s = st;

	}

};





class Modelo24P
{
	
public:
	Estado24P inicial;
	Estado24P goal;
	hash<bitset<125>> fHash; //Funcion de hash para el estado
	hash<bitset<40>> fHashPDBA;
	hash<bitset<130>> fHashPDB;
	unordered_map<size_t,bitset<125>> mHashCerrados; //Mapa donde se colocaran los estados cerrados
	unordered_map<size_t,int> map1;
	unordered_map<size_t,int> map2;
	unordered_map<size_t,int> map3;
	unordered_map<size_t,int> map4;
	unordered_map<size_t,int> map5;
	unordered_map<size_t,int> map6;

	Modelo24P(){
		bitset<125> estadoGoal;

		inicial = Estado24P();
		goal = Estado24P();
		int i = 1;
		do{
			estadoGoal = estadoGoal << 5;
			estadoGoal = estadoGoal ^ bitset<125>(i);
		
		i++;
		}while(i < 16);
		Estado24P goal = Estado24P(estadoGoal, bitset<5>(0));
	}

	Modelo24P(Estado24P i, Estado24P g){
		inicial = i;
		goal = g;
	}

	Modelo24P(Estado24P i, Estado24P g, unordered_map<size_t,int> m1,
									unordered_map<size_t,int> m2,
									unordered_map<size_t,int> m3,
									unordered_map<size_t,int> m4,
									unordered_map<size_t,int> m5,
									unordered_map<size_t,int> m6){
		inicial = i;
		goal = g;
		map1 = m1;
		map2 = m2;
		map3 = m3;
		map4 = m4;
		map5 = m5;
		map6 = m6;
	}

	Estado24P leer(int fd){

	};

	bool is_goal(bitset<125> estado){
		if (estado == goal.matriz){
			return true;
		}
		return false;
	};

	vector<ParEstadoAccion> succ(bitset<125> matriz, bitset<5> ubicacion0){
		int posCero;
		Estado24P eG;
		vector<ParEstadoAccion> sucesores;
		bitset<125> estadoComp;
		bitset<125> valor;
 		posCero = ubicacion0.to_ulong();
 		estadoComp = matriz;
 		Accion24P a;

		//Revisa cada posible accion (Arriba, Abajo, Izquierda, Derecha)
		//Y efectua el cambio de estado adecuado para luego crear el nodo.
		//Hacia arriba
		if(posCero >= 5){
			estadoComp = matriz;
			a = Accion24P(bitset<2>(0));
			valor = (estadoComp >> 125 - posCero*5 + 20) & bitset<125>(31);
			estadoComp = (valor << 125 - posCero*5 -5)|(estadoComp & ~(bitset<125>(31)<<(125 - posCero*5 + 20)));
			eG = Estado24P(estadoComp, bitset<5>(posCero-5));
			sucesores.push_back(ParEstadoAccion(a,eG));
		}
		//Hacia abajo
		if(posCero <= 19){
			estadoComp = matriz;
			a = Accion24P(bitset<2>(3));
			valor = (estadoComp >> 125 - posCero*5 - 30) & bitset<125>(31);
			estadoComp = (valor << 125 - posCero*5 - 5)|(estadoComp & ~(bitset<125>(31)<<(125 - posCero*5- 30)));
			
			eG = Estado24P(estadoComp, bitset<5>(posCero+5));
			sucesores.push_back(ParEstadoAccion(a,eG));	
		}
		//Izquierda
		if(posCero % 5 != 0){
			estadoComp = matriz;
			a =  Accion24P(bitset<2>(1));
			valor = (estadoComp >> 125 - posCero*5) & bitset<125>(31);
			estadoComp = (valor << 125 - posCero*5 - 5)|(estadoComp & ~(bitset<125>(31)<<(125 - posCero*5)));
			eG = Estado24P(estadoComp, bitset<5>(posCero-1));
			sucesores.push_back(ParEstadoAccion(a,eG));

			
		}
		//Derecha
		if(posCero % 5 != 4){
			estadoComp = matriz;
			a = Accion24P(bitset<2>(2));
			valor = (estadoComp >> 125 - posCero*5 - 10) & bitset<125>(31);
			estadoComp = (valor << 125 - posCero*5 -5)|(estadoComp & ~(bitset<125>(31)<<(125 - posCero*5- 10)));
			eG = Estado24P(estadoComp, bitset<5>(posCero+1));
			sucesores.push_back(ParEstadoAccion(a,eG));
		}	
			



		
		

		return sucesores;
	}
	//Pendiente
	bitset<40> insertarEnPatron(bitset<40> p, bitset<5> v){
		p <<= 5;
		for(int j = 0; j<5 ; j++){
			p[j] = v[j];
		}
	

		return p;
	}


	//Funcion que determina la heuristica ---- PENDIENTE 24
	int h(bitset<125> matriz, bitset<5> ubicacion0){
		
		bitset<40> patron1 = bitset<40>(0);
		bitset<40> patron2 = bitset<40>(0);
		bitset<40> patron3 = bitset<40>(0);
		bitset<40> patron4 = bitset<40>(0);
		bitset<40> patron5 = bitset<40>(0);
		bitset<40> patron6 = bitset<40>(0);
		bitset<125> temp = bitset<125>(0);
		temp.set();
		temp &= matriz;
		//La heuristica esta basada en una pdb dividida en 4 patrones
		// de 6 valores.
		bitset<5> valor;
		bitset<5> indice;
		int valorI;

		for(int i = 24; i >= 0; i--){

			//extraer el valor
			for(int j = 0; j<5 ; j++){
				valor[j] = temp[j];
			}

			//extraer la posicion
			indice = bitset<5>(i);
			//Determinar en que bitset colocar
			//Son tres casos
			// 1<=valor <6
			// 6<= valor <11
			// 11<= valor <16
			valorI = valor.to_ulong();

			if(valorI >= 1 && valorI < 5){
				patron1 = insertarEnPatron(patron1,valor);
				patron1 = insertarEnPatron(patron1,indice);
			}else if(valorI >= 5 && valorI < 9){
				patron2 = insertarEnPatron(patron2,valor);
				patron2 = insertarEnPatron(patron2,indice);
			}else if(valorI >= 9 && valorI < 13){
				patron3 = insertarEnPatron(patron3,valor);
				patron3 = insertarEnPatron(patron3,indice);
			}else if(valorI >= 13 && valorI < 17){
				patron4 = insertarEnPatron(patron4,valor);
				patron4 = insertarEnPatron(patron4,indice);
			}else if(valorI >= 17 && valorI < 21){
				patron5 = insertarEnPatron(patron5,valor);
				patron5 = insertarEnPatron(patron5,indice);
			}else if(valorI >= 21 && valorI < 25){
				patron6 = insertarEnPatron(patron6,valor);
				patron6 = insertarEnPatron(patron6,indice);
			}
			temp >>= 5;

		}

		int v1;
		int v2;
		int v3;
		int v4;
		int v5;
		int v6;

		v1 = map1.at(fHashPDBA(patron1));
		v2 = map2.at(fHashPDBA(patron2));
		v3 = map3.at(fHashPDBA(patron3));
		v4 = map4.at(fHashPDBA(patron4));
		v5 = map5.at(fHashPDBA(patron5));
		v6 = map6.at(fHashPDBA(patron6));



		//Aqui se calcula el hash de los tres y se busca en las tablas
		// para los valores.
		return v1 + v2 + v3 + v4 + v5 + v6;
	
	}
//INCOMPLETO
	Estado24P operar(bitset<125> matriz, bitset<5> ubicacion0, bitset<2> accion){
		Estado24P estadoGenerado;
		Estado24P eG;
		bitset<125> estadoComp = matriz;
		int posCero = ubicacion0.to_ulong();
//Hay que optimizar estas operaciones
		//Hacia arriba
		if(accion == bitset<2>(0)){
			for(int j = 0 ; j < 5 ; j++){
				estadoComp[124 - (posCero*5) - j] = estadoComp[124 - posCero*5 + 25 - j];
				estadoComp.reset(124 - (posCero*5) + 25 - j);
			}
			eG = Estado24P(estadoComp, bitset<5>(posCero-5));

		}
		//Hacia abajo
		else if(accion == bitset<2>(3)){
			for(int j = 0 ; j < 5 ; j++){
				estadoComp[124 - (posCero*5) - j] = estadoComp[124 - posCero*5 - 25 - j];
				estadoComp.reset(124 - (posCero*5) - 25 - j);
			}
			eG = Estado24P(estadoComp, bitset<5>(posCero+5));

		}
		//Izquierda
		else if(accion == bitset<2>(1)){
			for(int j = 0 ; j < 5 ; j++){
				estadoComp[124 - (posCero*5) - j] = estadoComp[124 - posCero*5 + 5 - j];
				estadoComp.reset(124 - (posCero*5) + 5 - j);
			}
			eG = Estado24P(estadoComp, bitset<5>(posCero-1));

		}
		//Derecha
		else if(accion == bitset<2>(2)){
			for(int j = 0 ; j < 5 ; j++){
				estadoComp[124 - (posCero*5) - j] = estadoComp[124 - posCero*5 - 5 - j];
				estadoComp.reset(124 - (posCero*5) - 5 - j);
			}
			eG = Estado24P(estadoComp, bitset<5>(posCero+1));
		}
		return eG;
	}

	bool esAccionInversa(bitset<2> b1, bitset<2> b2){
		if(~(b1) == b2){
			return true;
		}
		return false;
	}

	virtual size_t calcularHash(bitset<125> matriz){
		return fHash(matriz);

	}

	virtual size_t calcularHashPDB(Estado24P e){
		bitset<125> temp = bitset<125>(4294967295);
		bitset<130> temp2;
		long int valor = e.ubicacion0.to_ulong();
		
		bitset<130> valor0 = bitset<130>(valor);
		valor0 <<= 32;

		//cout << "Metiendo el cero:"<< valor0 << endl;
		//cout << "Matriz:" << e.matriz << endl;
		temp &= e.matriz;
		valor0 = (bitset<130>(temp.to_ulong()))|(valor0 & ~(bitset<130>(4294967295)));
		valor0 <<= 31;
		e.matriz >>= 32;

		//cout <<"Metiendo el primer cuarto:"<< valor0 << endl;
		//cout << "Matriz:" << e.matriz << endl;
		temp = bitset<125>(4294967295);
		temp &= e.matriz;
		temp2 = bitset<130>(temp.to_ulong());
		valor0 = (temp2)|(valor0 & ~(bitset<130>(2147483647)));
		valor0 <<= 31;
		e.matriz >>= 31;

		//cout << "Metiendo el segundo cuarto:"<<valor0 << endl;
		//cout << "Matriz:" << e.matriz << endl;
		temp = bitset<125>(4294967295);
		temp &= e.matriz;
		temp2 = bitset<130>(temp.to_ulong());
		valor0 = (temp2)|(valor0 & ~(bitset<130>(2147483647)));
		valor0 <<= 31;
		e.matriz >>= 31;

		//cout << "Metiendo el tercer cuerto:"<<valor0 << endl;
		//cout << "Matriz:" << e.matriz << endl;
 		temp = bitset<125>(4294967295);
		temp &= e.matriz;
		temp2 = bitset<130>(temp.to_ulong());
		valor0 = (temp2)|(valor0 & ~(bitset<130>(2147483647)));

		//cout <<"Metiendo el cuarto:"<< valor0 << endl;
		
		return fHashPDB(valor0);
	}

	size_t calcularHashPDBArchivo(Estado24P e/*, int ignorar*/){

		bitset<40> patron = bitset<40>(0);


		bitset<125> temp = bitset<125>(0);
		temp.set();
		temp &= e.matriz;

		bitset<5> valor;
		bitset<5> indice;
		int valorI;

		for(int i = 24; i >= 0; i--){
			//extraer el valor
			for(int j = 0; j<5 ; j++){
				valor[j] = temp[j];
			}
			if(valor != bitset<5>(0) /*&& valor != bitset<4>(ignorar)*/){
				//extraer la posicion
				indice = bitset<5>(i);
				patron = insertarEnPatron(patron,valor);
				patron = insertarEnPatron(patron,indice);
			}
			temp >>= 5;
		}
		return fHashPDBA(patron);

	}

	int estaCerrado(bitset<125> matriz){
		size_t hash = fHash(matriz);
		return mHashCerrados.count(hash);

	}

	void insertarCerrado(bitset<125> matriz){
		size_t hash = fHash(matriz);
		pair<size_t,bitset<125>> parHashEstado (hash,matriz);
		mHashCerrados.insert(parHashEstado);
	}
};

class compararB{
public:
	//el de 6 para el 24, que coño
	bool operator() (pair<Estado24P,int> izq, pair<Estado24P,int> der) const
	{

		return izq.second > der.second;

	}

};



class PDB {
public:
	unordered_map<size_t, int> costosp1;
	unordered_map<size_t, int> costosp2;
	unordered_map<size_t, int> costosp3;
	PDB() {};

	PDB(Estado24P inicial) {
		generarHeuristicas(inicial);
	};

	bitset<5> extraerValor(bitset<125> matriz, int i){
		bitset<5> valor;
		for(int j = 0; j < 5 ; j++){
			valor[j] = matriz[i];
			i++;
		}
		return valor;
	}



	void generarPatron(ofstream& fd, Estado24P patron){
		bitset<125> patronson = bitset<125>(31);
		unordered_map<size_t,bitset<1>> ya_escritos;
		Modelo24P m = Modelo24P();
		priority_queue<pair<Estado24P,int>, vector<pair<Estado24P,int>>, compararB> por_revisar;
		//unordered_set<size_t, Hash_Estado_t, Hash_Estado_t> cerrados;
		unordered_map<size_t,bitset<1>> cerrados;
		bitset<125> temporal;
		Estado24P e15;
		Accion24P a15;
		
		pair<Estado24P,int> por_generar;
		short costoI;
		vector<ParEstadoAccion> sucesores;
		size_t jhash;
		size_t ehash;
		size_t invhash;
		short posDelCero;
		int elCero, i = 0;
		int costo = 0;
		por_revisar.push(pair<Estado24P,int>(patron,costo));

		cerrados.insert({m.calcularHashPDB(patron),bitset<1>(0)});

		//ARCHIVO 1
		while(!por_revisar.empty()){

			por_generar = por_revisar.top();
			
			
			jhash = m.calcularHashPDBArchivo(por_generar.first);
							
			if(ya_escritos.count(jhash) == 0){
				ya_escritos.insert({jhash,bitset<1>(0)});
				fd << jhash << " " << por_generar.second << "\n";
			}
			invhash = m.calcularHash(por_generar.first.matriz);
			
			sucesores = m.succ(por_generar.first.matriz, por_generar.first.ubicacion0);
			por_revisar.pop();

			for(int i=0;i < sucesores.size();i++){
				
				Estado24P e15 = sucesores[i].s;
				Accion24P a15 = sucesores[i].a;
				jhash= m.calcularHash(e15.matriz);


				if(cerrados.count(m.calcularHashPDB(e15)) == 0){

					if(jhash == invhash){
						costo = por_generar.second;
						por_revisar.push(pair<Estado24P,int>(e15, costo));
					}else{
						costoI = por_generar.second;
						costo = costoI + 1;
						por_revisar.push(pair<Estado24P,int>(e15, costo));
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
		priority_queue<pair<Estado24P,int>, vector<pair<Estado24P,int>>, compararB>().swap(por_revisar);
		vector<ParEstadoAccion>().swap(sucesores);
	}

	void generarHeuristicas(Estado24P ini) {
		Estado24P patron1 = Estado24P(bitset<125>(1048575),bitset<5>(0));
		Estado24P patron2 = Estado24P(bitset<125>(1048575),bitset<5>(0));
		Estado24P patron3 = Estado24P(bitset<125>(1048575),bitset<5>(0));
		Estado24P patron4 = Estado24P(bitset<125>(1048575),bitset<5>(0));
		Estado24P patron5 = Estado24P(bitset<125>(1048575),bitset<5>(0));
		Estado24P patron6 = Estado24P(bitset<125>(1048575),bitset<5>(0));

		patron1.matriz <<= 100;
		patron2.matriz <<= 80;
		patron3.matriz <<= 60;
		patron4.matriz <<= 40;
		patron5.matriz <<= 20;

		patron1.matriz &= ini.matriz;
		patron2.matriz &= ini.matriz;
		patron3.matriz &= ini.matriz;
		patron4.matriz &= ini.matriz;
		patron5.matriz &= ini.matriz;
		patron6.matriz &= ini.matriz;

		patron1.imprimirEstado();
		cout << "\n";
		patron2.imprimirEstado();
		cout << "\n";
		patron3.imprimirEstado();
		cout << "\n";
		patron4.imprimirEstado();
		cout << "\n";
		patron5.imprimirEstado();
		cout << "\n";
		patron6.imprimirEstado();

		ofstream  archivop1;
		ofstream archivop2;
		ofstream archivop3;
		ofstream archivop4;
		ofstream archivop5;
		ofstream archivop6;
		archivop1.open ("archivop1-24P.txt");
		archivop2.open("archivop2-24P.txt");
		archivop3.open("archivop3-24P.txt");
		archivop4.open("archivop4-24P.txt");
		archivop5.open("archivop5-24P.txt");
		archivop6.open("archivop6-24P.txt");

		


		
		//ARCHIVO 1
		generarPatron(archivop1,patron1);
		cout << "paso1\n";
		archivop1.close();

		//ARCHIVO2
		generarPatron(archivop2,patron2);
		cout << "paso2\n";
		archivop2.close();
		//ARCHIVO3
		generarPatron(archivop3,patron3);
		cout << "paso3\n";
		archivop3.close();
		//ARCHIVO4
		generarPatron(archivop4,patron4);
		cout << "paso4\n";
		archivop4.close();
		//ARCHIVO5
		generarPatron(archivop5,patron5);
		cout << "paso5\n";
		archivop5.close();
		//ARCHIVO6
		generarPatron(archivop6,patron6);
		cout << "paso6\n";
		archivop6.close();
			

		};
		
};

class Gen24P {
public:

	Gen24P(Estado24P inicial) {
		ofstream  archivogen;
		Accion24P accion = Accion24P();
		archivogen.open ("archivoCasos.txt");
		generar24P(inicial, 100, 0, accion, archivogen);
		archivogen.close();
	};

	void generar24P(Estado24P nod,int l, int profundi, Accion24P acc, ofstream& fd){
		vector<ParEstadoAccion> sucesores;
		Modelo24P m = Modelo24P();
		bitset<125> tempo;
		bitset<125> mascara = bitset<125>(31);
		mascara <<= 120;
		bitset<125> para_escribir;
		int encontrado;
		int resultado;
		int profundidad = profundi+1;
		//int profundidad = 0;
		
		if(profundi > l) return;

		sucesores = m.succ(nod.matriz, nod.ubicacion0);
		for(int i = 0; i < sucesores.size();i++){
			//if (i > 1) break;
			if(profundi != 0){
				if(!(m.esAccionInversa(acc.accion, sucesores[i].a.accion))){
					tempo = sucesores[i].s.matriz;
					for(int j=24; j >=0; j--) {
						para_escribir = tempo & mascara;
						para_escribir>>=120;
						fd << para_escribir.to_ulong() << " ";
						tempo<<=5;
					}
					fd << profundidad <<"\n";
					generar24P(sucesores[i].s, l, profundidad, sucesores[i].a, fd);
				}
			}else{
				generar24P(sucesores[i].s, l, profundidad, sucesores[i].a, fd);
			}
		}
	}

		
};

#endif