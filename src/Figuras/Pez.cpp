/*
 * Pez.cpp
 *
 *  Created on: 12/10/2013
 *      Author: jorge
 */

#include "Pez.h"
#include <vector>


// ******************************* ALETA *******************************
AletaPez::AletaPez(myWindow* ventana) :Figura(ventana) {
	this->mi_superficie = this->crear_superficie (ventana);
}
AletaPez::~AletaPez() { }
void AletaPez::renderizar(glm::mat4 model_matrix) {
	model_matrix = glm::scale(model_matrix,glm::vec3(0.5,0.5,1.5));
	//Figura::renderizar(model_matrix);
}
// PARA HACER !!!
Curva* AletaPez::crear_curva_trayectoria () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
Curva* AletaPez::crear_curva_seccion () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
int AletaPez::obtener_pasos_trayectoria () {
	return 500;
}
int AletaPez::obtener_pasos_seccion () {
	return 500;
}
std::vector<glm::mat4> AletaPez::crear_transformaciones () {
	return (std::vector<glm::mat4>());
}

// TEMPORAL!!!
Superficie* AletaPez::crear_superficie (myWindow* ventana) {
	return (new Cubo (ventana));
}
// ******************************* COLA ********************************
ColaPez::ColaPez(myWindow* ventana) :Figura(ventana) {
	this->mi_superficie = this->crear_superficie (ventana);
}
ColaPez::~ColaPez() { }
void ColaPez::renderizar(glm::mat4 model_matrix) {

}

// PARA HACER !!!
Curva* ColaPez::crear_curva_trayectoria () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
Curva* ColaPez::crear_curva_seccion () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
int ColaPez::obtener_pasos_trayectoria () {
	return 500;
}
int ColaPez::obtener_pasos_seccion () {
	return 500;
}
std::vector<glm::mat4> ColaPez::crear_transformaciones () {
	return (std::vector<glm::mat4>());
}

// TEMPORAL!!!
Superficie* ColaPez::crear_superficie (myWindow* ventana) {
	return (new Cubo (ventana));
}

// ******************************* TORSO *******************************
TorsoPez::TorsoPez(myWindow* ventana) :Figura(ventana) {
	this->mi_superficie = this->crear_superficie (ventana);
}
TorsoPez::~TorsoPez() { }
void TorsoPez::renderizar(glm::mat4 model_matrix) {

	//model_matrix = glm::rotate(model_matrix, 45.0f, glm::vec3(0.0,1.0,0.0));
	//model_matrix = glm::rotate(model_matrix, 90.0f, glm::vec3(1.0,0.0,0.0));

	model_matrix = glm::scale(model_matrix,glm::vec3(1.5,0.5,0.5));
	Figura::renderizar(model_matrix);
}
// PARA HACER !!!
Curva* TorsoPez::crear_curva_trayectoria () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
Curva* TorsoPez::crear_curva_seccion () {
	return (new Curva(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,1.0)));
}
int TorsoPez::obtener_pasos_trayectoria () {
	return 500;
}
int TorsoPez::obtener_pasos_seccion () {
	return 500;
}
std::vector<glm::mat4> TorsoPez::crear_transformaciones () {
	return (std::vector<glm::mat4>());
}

// TEMPORAL!!!
Superficie* TorsoPez::crear_superficie (myWindow* ventana) {
	return (new Cubo (ventana));
}

