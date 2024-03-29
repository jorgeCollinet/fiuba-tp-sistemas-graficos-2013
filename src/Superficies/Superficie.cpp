#include "Superficie.h"
#include "myWindow.h"
#include <iostream>

void Superficie::init () {
	this->window = NULL;
	this->vertex_buffer = NULL;
	this->tangent_buffer = NULL;
	this->normal_buffer = NULL;
	this->texture_buffer = NULL;
	this->index_buffer = NULL;
	this->vertex_buffer_size = 0;
	this->tangent_buffer_size = 0;
	this->normal_buffer_size = 0;
	this->texture_buffer_size = 0;
	this->index_buffer_size = 0;
	this->modo = GL_TRIANGLE_STRIP;
	
	this->ka = glm::vec3(0.8, 0.8, 0.8);
	this->kd = glm::vec3(0.6, 0.6, 0.6);
	this->ks = glm::vec3(0.5, 0.5, 0.5);
	this->shininess = 20.0;
	this->nombreTextura="defaultTexture.jpg";
	this->nombreTexturaNormal="";
	
	this->debeAnimarse = false;
	this->tiempo = 0;
	this->amplitud = 0;
	this->numeroOnda = 0;
	this->velocidad = 0;
	
	this->reflexionActivada = false;
}

Superficie::Superficie () {
	this->init();
}

Superficie::Superficie (myWindow* passed_window) {
	this->init();
	this->window = passed_window;
}

void Superficie::render (glm::mat4 view_model_matrix) {
	if (this->window != NULL) {
		if(this->reflexionActivada){
			this->window->renderObject(view_model_matrix, vertex_buffer,tangent_buffer, normal_buffer,
			texture_buffer, nombreTextura,nombreTexturaNormal,index_buffer, index_buffer_size, modo,ka,kd,ks,shininess
			,"Peznegx.png",
			"Peznegy.png",
			"Peznegz.png",
			"Pezposx.png",
			"Pezposy.png",
			"Pezposz.png");
		}else if ((this->tangent_buffer != NULL) && (this->texture_buffer != NULL) && (this->debeAnimarse)) {
			// superficie con textura y animacion senoidal
			this->window->renderObject (view_model_matrix, vertex_buffer, tangent_buffer, normal_buffer, texture_buffer, nombreTextura,
				index_buffer, index_buffer_size, modo, ka, kd, ks, shininess, amplitud, numeroOnda, velocidad, tiempo);
		}else if ((this->tangent_buffer != NULL) && (texture_buffer != NULL) && nombreTexturaNormal!="") {
			// superficie con textura y normal map
			this->window->renderObject (view_model_matrix, vertex_buffer, tangent_buffer, normal_buffer, texture_buffer,
				nombreTextura,nombreTexturaNormal,index_buffer, index_buffer_size, modo,ka,kd,ks,shininess);
		}else if ((this->tangent_buffer != NULL) && (this->texture_buffer != NULL)) {
			// superficie con textura
			this->window->renderObject (view_model_matrix, this->vertex_buffer, this->tangent_buffer, this->normal_buffer, this->texture_buffer,
										this->nombreTextura, this->index_buffer, this->index_buffer_size, this->modo, ka,kd,ks,shininess);
		} else {
			// superficie comun
			this->window->renderObject (view_model_matrix, this->vertex_buffer, this->normal_buffer,
										this->index_buffer, this->index_buffer_size, this->modo);
		}
	}
}

void Superficie::set_parametros_animacion (float _amplitud, float _numeroOnda, float _velocidad) {
	this->debeAnimarse = true;
	this->amplitud = _amplitud;
	this->numeroOnda = _numeroOnda;
	this->velocidad = _velocidad;
}

void Superficie::animar () {
	++(this->tiempo);
}

Superficie::~Superficie () {
}
