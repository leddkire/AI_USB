/*Programa principal del proyecto de A* IDA*

Anna Gomez
Erik Dell 08-10301
Francisco Gamundi

*/



#include "Modelo15P.cpp" 


#include "aEstrella.cpp"
#include "idaEstrella.cpp"


//Revisar estas librerias porque puede que haya una que no se necesite
#include <chrono>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

//Para eliminar

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
//


int main(){
	//Declaracion de variables
	string line; //string para la obtencion de cada estado inicial
	ifstream file; //stream para leer del archivo
//POR AHORA SE INICIALIZAN COMO PARA EL 15-PUZZLE
//Antes de esto se tiene que preguntar que tipo de problema se quiere resolver
	vector<int> estadoI; //estructura para almacenar los datos del estado inicial
	bitset<64> tile;
	bitset<64> buffer;
	bitset<64> estadoComp;
	bitset<64> estadoGoal;
	bitset<4> ubicacion0; //Indice donde se encuentra en cuadro blanco
	std::chrono::duration<double> tTotal;
	////
	//Apertura de archivo (Mas tarde se pedira en vez de colocarse aqui)
	file.open("15testsE.txt");
	//No olvidar chequeo de errores

	int i = 1;
	do{
		estadoGoal = estadoGoal << 4;
		estadoGoal = estadoGoal ^ bitset<64>(i);
		
		i++;
	}while(i < 16);

while(getline(file,line)){
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
	//Creacion del nodo inicial
	cout << "\nEstado Inicial: \n\n";
	//imprimirEstado(estadoComp);
	Estado15P* E_inicial = new Estado15P(estadoComp, ubicacion0);
	Nodo* inicial = new Nodo(E_inicial);
	
	
	//Creacion del estado objetivo
	
	//imprimirEstado(estadoGoal);
	
	
	Estado15P* goal = new Estado15P(estadoGoal, bitset<4>(0));

	goal -> imprimirEstado();
	//Creacion del modelo
	Modelo15P modelo = Modelo15P(E_inicial, goal);


	int dMan = modelo.h(inicial -> estado);
	cout << "Distancia Manhattan: "<< dMan << "\n";
	cout << distanciaMan(estadoComp);
	//Creacion del algoritmo
	//OJO, SE TIENE QUE PEDIR CUAL ALGORITMO USAR
	//POR AHORA A*

	

	idaEstrella alg = idaEstrella(&modelo);
	int resultado;
	chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
	resultado = alg.buscar(inicial);

	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();

  	std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  	tTotal = tTotal + tiempo_corrida;
  	cout << "Tardo " << tiempo_corrida.count() << " segundos.";
  	cout << std::endl;

	cout << resultado << "\n";
	if(resultado != -1){
		cout << "No se encontro una solucion";
	}
	estadoI.clear();
	vector<int>().swap(estadoI);
	estadoComp.reset();
}
	cout << "Tiempo de duracion de todas las corridas:" << tTotal.count() << "\n";
	file.close();

	

}
	
