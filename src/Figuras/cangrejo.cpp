/*
 * cangrejo.cpp
 *
 *  Created on: 22/09/2013
 *      Author: jorge
 */

#include "cangrejo.h"
#include <cstdio>
#include <iostream>

using namespace std;

PataCangrejo::PataCangrejo(myWindow* ventana) : Figura (ventana) {
	this->ang_braz2_X=-45;
	this->ang_mano=-45;
}
PataCangrejo::~PataCangrejo() {
}
void PataCangrejo::renderizar(glm::mat4 model_matrix) {
		// primera parte del brazo
		this->renderArm(model_matrix);

		model_matrix = glm::translate(model_matrix, glm::vec3 (0.0f, 0.0f, 3.5f));
		model_matrix = glm::rotate(model_matrix, this->ang_braz2_X, glm::vec3 ( 1.0f,0.0f,0.0f));

		// segunda parte del brazo
		this->renderArm(model_matrix);

		model_matrix= glm::translate(model_matrix, glm::vec3 (0.0f, 0.0f, 3.5f));
		model_matrix = glm::rotate(model_matrix, this->ang_mano, glm::vec3 (1.0f,0.0f,0.0f));

		// mano del brazo 1
		this->renderArm(model_matrix);


}

CabezaCangrejo::CabezaCangrejo(myWindow* ventana) : Figura (ventana) {
	this->mi_superficie = new Esfera (ventana, 1.0, 32, 32);
}
CabezaCangrejo::~CabezaCangrejo() {
	delete this->mi_superficie;
}
void CabezaCangrejo::renderizar(glm::mat4 model_matrix) {
	glm::vec3 translado(0, 1.2, 0);
	model_matrix = glm::translate(model_matrix, translado);

	glm::vec3 escala(0.5, 0.5, 0.5);
	model_matrix = glm::scale(model_matrix, escala);
	this->mi_superficie->render(model_matrix);
}

BrazoCangrejo::BrazoCangrejo(myWindow* ventana) : Figura (ventana) {
	ang_braz2_X=0;
	ang_braz2_Z=0;

	ang_mano=0;
	ang_dedos_X=20;
}
BrazoCangrejo::~BrazoCangrejo() {
}
void BrazoCangrejo::renderizar(glm::mat4 model_matrix) {
	// primera parte del brazo
	this->renderArm(model_matrix);

	model_matrix = glm::translate(model_matrix, glm::vec3 (0.0f, 0.0f, 3.5f));
	model_matrix = glm::rotate(model_matrix, this->ang_braz2_X, glm::vec3 (1.0f,0.0f,0.0f));
	model_matrix = glm::rotate(model_matrix, this->ang_braz2_Z, glm::vec3 ( 0.0f,0.0f,1.0f));

	// segunda parte del brazo
	this->renderArm(model_matrix);

	model_matrix= glm::translate(model_matrix, glm::vec3 (0.0f, 0.0f, 3.5f));
	model_matrix = glm::rotate(model_matrix, this->ang_dedos_X, glm::vec3 (1.0f, 0.0f,0.0f));

	// pinza1
	glm::mat4 rot3 = glm::rotate(glm::mat4 (1.0f), this->ang_mano, glm::vec3 (0.0f, 0.0f,1.0f));
	this->renderArm( model_matrix * rot3 );

	// pinza2
	glm::mat4 rot4 = glm::rotate(glm::mat4 (1.0f), -this->ang_dedos_X, glm::vec3 (1.0f, 0.0f, 0.0f));
	this->renderArm( model_matrix * rot4 );

}

TorsoCangrejo::TorsoCangrejo(myWindow* ventana) : Figura (ventana) {
	this->mi_superficie = new Esfera (ventana, 1.0, 32, 32);
}
TorsoCangrejo::~TorsoCangrejo() {
	delete this->mi_superficie;
}
void TorsoCangrejo::renderizar(glm::mat4 model_matrix) {
	glm::vec3 escala(1, 1.1, 0.8);
	model_matrix = glm::scale(model_matrix, escala);
	this->mi_superficie->render(model_matrix);
}

