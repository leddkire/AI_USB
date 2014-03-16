#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
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


class Nodo{
	Nodo* padre;
	bitset<4> accion; //Accion que produjo este estado a partir del padre
	int costo;
public:
	Estado estado;
	Nodo(Estado e){
		estado = e;
		padre = NULL;
		accion = bitset<4>(0);
		costo = 0;
	}
	Nodo (Estado e, Nodo &n, bitset<4> a, int costo){
		estado = e;
		padre = &n;
		accion = a;
		costo = costo;
	}
	//Funcion que genera los sucesores de un nodo
	vector<Nodo> succ(){
		vector<Nodo> sucesores;
		int posCero;
		Estado estadoGenerado;
		bitset<64> estadoComp;
		bitset<4> accion;
		posCero = this -> estado.ubicacion0.to_ulong();
		//Revisa cada posible accion (Arriba, Abajo, Izquierda, Derecha)
		//Y efectua el cambio de estado adecuado para luego crear el nodo.
		if(posCero >= 4){
			accion = bitset<4>(0);
			estadoComp = this -> estado.matriz;
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 16 - j];
				estadoComp.reset(63 - posCero*4 + 16 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero-4));
			Nodo nuevo = Nodo(estadoGenerado, *this, accion, this -> costo + 1);
			sucesores.push_back(nuevo);
			
		}
		if(posCero <= 11){
			accion = bitset<4>(1);
			estadoComp = this -> estado.matriz;
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 16 - j];
				estadoComp.reset(63 - posCero*4 - 16 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero+4));
			Nodo nuevo = Nodo(estadoGenerado, *this, accion, this -> costo + 1);
			sucesores.push_back(nuevo);
		}
		if(posCero % 4 != 0){
			accion = bitset<4>(2);
			estadoComp = this -> estado.matriz;
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 + 4 - j];
				estadoComp.reset(63 - posCero*4 + 4 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero-1));
			Nodo nuevo = Nodo(estadoGenerado, *this, accion, this -> costo + 1);
			sucesores.push_back(nuevo);
		}
		if(posCero % 4 != 3){
			accion = bitset<4>(3);
			estadoComp = this -> estado.matriz;
			for(int j = 0 ; j < 4 ; j++){
				estadoComp[63 - (posCero*4) - j] = estadoComp[63 - posCero*4 - 4 - j];
				estadoComp.reset(63 - posCero*4 - 4 - j);
			}
			estadoGenerado = Estado(estadoComp, bitset<4>(posCero+1));
			Nodo nuevo = Nodo(estadoGenerado, *this, accion, this -> costo + 1);
			sucesores.push_back(nuevo);
		}
		return sucesores;
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


int main(){
	//Declaracion de variables
	string line; //string para la obtencion de cada estado inicial
	ifstream file; //stream para leer del archivo
	vector<int> estadoI; //estructura para almacenar los datos del estado inicial
	bitset<64> tile;
	bitset<64> buffer;
	bitset<64> estadoComp;
	bitset<4> ubicacion0; //Indice donde se encuentra en cuadro blanco
	////
	//Apertura de archivo (Mas tarde se pedira en vez de colocarse aqui)
	file.open("15tests.txt");
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


/*
	//Prueba de acciones para un arreglo de bitsets
	int end = 1;
	while (end != 0){
		int dir;
		unsigned long int zero;
		cin >> dir;
		zero = ubicacion0.to_ulong();
		switch(dir){
			case 1:
				if (zero<4){
					cout << "Movimiento invalido\n";
				}else{
					estadoIComp[zero] = estadoIComp[zero]|=estadoIComp[zero - 4];
					estadoIComp[zero -4].reset();
					ubicacion0 = bitset<4>(zero-4);
				}
				imprimirEstado(estadoIComp);
				break;
			case 2:
				if(zero > 11){
					cout << "Movimiento invalido\n";
				}else{
					estadoIComp[zero] = estadoIComp[zero + 4];
					estadoIComp[zero +4].reset();
					ubicacion0 = bitset<4>(zero+4);
				}
				imprimirEstado(estadoIComp);
				break;
			case 3:
				if(zero % 4 == 0){
					cout << "Movimiento invalido\n";
				}else{
					estadoIComp[zero] = estadoIComp[zero - 1];
					estadoIComp[zero - 1].reset();
					ubicacion0 = bitset<4>(zero-1);
				}
				imprimirEstado(estadoIComp);
				break;
			case 4:
				if(zero % 4 == 3){
					cout << "Movimiento invalido\n";
				}else{
					estadoIComp[zero] = estadoIComp[zero + 1];
					estadoIComp[zero + 1].reset();
					ubicacion0 = bitset<4>(zero+1);
				}
				imprimirEstado(estadoIComp);
				break;
		}

	}
	////
*/
	////
	//Creacion del nodo inicial
	cout << "\nEstado Inicial: \n\n";
	imprimirEstado(estadoComp);
	Estado E_inicial = Estado(estadoComp, ubicacion0);
	Nodo inicial = Nodo(E_inicial);
	vector<Nodo> sucesores = inicial.succ();
	cout << "\nAcciones aplicadas (En lo posible en este orden: arriba, abajo, izq, der): \n\n";
	for (int i = 0 ; i < sucesores.size(); i ++){
		Nodo suc = sucesores[i];
		imprimirEstado(suc.estado.matriz);
		cout << "\n";
	}
	////
	//Inicio del algoritmo de busqueda

	////
	file.close();
	



}
