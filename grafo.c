#include "grafo.h"

grafo make_root_node(int estado1, int estado2){
	grafo nuevo_grafo;
	nuevo_grafo->estado1 = estado1;
	nuevo_grafo->estado2 = estado2;
	nuevo_grafo->accion = '';
	nuevo_grafo->costo = 0;
	nuevo_grafo->padre = NULL;
	return nuevo_grafo; 
}

grafo make_node(grafo padre, char accion, int estado1, int estado2){
	grafo nuevo_grafo;
	nuevo_grafo->estado1 = estado1;
	nuevo_grafo->estado2 = estado2;
	nuevo_grafo->accion = accion;
	nuevo_grafo->costo = padre->costo + 1;
	nuevo_grafo->padre = padre;
	return nuevo_grafo;
}