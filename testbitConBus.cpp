#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;


class Estado{
public:
	bitset<64> matriz;
	bitset<4> ubicacion0;

	Estado(){
		matriz.reset();
		ubicacion0.reset();
	}
	Estado(bitset<64> matrizE, bitset<4> ubicacion){
		matriz = matrizE;
		ubicacion0 = ubicacion;
	}
		
};

void imprimirEstado(bitset<64> estadoComp){
	for(int i = 15; i >= 0; i--){
		if(i % 4 == 0){
			for(int j = 3 ; j >=0  ; j--){
				cout << estadoComp[i*4 + j];
			}
			cout  << "  " << "\n";
		}else{
			for(int j = 3 ; j >=0  ; j--){
				cout << estadoComp[i*4 + j];
			}
			cout << "  ";
		}

	}
}

int distanciaMan(bitset<64> estado){
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



class Nodo{
	
public:
	Estado estado;
	Nodo* padre;
	bitset<2> accion; //Accion que produjo este estado a partir del padre
	int costo;
	int profundidad;
	Nodo(){
		estado = Estado();
		padre = NULL;
		accion.reset();
		costo = -1;
		profundidad = -1;
	}
	Nodo(Estado e){
		estado = e;
		padre = NULL;
		accion = bitset<2>(0);
		costo = 0;
		profundidad = 0;
	}
	Nodo (Estado e, Nodo *n, bitset<2> a, const int c, int p){
		estado = e;
		padre = n;
		accion = a;
		costo = c;
		profundidad = p;
	}
	
};

//Funcion que genera los sucesores de un nodo
	vector<Nodo> succ(Nodo nodo){
		vector<Nodo> sucesores;
		int posCero;
		Estado estadoGenerado;
		bitset<64> estadoComp;
		bitset<2> accion;
		posCero = nodo.estado.ubicacion0.to_ulong();
		Nodo nuevo;		
		int c = nodo.costo + 1;
		int p = nodo.profundidad + 1;


		//Revisa cada posible accion (Arriba, Abajo, Izquierda, Derecha)
		//Y efectua el cambio de estado adecuado para luego crear el nodo.
		if(posCero >= 4){
			accion = bitset<2>(0);
			estadoComp = nodo.estado.matriz;

			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 16 - j];
				estadoComp.reset(63 - (posCero*4) + 16 - j);
			}

			estadoGenerado = Estado(estadoComp, bitset<4>(posCero-4));
			c = c + distanciaMan(estadoComp);
			nuevo = Nodo(estadoGenerado, &nodo, accion, c, p);
			sucesores.push_back(nuevo);


			
		}

		if(posCero <= 11){
			accion = bitset<2>(3);
			estadoComp = nodo.estado.matriz;
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 16 - j];
				estadoComp.reset(63 - (posCero*4) - 16 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero+4));
			c = c + distanciaMan(estadoComp);
			nuevo = Nodo(estadoGenerado, &nodo, accion,c, p);
			sucesores.push_back(nuevo);


		}

		if(posCero % 4 != 0){
			accion = bitset<2>(1);
			estadoComp = nodo.estado.matriz;

			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 4 - j];
				estadoComp.reset(63 - (posCero*4) + 4 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero-1));
			c = c + distanciaMan(estadoComp);
			nuevo = Nodo(estadoGenerado, &nodo, accion, c, p);
			sucesores.push_back(nuevo);


		}

		if(posCero % 4 != 3){
			accion = bitset<2>(2);
			estadoComp = nodo.estado.matriz;

			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 4 - j];
				estadoComp.reset(63 - (posCero*4) - 4 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero+1));
			c = c + distanciaMan(estadoComp);
			nuevo = Nodo(estadoGenerado, &nodo, accion, c, p);
			sucesores.push_back(nuevo);


			

		}

		return sucesores;
	}

class comparar{//Clase para la comparacion de costos de nodo

public:
	bool operator() (const Nodo izq, Nodo der) const
	{
		if(izq.costo >= der.costo){
			return true;
		}else{
			return false;
		}
	}
};






