#ifndef MODELO
#define MODELO

#include <vector>
using namespace std;

class Accion{
public:
	virtual ~Accion() =0;

};
Accion::~Accion(){}

class Estado{
	
public:
	virtual ~Estado() =0;
	virtual int costo(Accion* a) = 0;

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


class Modelo{
	public:
		virtual ~Modelo() =0;
		virtual Estado* init() = 0;
		virtual bool is_goal(Estado* s) = 0;
		virtual vector<ParEstadoAccion> succ(Estado* s) = 0;
		virtual Estado* operar(Estado* s, Accion* a) = 0;
		virtual bool esAccionInversa(Accion* anterior, Accion* aEvaluar);		
};

Modelo::~Modelo(){}
#endif



