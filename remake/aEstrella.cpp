//Algoritmo A*
#include <cstddef>

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
		Nodo* nodo_A_Evaluar;
		nodos.push(inicial);
		
		while(1){
			if (nodos.empty()){
				cout << "No se encontro una solucion \n";
				return 0;
			}else{

				nodo_A_Evaluar = nodos.top();
				nodos.pop();
				if(nodo_A_Evaluar ->profundidad > profundidadActual){
					profundidadActual = nodo_A_Evaluar->profundidad;
					cout << "En profundidad " << profundidadActual << "\n";
				}
				if(modelo -> is_goal(nodo_A_Evaluar->estado)){
					cout << "Se encontro una solucion \n";
					cout << "Paso por " << nodo_A_Evaluar->profundidad << " movimientos \n" ;
					return 1;
				}else{
					sucesores = succ(nodo_A_Evaluar);
					//Para que no ocurra una referencia a una accion nula.
					if(profundidadActual !=0){
						for(int i = 0; i < sucesores.size(); i ++){
							if(modelo -> esAccionInversa(nodo_A_Evaluar->accion,sucesores[i]->accion) & sucesores[i]->costo != 0){
								continue;
							}
							nodos.push(sucesores[i]);
						}

					}else{
						for(int i = 0; i < sucesores.size(); i ++){
							nodos.push(sucesores[i]);
						}
					}
					
				}
			}
		};
	}
};


