#include <cstddef>
#include "AlgoritmoBusqueda.cpp"
#include <queue>
#include <climits>

#include <iostream>

using namespace std;

class idaEstrella:public AlgoritmoBusqueda{
public:
	idaEstrella(){
		modelo = NULL;
	}

	idaEstrella(Modelo* m){
		modelo = m;
	}

	int buscar(Nodo* inicial){
		int limite = modelo -> h(inicial -> estado);
		int t;
		while(1){
			t = busquedaProfundidad(inicial,limite);
			if(t==-1){
				return -1; //Se encontro una solucion
			}else if(t == INT_MAX){
				return -2; //No se encontro una solucion
			}
			limite = t;
		}
		return 0;
	}

	int busquedaProfundidad(Nodo* nod,int l){
		vector<Nodo*> sucesores;
		int encontrado;
		int resultado;
		int f;

		f = nod -> costo + modelo -> h(nod -> estado);
		
		if(f > l){
			delete nod;
			return f;
		}
		if(modelo -> is_goal(nod -> estado)){
			return -1;
		}
		int min = INT_MAX;
		sucesores = succ(nod);
		for(int i = 0; i < sucesores.size();i++){
			if(nod -> accion != NULL){
				if(!(modelo -> esAccionInversa(nod -> accion, sucesores[i] -> accion))){
					resultado = busquedaProfundidad(sucesores[i], l);
					if(resultado == -1){
						sucesores.clear();
						vector<Nodo*>().swap(sucesores);
						return -1;
					}
					if(resultado < min){
						min = resultado;
					}
				}
			}else{
				resultado = busquedaProfundidad(sucesores[i], l);
				if(resultado == -1){
					sucesores.clear();
					vector<Nodo*>().swap(sucesores);
					return -1;
				}
				if(resultado < min){
					min = resultado;
				}
			}
			
			
		}
		
		
		return min;

	}
};