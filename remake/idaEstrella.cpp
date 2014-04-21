#include <cstddef>
#include "AlgoritmoBusqueda.cpp"
#include <queue>
#include <climits>

class idaEstrella:public AlgoritmoBusqueda{
public:
	idaEstrella(){
		modelo = NULL;
	}

	idaEstrella(Modelo* m){
		modelo = m;
	}

	int buscar(Nodo inicial){
		int profundidad = 0;
		int resultado = 0;
		while(1){
			resultado = busquedaProfundidad(inicial,profundidad);
			if(resultado){
				return resultado;
			}
			profundidad++;
		}
		return 0;
	}

	int busquedaProfundidad(Nodo nod,int p){
		vector<Nodo> sucesores;
		int encontrado;
		int resultado;
		if(nod.profundidad > p){
			return 0;
		}
		if(modelo -> is_goal(nod.estado)){
			return 1;
		}
		int min = INT_MAX;
		sucesores = succ(nod);
		for(int i = 0; i < sucesores.size();i++){
			resultado = busquedaProfundidad(sucesores[i], p);
			if(resultado){
				return resultado;
			}
			if(resultado < min){
				min = resultado;
			}
		}
		return resultado;

	}
};