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

	aEstrella(Modelo15P m){
		modelo = m;
	}

	int buscar(Nodo* inicial){
		
		priority_queue<Nodo*, vector<Nodo*>, comparar> nodos;
		int profundidadActual = 0;
		int profundidad = 0;
		vector<Nodo*> sucesores;
		unordered_map<size_t, bool> cerrados;
		size_t hash;
		int generados = 0;
		bool bit0 = false;
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
		hash = modelo.calcularHash(inicial -> getEstado());
		cerrados.insert({hash,bit0});
		
		while(1){
			if (nodos.empty()){
				cerr << "No se encontro una solucion \n";
				return 0;
			}else{

				nodo_A_Evaluar = nodos.top();
				nodos.pop();
				profundidad = nodo_A_Evaluar ->profundidad;
				if( profundidad > profundidadActual){
					profundidadActual = profundidad;
					cerr << profundidadActual << "\n";
				}
				if(modelo.is_goal(nodo_A_Evaluar->estado)){
					// cout << "Se encontro una solucion \n";
					// cout << "Paso por " << nodo_A_Evaluar->costo<< " movimientos \n" ;
					cout << generados << " " << nodo_A_Evaluar->getCosto()<< " ";
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
					generados = generados + sucesores.size();
					//Para que no ocurra una referencia a una accion nula con el nodo inicial
					if(profundidadActual !=0){
						for(int i = 0; i < sucesores.size(); i ++){
							hash = modelo.calcularHash(sucesores[i] -> getEstado());
							if(cerrados.count(hash) == 1 & sucesores[i]->getCosto() != 0){
								sucesores[i] -> padre = NULL;
								delete sucesores[i];
								continue;
							}
							//Calculando la heuristica.
							
							sucesores[i]-> f = sucesores[i] -> getCosto() + modelo.h(sucesores[i] -> getEstado(), sucesores[i] -> getUbicacion0()) ;
							nodos.push(sucesores[i]);
							cerrados.insert({hash,bit0});
						}

					}else{
						for(int i = 0; i < sucesores.size(); i ++){
							sucesores[i]-> setF(bitset<8>(sucesores[i] -> getCosto() + modelo.h(sucesores[i] -> getEstado(), sucesores[i] -> getUbicacion0()))) ;
							nodos.push(sucesores[i]);
							hash = modelo.calcularHash(nodo_A_Evaluar -> getEstado());
							cerrados.insert({hash,bit0});
						}
					}
					
				}
				if(nodo_A_Evaluar ->  getProf() != 0){
					nodo_A_Evaluar -> padre = NULL;
					delete nodo_A_Evaluar;
				}
				
			}
		};
	}
};