// ******************************** PEZ ********************************
Pez::Pez(myWindow* ventana) :
		Figura(ventana), aleta1(ventana), aleta2(ventana), cola(ventana), torso(ventana), m_pos(0,0,0) {
	trayecto=NULL;
	cont=0;
	angX=0;
	angZ=0;
	angY=0;
	angXViejo=0;
	angZViejo=0;
	angYViejo=0;
	this->mi_superficie = this->crear_superficie (ventana);
}
Pez::~Pez() {}
void Pez::renderizar(glm::mat4 model_matrix) {
	model_matrix = glm::translate(model_matrix,m_pos);

	// OJO: hay dos rotaciones desavilitadas !!!
	//model_matrix = glm::rotate(model_matrix,angZ,glm::vec3(1,0,0));
	model_matrix = glm::rotate(model_matrix,angX,glm::vec3(0,0,1));
	//model_matrix = glm::rotate(model_matrix,angY,glm::vec3(0,1,0));

	aleta1.renderizar(model_matrix);
	aleta2.renderizar(model_matrix);
	cola.renderizar(model_matrix);

	torso.renderizar(model_matrix);
	// dibujo trayecto
	float cant_tramos = trayecto->cantidad_tramos();
	for (float i = 0; i < cant_tramos; i += 0.1) {
		glm::mat4 model_aux(1.0f);
		model_aux = glm::translate(model_aux, trayecto->damePunto(i));
		model_aux = glm::scale(model_aux, glm::vec3(0.1f, 0.1f, 0.1f));
		this->mySphere->render(model_aux);
	}

}
void Pez::set_trayecto(std::vector<glm::vec3> puntos){
	glm::vec3 centro (1,1,1);
	glm::vec3 orientacion (1,1,1);
	trayecto = new CurvaBSpline(puntos,centro,orientacion);
}
void Pez::animar() {
	if (trayecto == NULL) {
		std::cout << "TRAYECTO NO INICIADO";
		return;
	}
	std::cout << "Cantidad de tramos: " << trayecto->cantidad_tramos() << " "<< std::endl;
	glm::vec3 a = trayecto->dameTangente(cont, 0.001f);
	glm::vec3 aN = glm::normalize (a);
	glm::vec3 ProyAZ = glm::normalize( glm::dot(aN, glm::vec3(0, 0, 1))* a );
	glm::vec3 ProyAY = glm::normalize( glm::dot(aN, glm::vec3(0, 1, 0))* a );
	glm::vec3 ProyAX = glm::normalize( glm::dot(aN, glm::vec3(1, 0, 0))* a );
	
	// cont es el u , y avanzo al siguiente paso
	cont += 0.01;
	if (cont > trayecto->cantidad_tramos()) {
		cont = 0;
		angZ = 0;
		angX = 0;
		angY = 0;
		m_pos = trayecto->damePunto(cont);
		return;
	}
	glm::vec3 b = trayecto->dameTangente(cont, 0.001f);
	glm::vec3 bN = glm::normalize (b);
	glm::vec3 ProyBZ = glm::normalize( glm::dot(bN, glm::vec3(0, 0, 1))* b);
	glm::vec3 ProyBY = glm::normalize( glm::dot(bN, glm::vec3(0, 1, 0))* b);
	glm::vec3 ProyBX = glm::normalize( glm::dot(bN, glm::vec3(1, 0, 0))* b);

	std::cout<<"vector a: "<<a.x<<" , "<<a.y<<" , "<<a.z<<std::endl;
	std::cout<<"vector b: "<<b.x<<" , "<<b.y<<" , "<<b.z<<std::endl;

	//std::cout<<"vector PaZ: "<<ProyAZ.x<<" , "<<ProyAZ.y<<" , "<<ProyAZ.z<<std::endl;
	//std::cout<<"vector PbZ: "<<ProyBZ.x<<" , "<<ProyBZ.y<<" , "<<ProyBZ.z<<std::endl;

	std::cout<<"vector PaX: "<<ProyAX.x<<" , "<<ProyAX.y<<" , "<<ProyAX.z<<std::endl;
	std::cout<<"vector PbX: "<<ProyBX.x<<" , "<<ProyBX.y<<" , "<<ProyBX.z<<std::endl;

	// asigno siguiente posicion al pez
	glm::vec3 nueva_pos = trayecto->damePunto(cont);
	m_pos = nueva_pos;
	std::cout << "trayecto:" << nueva_pos.x << " , " << nueva_pos.y << " , "<< nueva_pos.z << std::endl;

	// calculo del angulo a sumar

	//OJO este eje FALLUTA , mirar lo que hace renderizar ignorando el angZ
	float radianesZ = acos(glm::dot(ProyAZ, ProyBZ));
	std::cout << "giro en grados: "<<(radianesZ * 360) / (2 * 3.141592654f)<<std::endl;
	if(radianesZ<0 || radianesZ >0){
		angZ += (radianesZ * 360) / (2 * 3.141592654f);
	}
	std::cout << "angulo de giroZ: " << angZ << std::endl;


	float radianesX = acos(glm::dot(ProyAX, ProyBX));
	if(radianesX<0 || radianesX>0){
		float angXNuevo = (radianesX * 360) / (2 * 3.141592654f);
		angX+=angXNuevo;
		std::cout << "angulo de NUEVOX: " << angXNuevo << std::endl;
	}
	std::cout << "angulo de giroX: " << angX << std::endl;

	//OJO este eje FALLUTA , mirar lo que hace renderizar ignorando el angY
	float radianesY = acos(glm::dot(ProyAY, ProyBY));
	if (radianesY < 0 || radianesY > 0) {
		angY += (radianesY * 360) / (2 * 3.141592654f);
	}
	std::cout << "angulo de giroY: " << angY << std::endl;


}
