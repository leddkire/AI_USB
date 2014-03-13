#include <stdlib.h>

struct nodo
{
	int estado1;
	int estado2;
	char accion;
	int costo;
	struct nodo *padre;

};
typedef struct nodo *grafo;

extern grafo make_root_node(int, int);
extern grafo make_node(grafo, char, int, int);