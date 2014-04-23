//Algoritmo A*
#include <cstddef>
#include <unordered_map>

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
					cout << "En profundidad " << profundidadActual << "\n";
				}
				if(modelo -> is_goal(nodo_A_Evaluar->estado)){
					cout << "Se encontro una solucion \n";
					cout << "Paso por " << nodo_A_Evaluar->profundidad << " movimientos \n" ;
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
							sucesores[i] -> f =sucesores[i] -> f + modelo -> h(sucesores[i] -> estado);
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


