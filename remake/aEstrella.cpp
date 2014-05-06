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
		
		priority_queue<Nodo*, vector<Nodo*>, comparar15P> nodos;
		int profundidadActual = 0;
		int profundidad = 0;
		vector<Nodo*> sucesores;
		unordered_map<size_t, bitset<1>> cerrados;
		size_t hash;
		int generados = 0;
		bitset<1> bit0 = bitset<1>(0);
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
		hash = modelo -> calcularHash(inicial -> estado);
		cerrados.insert({hash,bit0});
		
		while(1){
			if (nodos.empty()){
				cerr << "No se encontro una solucion \n";
				return 0;
			}else{

				nodo_A_Evaluar = nodos.top();
				nodos.pop();
				profundidad = nodo_A_Evaluar ->getProfundidad();
				if( profundidad > profundidadActual){
					profundidadActual = profundidad;
					cerr << profundidadActual << "\n";
				}
				if(modelo -> is_goal(nodo_A_Evaluar->estado)){
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
							hash = modelo -> calcularHash(sucesores[i] -> estado);
							if(cerrados.count(hash) == 1 & sucesores[i]->getCosto() != 0){
								delete sucesores[i] -> estado;
								delete sucesores[i] -> accion;
								sucesores[i] -> padre = NULL;
								sucesores[i] -> estado=NULL;
								sucesores[i] -> accion = NULL;
								delete sucesores[i];
								continue;
							}
							//Calculando la heuristica.
							
							sucesores[i]-> asignarF(modelo);
							nodos.push(sucesores[i]);
							cerrados.insert({hash,bit0});
						}

					}else{
						for(int i = 0; i < sucesores.size(); i ++){
							sucesores[i]-> asignarF(modelo);
							nodos.push(sucesores[i]);
							hash = modelo -> calcularHash(nodo_A_Evaluar -> estado);
							cerrados.insert({hash,bit0});
						}
					}
					
				}
				if(nodo_A_Evaluar ->  accion != NULL){
					delete nodo_A_Evaluar -> estado;
					delete nodo_A_Evaluar -> accion;
					nodo_A_Evaluar -> padre = NULL;
					nodo_A_Evaluar -> estado=NULL;
					nodo_A_Evaluar -> accion = NULL;
					delete nodo_A_Evaluar;
				}
				
			}
		};
	}
};


