#include <cstddef>
#include "AlgoritmoBusqueda.cpp"
#include <queue>
#include <climits>

#include <iostream>

using namespace std;

class idaEstrella:public AlgoritmoBusqueda{
public:

	idaEstrella(){
		modelo = Modelo15P();
	}

	idaEstrella(Modelo15P m){
		modelo = m;
	}

	int buscar(Nodo* inicial){
		int limite = modelo.h(inicial -> getEstado(), inicial -> getUbicacion0());
		int t;
		int generados = 0;


		while(1){
			t = busquedaProfundidad(inicial,limite,&generados);
			if(t==-1){
				return generados; //Se encontro una solucion
			}else if(t == INT_MAX){
				return generados; //No se encontro una solucion
			}
			limite = t;
		}
		return 0;
	}

	int busquedaProfundidad(Nodo* nod,int l,int* generados){
		vector<Nodo*> sucesores;
		int encontrado;
		int resultado;
		int f;
		int hh;

		hh = modelo.h(nod -> getEstado(), nod -> getUbicacion0());

		f = nod -> getCosto().to_ulong() + hh;
		
		
		if(f > l){
			delete nod;
			nod = NULL;
			return f;
		}
		if(modelo.is_goal(nod -> getEstado())){
			// cout << "Profundidad: " << nod -> costo << "\n";
			cout << *generados << "," << nod -> getCosto().to_ulong() << ",";
			return -1;
		}
		int min = INT_MAX;
		sucesores = succ(nod);
		*generados = *generados + sucesores.size();
		for(int i = 0; i < sucesores.size();i++){
			if(nod -> getProf() != 0){
				if(!(modelo.esAccionInversa(nod -> getAccion(), sucesores[i] -> getAccion()))){
					resultado = busquedaProfundidad(sucesores[i], l, generados);
					if(resultado == -1){
						return -1;
					}
					if(resultado < min){
						min = resultado;
					}
				}else{
					delete sucesores[i];
				}
			}else{
				resultado = busquedaProfundidad(sucesores[i], l, generados);
				if(resultado == -1){
					return -1;
				}
				if(resultado < min){
					min = resultado;
				}
			}
			
			
		}

		if(nod -> getProf() != 0){
			delete nod;
			nod = NULL;
		}
		return min;

	}
};