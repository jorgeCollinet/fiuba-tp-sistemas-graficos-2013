/*
 * Piedra.cpp
 *
 *  Created on: 08/10/2013
 *      Author: jorge
 */

#include "piedra.h"
#include <vector>
#include "Helper.h"

Piedra::Piedra(myWindow* ventana) : Figura (ventana) {
	this->mi_superficie = this->crear_superficie (ventana);
	this->mi_superficie->nombreTextura="roca.jpg";
	//this->mi_superficie->nombreTexturaNormal="rocaNormal.jpg";
}
Piedra::~Piedra() { }

void Piedra::renderizar(glm::mat4 model_matrix) {
	model_matrix = glm::scale(model_matrix,glm::vec3(0.8,0.8,0.8));
	Figura::renderizar(model_matrix);
}

Curva* Piedra::crear_curva_trayectoria () {
	std::vector<glm::vec3> control_trayectoria;
	
	float numX = Helper::num_aleatorio (0.8, 1.4);
	float numZ = Helper::num_aleatorio (0.40, 0.60);
	float coefZ_inicial = Helper::num_aleatorio (0.0, 1.5);
	float coefZ_final = Helper::num_aleatorio (0.0, 1.5);
	control_trayectoria.push_back ( glm::vec3 ( -numX, 0.0, numZ * coefZ_inicial) );
	control_trayectoria.push_back ( glm::vec3 ( 0.0, 0.0, numZ) );
	control_trayectoria.push_back ( glm::vec3 ( numX, 0.0, numZ * coefZ_final) );
	return (new CurvaBezier (control_trayectoria, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
}

Curva* Piedra::crear_curva_seccion () {
	unsigned int cantPuntos = 8;
	float radio = 0.75;
	float PI = 3.1415927f;
	std::vector<glm::vec3> control_seccion;
	for (unsigned int puntoNumero = 0 ; puntoNumero < cantPuntos ; puntoNumero++) {
		float angulo = puntoNumero * 2.0 * PI / cantPuntos;
		float coef = Helper::num_aleatorio (0.80, 1.35);
		control_seccion.push_back ( glm::vec3 (radio * std::cos(angulo) * coef, radio * std::sin(angulo) * coef, 0.0) );
	}
	// completo el ultimo tramo
	control_seccion.push_back (control_seccion.at(0));
	control_seccion.push_back (control_seccion.at(1));
	control_seccion.push_back (control_seccion.at(2));
	
	return (new CurvaBSpline (control_seccion, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
}

int Piedra::obtener_pasos_trayectoria () {
	return 100;
}

int Piedra::obtener_pasos_seccion () {
	return 100;
}

FuncionCurvaBezier Piedra::crear_funcion () {
	std::vector<glm::vec3> puntos;
	puntos.push_back ( glm::vec3 (0.0, 0.0, 0.0) );
	puntos.push_back ( glm::vec3 (0.005 * this->obtener_pasos_trayectoria(), 3.3, 0.0) );
	puntos.push_back ( glm::vec3 (0.5 * this->obtener_pasos_trayectoria(), 1.0, 0.0) );
	puntos.push_back ( glm::vec3 (0.99 * this->obtener_pasos_trayectoria(), 2.9, 0.0) );
	puntos.push_back ( glm::vec3 (this->obtener_pasos_trayectoria(), 0.0, 0.0) );
	return (FuncionCurvaBezier(puntos));
}

std::vector<glm::mat4> Piedra::crear_transformaciones () {
	std::vector<glm::mat4> transformaciones;
	
	FuncionCurvaBezier funcion = this->crear_funcion();
	int pasosTrayectoria = this->obtener_pasos_trayectoria();
	for (int i = 0 ; i <= pasosTrayectoria ; i++) {
		float coef = 0.0;
		if ((i > 0) && (i < pasosTrayectoria)) coef = funcion.evaluar_en (i * 1.0);
		if (coef < 0.0) coef *= (-1);
		glm::mat4 matriz = glm::scale ( glm::mat4 (1.0f) , glm::vec3 (coef, coef, coef));
		transformaciones.push_back(matriz);
	}
	return transformaciones;
}