int main(){
	//Declaracion de variables
	string line; //string para la obtencion de cada estado inicial
	ifstream file; //stream para leer del archivo
	vector<int> estadoI; //estructura para almacenar los datos del estado inicial
	bitset<64> tile;
	bitset<64> buffer;
	bitset<64> estadoComp;
	bitset<64> estadoGoal;
	bitset<4> ubicacion0; //Indice donde se encuentra en cuadro blanco
	////
	//Apertura de archivo (Mas tarde se pedira en vez de colocarse aqui)
	file.open("15testsE.txt");
	//No olvidar chequeo de errores
	getline(file,line);
	//Se construye un sstream a partir de la linea
	//Para poder convertir la linea en un arreglo de enteros
	stringstream str(line);
	cout << "Estado inicial: \n" <<  line << "\n";
	//Se leen los valores de 'str' y se insertan en el vector

	for (int numero; str >> numero;){
		estadoI.push_back(numero);

	}
	file.close();
	//Impresion del vector para comprobar la conversion
	cout << "\nEstado inicial en base 10:\n";
	for (size_t i = 0; i < estadoI.size(); ++i){
		if(i % 4 == 3){
			cout << estadoI[i] << "\n";	
		}else{
			cout << estadoI[i] << "  ";
		}

	}
	cout << endl;

	//Construccion del estado compactado
	for (size_t i = 0; i < estadoI.size(); ++i){
		tile = bitset<64>(estadoI[i]);
		estadoComp = estadoComp << 4;
		estadoComp = estadoComp ^ tile;
		
		
		if(estadoI[i] == 0){
			ubicacion0 = bitset<4>(i);
		}
	}

	cout <<"\nRepresentacion del estado en base 2 y compactado:\n" << estadoComp << "\n";
	//Creacion del nodo inicial
	cout << "\nEstado Inicial: \n\n";
	imprimirEstado(estadoComp);
	Estado E_inicial = Estado(estadoComp, ubicacion0);
	Nodo inicial = Nodo(E_inicial);
	
	vector<Nodo> suce = succ(inicial);
	/*
	cout << "\nAcciones aplicadas (En lo posible en este orden: arriba, abajo, izq, der): \n\n";
	for (int p = 0 ; p < suce.size(); p ++){
		Nodo suc = suce[p];
		imprimirEstado(suc.estado.matriz);
		if(suc.accion == bitset<2>(2)){
			cout << "\nsucesores de moverlo a la izq \n";
			vector<Nodo> testsuc = suc.succ();
			for (int j = 0 ; j < testsuc.size(); j ++){
				Nodo sucs = testsuc[j];
				imprimirEstado(sucs.estado.matriz);
				cout << "\n";
			}
			cout << "fin de sucesores de moverlo a la izq \n";
		}
		cout << "\n";
	}
	*/
	////
	//Creacion del estado objetivo
	int i = 1;
	do{
		estadoGoal = estadoGoal << 4;
		estadoGoal = estadoGoal ^ bitset<64>(i);
		
		i++;
	}while(i < 16);
	cout << "Estado objetivo: \n";
	imprimirEstado(estadoGoal);
	int dMan = distanciaMan(estadoComp);
	cout << "Distancia Manhattan: "<< dMan << "\n";
	//Inicio del algoritmo de busqueda
	priority_queue<Nodo, vector<Nodo>, comparar> nodos;
	int profundidadActual = 0;
	nodos.push(inicial);
	vector<Nodo> sucesores;
	while(1){
		if (nodos.empty()){
			cout << "No se enconctro una solucion \n";
			return 0;
		}else{

			Nodo nodo_A_Evaluar = nodos.top();
			nodos.pop();
			if(nodo_A_Evaluar.profundidad > profundidadActual){
				profundidadActual = nodo_A_Evaluar.profundidad;
				cout << "En profundidad " << profundidadActual << "\n";
			}
			if(nodo_A_Evaluar.estado.matriz == estadoGoal){
				cout << "Se encontro una solucion \n";
				cout << "Paso por " << nodo_A_Evaluar.profundidad << " movimientos \n" ;
				return 0;
			}else{
				sucesores = succ(nodo_A_Evaluar);

				

				bitset<2> a = nodo_A_Evaluar.accion;
				for(int i = 0; i < sucesores.size(); i ++){
					if(~(a) == sucesores[i].accion & sucesores[i].costo != 0){
						continue;
					}
					nodos.push(sucesores[i]);
				}

			}
			/*
						cout << "test2\n";
priority_queue<Nodo, vector<Nodo>, comparar> nodos3;
				nodos3 = nodos;
				while(!nodos3.empty()){
					Nodo nod = nodos3.top();
					nodos3.pop();
					cout << nod.costo;

				}
			cout << "\n";
			int lol;
			cin >> lol;
			*/
			
		}
	}
	////
	
	



}
