//Algoritmo A*
#include <cstddef>
#include <unordered_map>
#include "Modelo15P.cpp"
#include "AlgoritmoBusqueda.cpp"



#include <queue>

#include <iostream>
using namespace std;



class aEstrella : public AlgoritmoBusqueda{
public:

	aEstrella(){
		modelo = NULL;
	}

	aEstrella(Modelo* m){
		modelo = m;
	}

	int buscar(Nodo* inicial){
		
		priority_queue<Nodo*, vector<Nodo*>, comparar> nodos;
		int profundidadActual = 0;
		vector<Nodo*> sucesores;
		unordered_map<size_t, Estado*> cerrados;
		size_t hash;
		//bitset<64> estadoGoal;
		//int i = 1;
		//do{
		//	estadoGoal = estadoGoal << 4;
		//	estadoGoal = estadoGoal ^ bitset<64>(i);
		
		//i++;
		//}while(i < 16);
		//Estado15P* goal = new Estado15P(estadoGoal, bitset<4>(0));
		//goal->imprimirEstado();
		//Manhattan modelito = Manhattan(goal);
		Nodo* nodo_A_Evaluar;
		nodos.push(inicial);
		
		while(1){
			if (nodos.empty()){
				cout << "No se encontro una solucion \n";
				return 0;
			}else{

				nodo_A_Evaluar = nodos.top();
				nodos.pop();
				hash = modelo -> calcularHash(nodo_A_Evaluar -> estado);
				cerrados.insert({hash,nodo_A_Evaluar -> estado});
				if(nodo_A_Evaluar ->profundidad > profundidadActual){
					profundidadActual = nodo_A_Evaluar->profundidad;
				}
				if(modelo -> is_goal(nodo_A_Evaluar->estado)){
					cout << "Se encontro una solucion \n";
					cout << "Paso por " << nodo_A_Evaluar->costo<< " movimientos \n" ;
					delete nodo_A_Evaluar;
					while(!nodos.empty()){
						nodo_A_Evaluar = nodos.top();
						nodos.pop();
						delete nodo_A_Evaluar;
					}
					cerrados.clear();
					return 1;
				}else{
					sucesores = succ(nodo_A_Evaluar);
					//Para que no ocurra una referencia a una accion nula con el nodo inicial
					if(profundidadActual !=0){
						for(int i = 0; i < sucesores.size(); i ++){
							hash = modelo -> calcularHash(sucesores[i] -> estado);
							if(cerrados.count(hash) == 1 & sucesores[i]->costo != 0){
								delete sucesores[i];
								continue;
							}
							//Calculando la heuristica.

							sucesores[i] -> f =sucesores[i] -> costo + modelo->h(sucesores[i] -> estado);
							nodos.push(sucesores[i]);
						}

					}else{
						for(int i = 0; i < sucesores.size(); i ++){
							nodos.push(sucesores[i]);
						}
					}
					
				}
				if(nodo_A_Evaluar ->  accion != NULL){
					delete nodo_A_Evaluar;
				}
				
			}
		};
	}
};


