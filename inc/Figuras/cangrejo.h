/*
 * brazo_cangrejo.h
 *
 *  Created on: 20/09/2013
 *      Author: jorge
 */

#ifndef CANGREJO_H_
#define CANGREJO_H_

#include "figura.h"
#include <vector>

class PataCangrejo: public Figura {
	float ang_braz2_X;
	float ang_mano;
public:
	PataCangrejo(myWindow* ventana);
	~PataCangrejo();
	virtual void renderizar(glm::mat4 model_matrix);
};

class CabezaCangrejo: public Figura {
public:
	CabezaCangrejo(myWindow* ventana);
	~CabezaCangrejo();
	virtual void renderizar(glm::mat4 model_matrix);
};

class BrazoCangrejo : public Figura {
protected:
	// para manejo de brazo
	float ang_braz2_X;
	float ang_braz2_Z;

	float ang_dedos_X;
	float ang_mano;
public:
	BrazoCangrejo(myWindow* ventana);
	~BrazoCangrejo();
	virtual void renderizar(glm::mat4 model_matrix);
};

class TorsoCangrejo : public Figura {
public:
	TorsoCangrejo (myWindow* ventana);
	~TorsoCangrejo();
	virtual void renderizar(glm::mat4 model_matrix);
};

class Cangrejo: public Figura {
protected:
	PataCangrejo pata1;
	PataCangrejo pata2;
	PataCangrejo pata3;
	PataCangrejo pata4;
	PataCangrejo pata5;
	PataCangrejo pata6;
	std::vector<PataCangrejo*> patas;
	CabezaCangrejo cabeza;
	TorsoCangrejo torso;
	BrazoCangrejo brazo1;
	BrazoCangrejo brazo2;
	myWindow* window;
	float ang_pata_X[6];
	float ang_pata_Z[6];
public:
	Cangrejo(myWindow* ventana);
	~Cangrejo();
	virtual void renderizar(glm::mat4 model_matrix);
	virtual void animar();
};

#endif /* CANGREJO_H_ */
