#ifndef MODELO
#define MODELO

#include <vector>
using namespace std;

class Accion{
public:
	virtual inline ~Accion() =0;

	virtual void imprimirAccion() = 0;
};
Accion::~Accion(){}

class Estado{
	
public:
	virtual inline ~Estado() =0;
	virtual int costo(Accion* a) = 0;

	virtual void imprimirEstado() = 0;

};
Estado::~Estado(){}
class ParEstadoAccion{
	
public:
	Accion* a;
	Estado* s;

	ParEstadoAccion(Accion* ac, Estado* st){
		a = ac;
		s = st;

	}

};

class Heuristica{

public:
	virtual inline ~Heuristica() =0;
	virtual void generarHeuristicas(Estado* inicial) =0;
};
Heuristica::~Heuristica(){}

class Modelo{

	public:
		virtual inline ~Modelo() =0 ;
		virtual Estado* init() = 0;
		virtual int heuristicaEstado(Estado* s) = 0;
		virtual bool is_goal(Estado* s) = 0;
		virtual vector<ParEstadoAccion> succ(Estado* s) = 0;
		virtual Estado* operar(Estado* s, Accion* a) = 0;
		virtual int h(Estado* s) = 0;
		virtual bool esAccionInversa(Accion* anterior, Accion* aEvaluar) = 0;
		virtual size_t calcularHash(Estado* s) = 0;
		virtual void insertarCerrado(Estado* s) = 0;
		virtual int estaCerrado(Estado* s) = 0;		
};

Modelo::~Modelo(){}
#endif



