/*
 * figura.h
 *
 *  Created on: 20/09/2013
 *      Author: jorge
 */

#ifndef FIGURA_H_
#define FIGURA_H_

#include "myWindow.h"
#include <vector>
#include <glm/glm.hpp>
#include "Superficie.h"
#include "Grilla.h"
#include "Esfera.h"
#include "SuperficieBarrido.h"
#include "CurvaBezier.h"
#include "CurvaBSpline.h"
#include "Circunferencia.h"
#include "Parabola.h"
#include "Funcion.h"

class Figura {
protected:
	Superficie* mi_superficie;
	std::vector <Figura*> figuras_hijas;
	
	// temporal
	Superficie* mySphere;
	Superficie* myCube;
	void renderArm (glm::mat4 model_matrix);
	
public:
	Figura (myWindow* ventana);
	virtual ~Figura();
	virtual void renderizar(glm::mat4 model_matrix) = 0;
	void agregar_figura_hija(Figura& fig);
protected:
	void renderizar_figuras_hijas(glm::mat4 model_matrix);

};

#endif /* FIGURA_H_ */