Cangrejo::Cangrejo(myWindow* ventana) : Figura (ventana),
		pata1(ventana), pata2(ventana), pata3(ventana), pata4(ventana), pata5(
				ventana), pata6(ventana), cabeza(ventana), torso(ventana), brazo1(
				ventana), brazo2(ventana) {

	this->patas.push_back(&pata1);
	this->patas.push_back(&pata2);
	this->patas.push_back(&pata3);
	this->patas.push_back(&pata4);
	this->patas.push_back(&pata5);
	this->patas.push_back(&pata6);

	this->window=NULL;

	ang_pata_X[0]=-95;
	ang_pata_X[1]=-45;
	ang_pata_X[2]=-95;
	ang_pata_X[3]=-95;
	ang_pata_X[4]=-45;
	ang_pata_X[5]=-95;

	ang_pata_Z[0]=90;
	ang_pata_Z[1]=90;
	ang_pata_Z[2]=90;
	ang_pata_Z[3]=-90;
	ang_pata_Z[4]=-90;
	ang_pata_Z[5]=-90;

	patas_elevacion[0]=false;
	patas_elevacion[1]=false;
	patas_elevacion[2]=false;
	patas_elevacion[3]=false;
	patas_elevacion[4]=false;
	patas_elevacion[5]=false;
}
Cangrejo::~Cangrejo() {
}
void Cangrejo::renderizar(glm::mat4 model_matrix) {
	this->torso.renderizar(model_matrix);
	this->cabeza.renderizar(model_matrix);

	model_matrix = glm::scale(model_matrix, glm::vec3 (0.3f,0.3f,0.3f));

	float ang_braz1_X = -90;
	float ang_braz1_Z = -45;
	glm::mat4 m_brazo1  = glm::translate(model_matrix , glm::vec3 (2.0f, 3.0f, 0.0f));
	m_brazo1  = glm::rotate(m_brazo1 , ang_braz1_Z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_brazo1  = glm::rotate(m_brazo1 , ang_braz1_X, glm::vec3(1.0f, 0.0f, 0.0f));

	this->brazo1.renderizar(m_brazo1);

	float ang_braz2_X = -90;
	float ang_braz2_Z = 45;
	glm::mat4 m_brazo2  = glm::translate(model_matrix , glm::vec3 (-2.0f, 3.0f, 0.0f));
	m_brazo2  = glm::rotate(m_brazo2 , ang_braz2_Z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_brazo2  = glm::rotate(m_brazo2 , ang_braz2_X, glm::vec3(1.0f, 0.0f, 0.0f));
	this->brazo2.renderizar(m_brazo2);

	// Achico un poco para q las patas sean mas chicas que los brazos
	model_matrix = glm::scale(model_matrix, glm::vec3 (0.8f,0.8f,0.8f));

	// Dibujo las patas
	glm::mat4 m_pata;
	for (int i=0;i<6;++i) {
		if(i<3){
			m_pata = glm::translate(model_matrix , glm::vec3 (-3.5f , 1.0f - (2*i), 0.0f));
		}else{
			m_pata = glm::translate(model_matrix , glm::vec3 (3.5f , 1.0f - (2*(i-3)), 0.0f));
		}
		m_pata  = glm::rotate(m_pata , ang_pata_Z[i], glm::vec3(0.0f, 0.0f, 1.0f));
		m_pata  = glm::rotate(m_pata , ang_pata_X[i], glm::vec3(1.0f, 0.0f, 0.0f));
		this->patas[i]->renderizar(m_pata);
	}
}
void Cangrejo::animar() {
	std::cout<<"me estan animando"<<std::endl;
	for (unsigned int i=0;i<6;++i) {
		if ( patas_elevacion[i] == true ) {
			ang_pata_X[i]+=1;
			if ( ang_pata_X[i]>=-45 ) {
				patas_elevacion[i]=false;
			}
		}

		if ( patas_elevacion[i] == false ) {
			ang_pata_X[i]-=1;
			if ( ang_pata_X[i]<=-95 ){
				patas_elevacion[i]=true;
			}
		}
	}
}
