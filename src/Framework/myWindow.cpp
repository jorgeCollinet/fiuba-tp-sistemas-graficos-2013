#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>


#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp>

#include "myWindow.h"
#include "SOIL.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "figura.h"
#include "Esfera.h"
#include "Cubo.h"

void myWindow::cargarTextura(std::string nombreTextura, GLuint programShader, std::string nombreVariableUniforme) {
	std::string nombreTexturaNormal="";
	std::string nombreVUniformeNormal="";
	// Load texture file
	unsigned int textureid;
	int image_witdh;
	int image_height;
	int image_channels;
	unsigned char* image_buffer;
	// hash que hace eficiente el programa :)
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	if(this->cacheTextureId.count(nombreTextura) == 0) {
		std::ifstream ifile(nombreTextura.c_str());
		if (!ifile) {
			std::cout<<"error cargar textura: "+nombreTextura<<std::endl;
			throw std::ios_base::failure("error cargar textura"+nombreTextura); }
		glGenTextures(1, &textureid);
		glBindTexture(GL_TEXTURE_2D, textureid);
		this->cacheTextureId[nombreTextura]=textureid;
		image_buffer = SOIL_load_image(nombreTextura.c_str(), &image_witdh, &image_height, &image_channels, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_witdh, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	textureid = this->cacheTextureId[nombreTextura];
	glBindTexture(GL_TEXTURE_2D, textureid);

	// Set the Tex1 sampler uniform to refer to texture unit 0
	int Tex1 = glGetUniformLocation(programShader, nombreVariableUniforme.c_str());
	if (Tex1 >= 0) {
		// We indicate that Uniform Variable sampler2D "Tex1" uses  Texture Unit 0
		glUniform1i(Tex1, 0);
	}else{
		throw std::ios_base::failure("error al cargar textura en myWindow, metodo cargarTextura");
	}

}

void myWindow::cargarTexturaYNormal(std::string nombreTextura, GLuint programShader, std::string nombreVariableUniforme,std::string nombreTexturaNormal,std::string nombreVUniformeNormal){
	int image_witdh;
	int image_height;
	int image_channels;
	unsigned char* image_buffer;

	GLuint texIDs[2];
	if(this->cacheTextureId.count(nombreTextura) == 0){
		glGenTextures(2, texIDs);
		this->cacheTextureId[nombreTextura]=texIDs[0];
		this->cacheTextureId[nombreTexturaNormal]=texIDs[1];
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDs[0]);
		image_buffer = SOIL_load_image(nombreTextura.c_str(), &image_witdh,&image_height, &image_channels, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_witdh, image_height,0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Set the BrickTex sampler uniform to texture unit 0
		int uniloc = glGetUniformLocation(programShader, nombreVariableUniforme.c_str());
		if (uniloc >= 0){
			glUniform1i(uniloc, 0);
		}else{
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormal" <<std::endl;
			std::cout<< "No se encontro: "+nombreVariableUniforme <<std::endl;
			std::cout<< "program handle: "+programShader <<std::endl;

				throw std::ios_base::failure("");
			}

				// Copy moss texture to OpenGL
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDs[1]);
		image_buffer = SOIL_load_image(nombreTexturaNormal.c_str(), &image_witdh,&image_height, &image_channels, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_witdh, image_height,0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Set the MossTex sampler uniform to texture unit 1
		uniloc = glGetUniformLocation(programShader, nombreVUniformeNormal.c_str());
		if (uniloc >= 0){
			glUniform1i(uniloc, 1);
		}else{
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormal" <<std::endl;
			std::cout<< "No se encontro: "+nombreVUniformeNormal <<std::endl;
			throw std::ios_base::failure("error al cargar textura en myWindow, metodo cargarTextura");
		}

	}else{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->cacheTextureId[nombreTextura]);
		int uniloc = glGetUniformLocation(programShader,nombreVariableUniforme.c_str());
		if (uniloc >= 0) {
			glUniform1i(uniloc, 0);
		} else {
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormal" <<std::endl;
			std::cout<< "No se encontro: "+nombreVariableUniforme <<std::endl;
			throw std::ios_base::failure(
					"error al cargar textura en myWindow, metodo cargarTextura");
		}
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, cacheTextureId[nombreTexturaNormal]);
		uniloc = glGetUniformLocation(programShader, nombreVUniformeNormal.c_str());
		if (uniloc >= 0) {
			glUniform1i(uniloc, 1);
		} else {
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormal" <<std::endl;
			std::cout<< "No se encontro: "+nombreVUniformeNormal <<std::endl;
			throw std::ios_base::failure(
					"error al cargar textura en myWindow, metodo cargarTextura");
		}
	}
}

void myWindow::cargarTexturasReflexion(std::vector<std::string> texturas,
		GLuint programShader, std::string nombreVariableUniforme) {

	glActiveTexture(GL_TEXTURE0);
	if (this->cacheTextureId.count(texturas[0]) == 0) {
		GLuint tex_cube = SOIL_load_OGL_cubemap(texturas[0].c_str(),
				texturas[1].c_str(), texturas[2].c_str(), texturas[3].c_str(),
				texturas[4].c_str(), texturas[5].c_str(), SOIL_LOAD_RGB,
				SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		if(tex_cube==0){
			std::cout<<"ERROR CARGA DE CUBEMAP"<<std::endl;
		}
		this->cacheTextureId[texturas[0]] = tex_cube;

	}else{
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cacheTextureId[texturas[0]]);

		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int uniloc = glGetUniformLocation(programShader, nombreVariableUniforme.c_str());
		if (uniloc >= 0) {
			glUniform1i(uniloc, 0);
		} else {
			throw std::ios_base::failure(
					"error al cargar textura en myWindow, metodo cargarTexturasReflexion");
		}
	}

}

void myWindow::cargarTexturaYNormalParaReflexion(std::string nombreTextura, GLuint programShader, std::string nombreVariableUniforme,std::string nombreTexturaNormal,std::string nombreVUniformeNormal){
	int image_witdh;
	int image_height;
	int image_channels;
	unsigned char* image_buffer;

	GLuint texIDs[2];
	if ((this->cacheTextureId.count(nombreTextura) == 0) || (this->cacheTextureId.count(nombreTexturaNormal) == 0)) {
		glGenTextures(2, texIDs);
		this->cacheTextureId[nombreTextura]=texIDs[0];
		this->cacheTextureId[nombreTexturaNormal]=texIDs[1];
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDs[0]);
		image_buffer = SOIL_load_image(nombreTextura.c_str(), &image_witdh,&image_height, &image_channels, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_witdh, image_height,0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Set the Tex sampler uniform to texture unit 1
		int uniloc = glGetUniformLocation(programShader, nombreVariableUniforme.c_str());
		if (uniloc >= 0){
			glUniform1i(uniloc, 2);
		}else{
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion" <<std::endl;
			std::cout<< "no se pudo cargar por primera vez la textura: " + nombreTextura << std::endl;
			std::cout<< "No se encontro: "+nombreVariableUniforme <<std::endl;
			std::cout<< "program handle: " << programShader <<std::endl;
			throw std::ios_base::failure("");
		}
		
		// Copy normal texture to OpenGL
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDs[1]);
		image_buffer = SOIL_load_image(nombreTexturaNormal.c_str(), &image_witdh,&image_height, &image_channels, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_witdh, image_height,0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Set the normalmap sampler uniform to texture unit 2
		uniloc = glGetUniformLocation(programShader, nombreVUniformeNormal.c_str());
		if (uniloc >= 0){
			glUniform1i(uniloc, 1);
		}else{
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion" <<std::endl;
			std::cout<< "no se pudo cargar por primera vez la textura: " + nombreTexturaNormal << std::endl;
			std::cout<< "No se encontro: "+nombreVUniformeNormal <<std::endl;
			throw std::ios_base::failure("error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion");
		}

	}else{
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->cacheTextureId[nombreTextura]);
		int uniloc = glGetUniformLocation(programShader,nombreVariableUniforme.c_str());
		if (uniloc >= 0) {
			glUniform1i(uniloc, 2);
		} else {
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion" <<std::endl;
			std::cout<< "no se pudo cargar desde el cacheTexture la textura: " + nombreTextura << std::endl;
			std::cout<< "No se encontro: "+nombreVariableUniforme <<std::endl;
			throw std::ios_base::failure(
					"error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion");
		}
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, cacheTextureId[nombreTexturaNormal]);
		uniloc = glGetUniformLocation(programShader, nombreVUniformeNormal.c_str());
		if (uniloc >= 0) {
			glUniform1i(uniloc, 1);
		} else {
			std::cout<< "error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion" <<std::endl;
			std::cout<< "no se pudo cargar desde el cacheTexture la textura: " + nombreTexturaNormal << std::endl;
			std::cout<< "No se encontro: "+nombreVUniformeNormal <<std::endl;
			throw std::ios_base::failure(
					"error al cargar textura en myWindow, metodo cargarTexturaYNormalParaReflexion");
		}
	}
}

void myWindow::disableTextureUnits (int maxUnit, GLenum target0, GLenum target1, GLenum target2) {
	if (maxUnit >= 0) {
		glClientActiveTexture (GL_TEXTURE0);
		glDisableClientState (GL_TEXTURE_COORD_ARRAY);
		glActiveTexture (GL_TEXTURE0);
		glDisable (target0);
	}
	
	if (maxUnit >= 1) {
		glActiveTexture (GL_TEXTURE1);
		glDisable (target1);
	}
	
	if (maxUnit >= 2) {
		glActiveTexture (GL_TEXTURE2);
		glDisable (target2);
	}
	
	glClientActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0);
}

// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS Y MAPA DE NORMALES Y TEXTURAS DE REFLEXION
// PHONG RECIBIDO POR PARAMETRO
void myWindow::renderObject(glm::mat4 model_matrix, GLfloat* vertex_buff,
		GLfloat* tangent_buff, GLfloat* normal_buff, GLfloat* texture_buff,
		std::string nombreTextura, std::string nombreTexturaNormales,
		GLuint* index_buff, unsigned int index_buff_size, GLenum modo,
		glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shininess,
		std::string negx, std::string negy, std::string negz, std::string posx,
		std::string posy, std::string posz) {


	std::vector<std::string> texturas;
	texturas.push_back(negx);
	texturas.push_back(negy);
	texturas.push_back(negz);
	texturas.push_back(posx);
	texturas.push_back(posy);
	texturas.push_back(posz);
	
	glClientActiveTexture (GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_SAMPLER_CUBE_MAP_ARRAY);
	cargarTexturasReflexion(texturas,this->programHandlePhongAndTextureAndNormalMapAndReflection,"CubeMapTex");
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, tangent_buff);
	cargarTexturaYNormalParaReflexion(nombreTextura, this->programHandlePhongAndTextureAndNormalMapAndReflection,"Tex1",nombreTexturaNormales,"NormalMapTex");
	
	float reflectionFactor = 0.2;
	GLuint location_reflectionFactor = glGetUniformLocation(this->programHandlePhongAndTextureAndNormalMapAndReflection, "ReflectFactor");
	if (location_reflectionFactor >= 0) {
		glUniform1fv(location_reflectionFactor, 1, &reflectionFactor);
	}else {throw std::ios_base::failure("Error en Binding: ReflectFactor");}

	GLuint location_WorldCameraPosition = glGetUniformLocation(this->programHandlePhongAndTextureAndNormalMapAndReflection, "WorldCameraPosition");
	if (location_WorldCameraPosition >= 0) {
		glUniform3fv(location_WorldCameraPosition, 1, &this->m_direct[0]);
	}else {throw std::ios_base::failure("Error en Binding: WorldCameraPosition");}
	
	// cargo los u,v para la texture unit 1
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_buff);

	renderObjectCore(model_matrix, vertex_buff, normal_buff, index_buff,index_buff_size, modo, Ka, Kd, Ks, Shininess,this->programHandlePhongAndTextureAndNormalMapAndReflection);

	glDisableClientState(GL_COLOR_ARRAY);
	glClientActiveTexture (GL_TEXTURE0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_SAMPLER_CUBE_MAP_ARRAY);
	this->disableTextureUnits (2, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_2D, GL_TEXTURE_2D);
}
// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS Y MAPA DE NORMALES
// PHONG RECIBIDO POR PARAMETRO
void myWindow::renderObject(glm::mat4 model_matrix, GLfloat* vertex_buff,
		GLfloat* tangent_buff, GLfloat* normal_buff, GLfloat* texture_buff,
		std::string nombreTextura, std::string nombreTexturaNormales,
		GLuint* index_buff, unsigned int index_buff_size, GLenum modo,
		glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shininess) {
	
	glClientActiveTexture (GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, tangent_buff);
	cargarTexturaYNormal(nombreTextura, this->programHandlePhongAndTextureAndNormalMap,"Tex1",nombreTexturaNormales,"NormalMapTex");
	glTexCoordPointer(2, GL_FLOAT, 0, texture_buff);
	renderObjectCore(model_matrix, vertex_buff, normal_buff, index_buff,index_buff_size, modo, Ka, Kd, Ks, Shininess,this->programHandlePhongAndTextureAndNormalMap);

	glDisableClientState(GL_COLOR_ARRAY);
	this->disableTextureUnits (1, GL_TEXTURE_2D, GL_TEXTURE_2D, GL_TEXTURE_2D);
}

// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS + PARAMETROS DE ANIMACION
// PHONG CON PARAMETROS RECIBIDOS
void myWindow::renderObject(glm::mat4 model_matrix, GLfloat* vertex_buff,
		GLfloat* tangent_buff, GLfloat* normal_buff, GLfloat* texture_buff,
		std::string nombreTextura, GLuint* index_buff,
		unsigned int index_buff_size, GLenum modo,
		glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shininess,
		float amplitud, float numeroOnda, float velocidad, float tiempo)
{
	// QUE BUENO QUE ES REPETIR CODIGO !!!
	GLuint location_amplitud = glGetUniformLocation(this->programHandlePhongAndTextureAndAnimation, "Amp");
	if (location_amplitud >= 0) {
		glUniform1fv(location_amplitud, 1, &amplitud);
	}else {throw std::ios_base::failure("Error en Binding: Amp");}
	
	GLuint location_nOnda = glGetUniformLocation(this->programHandlePhongAndTextureAndAnimation, "WaveNumber");
	if (location_nOnda >= 0) {
		glUniform1fv(location_nOnda, 1, &numeroOnda);
	}else {throw std::ios_base::failure("Error en Binding: WaveNumber");}
	
	GLuint location_velocidad = glGetUniformLocation(this->programHandlePhongAndTextureAndAnimation, "Velocity");
	if (location_velocidad >= 0) {
		glUniform1fv(location_velocidad, 1, &velocidad);
	}else {throw std::ios_base::failure("Error en Binding: Velocity");}
	
	GLuint location_tiempo = glGetUniformLocation(this->programHandlePhongAndTextureAndAnimation, "Time");
	if (location_tiempo >= 0) {
		glUniform1fv(location_tiempo, 1, &tiempo);
	}else {throw std::ios_base::failure("Error en Binding: Time");}
	
	this->renderObjectTextureCore (model_matrix, vertex_buff, tangent_buff, normal_buff, texture_buff, nombreTextura, index_buff,
								   index_buff_size, modo, Ka, Kd, Ks, Shininess, this->programHandlePhongAndTextureAndAnimation);
}

// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS
// PHONG CON PARAMETROS RECIBIDOS
void myWindow::renderObject(glm::mat4 model_matrix, GLfloat* vertex_buff,
		GLfloat* tangent_buff, GLfloat* normal_buff, GLfloat* texture_buff,
		std::string nombreTextura, GLuint* index_buff,
		unsigned int index_buff_size, GLenum modo,
		glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shininess)
{
	this->renderObjectTextureCore (model_matrix, vertex_buff, tangent_buff, normal_buff, texture_buff, nombreTextura, index_buff,
								   index_buff_size, modo, Ka, Kd, Ks, Shininess, this->programHandlePhongAndTexture);
}

// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS   --> RECIBE PROGRAM HANDLE
// PHONG CON PARAMETROS RECIBIDOS
void myWindow::renderObjectTextureCore(glm::mat4 model_matrix, GLfloat* vertex_buff,
		GLfloat* tangent_buff, GLfloat* normal_buff, GLfloat* texture_buff,
		std::string nombreTextura, GLuint* index_buff,
		unsigned int index_buff_size, GLenum modo,
		glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shininess, GLuint programShader)
{
	glClientActiveTexture (GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, tangent_buff);
	cargarTextura(nombreTextura, programShader, "Tex1");
	glTexCoordPointer(2, GL_FLOAT, 0, texture_buff);
	
	renderObjectCore(model_matrix, vertex_buff, normal_buff, index_buff,index_buff_size, modo, Ka, Kd, Ks, Shininess,programShader);
	// ES IMPORTANTE QUE ESTO ESTE DESPUES DE RENDER OBJECT CORE

	glDisableClientState(GL_COLOR_ARRAY);
	this->disableTextureUnits (1, GL_TEXTURE_2D, GL_TEXTURE_2D, GL_TEXTURE_2D);
}

// RENDER CON BUFFERS DE TANGENTES Y TEXTURAS
// PHONG DEFAULT
void myWindow::renderObject (glm::mat4 model_matrix, GLfloat* vertex_buff, GLfloat* tangent_buff, GLfloat* normal_buff,
							 	GLfloat* texture_buff, std::string nombreTextura, GLuint* index_buff, unsigned int index_buff_size, GLenum modo)
{

	glm::vec3 Ka = glm::vec3(0.5,0.5,0.5);
	glm::vec3 Kd = glm::vec3(0.5,0.5,0.5);
	glm::vec3 Ks = glm::vec3(0.5,0.5,0.5);
	float Shininess = 20.0;
	renderObject(model_matrix,vertex_buff,tangent_buff,normal_buff,texture_buff,nombreTextura,index_buff,index_buff_size,modo,Ka,Kd,Ks,Shininess);

}

// RENDER SIN BUFFERS DE TANGENTES Y TEXTURAS
// USA PHONG CON PARAMETROS KA,KS,KD,SHININESS DEFAULTS
void myWindow::renderObject (glm::mat4 model_matrix, GLfloat* vertex_buff, GLfloat* normal_buff,
							 	GLuint* index_buff, unsigned int index_buff_size, GLenum modo)
{
	glm::vec3 Ka = glm::vec3(0.8,0.8,0.3);
	glm::vec3 Kd = glm::vec3(0.8,0.8,0.3);
	glm::vec3 Ks = glm::vec3(0.8,0.8,0.3);
	float Shininess = 20.0;
	renderObject(model_matrix,vertex_buff,normal_buff,index_buff,index_buff_size,modo,Ka,Kd,Ks,Shininess);
}
// RENDER SOLO PHONG RECIBIENDO PARAMETROS KA,KD,KS,SHININESS
void myWindow::renderObject (glm::mat4 model_matrix, GLfloat* vertex_buff, GLfloat* normal_buff,
							 GLuint* index_buff, unsigned int index_buff_size, GLenum modo,
							 glm::vec3 Ka,glm::vec3 Kd,glm::vec3 Ks,float Shininess)
{
	renderObjectCore(model_matrix, vertex_buff, normal_buff, index_buff, index_buff_size,modo,Ka,Kd,Ks,Shininess,this->programHandleSoloPhong);
}
// NUCLEO BASE DE RENDER -> SOLO PHONG
// NOTAR QUE RECIBE COMO PARAMETRO EL PROGRAMHANDLE
void myWindow::renderObjectCore (glm::mat4 model_matrix, GLfloat* vertex_buff, GLfloat* normal_buff,
							 GLuint* index_buff, unsigned int index_buff_size, GLenum modo,
							 glm::vec3 Ka,glm::vec3 Kd,glm::vec3 Ks,float Shininess,GLuint programShader)
{
	setDefault(programShader);
	
	glm::mat3 normal_matrix = calculateNormalMatrix (model_matrix);
    // Bind Normal MAtrix
    GLuint location_normal_matrix = glGetUniformLocation( programShader, "NormalMatrix");
    if( location_normal_matrix >= 0 )
	{
        glUniformMatrix3fv( location_normal_matrix, 1, GL_FALSE, &normal_matrix[0][0]);
	}else {throw std::ios_base::failure("Error en Binding: NormalMatrix");}

    // Bind Model Matrix
    GLuint location_model_matrix = glGetUniformLocation( programShader, "ModelMatrix");
    if( location_model_matrix >= 0 ) {
		glUniformMatrix4fv( location_model_matrix, 1, GL_FALSE, &model_matrix[0][0]);
	}else {throw std::ios_base::failure("Error en Binding: ModelMatrix");}

	GLuint location_Kd = glGetUniformLocation(programShader, "Kd");
	if (location_Kd >= 0) {
		glUniform3fv(location_Kd, 1, &Kd[0]);
	}else {throw std::ios_base::failure("Error en Binding: Kd");}

	GLuint location_Ka = glGetUniformLocation(programShader, "Ka");
	if (location_Ka >= 0) {
		glUniform3fv(location_Ka, 1, &Ka[0]);
	}else {throw std::ios_base::failure("Error en Binding: Ka");}

	GLuint location_Ks = glGetUniformLocation(programShader, "Ks");
	if (location_Ks >= 0) {
		glUniform3fv(location_Ks, 1, &Ks[0]);
	}else {throw std::ios_base::failure("Error en Binding: Ks");}

	GLuint location_Shininess = glGetUniformLocation(programShader, "Shininess");
	if (location_Shininess >= 0) {
		glUniform1fv(location_Shininess, 1, &Shininess);
	}else {throw std::ios_base::failure("Error en Binding: Shininess");}

    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex_buff);
	glNormalPointer(GL_FLOAT, 0, normal_buff);

    glDrawElements (modo, index_buff_size, GL_UNSIGNED_INT, index_buff);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

glm::mat3 myWindow::calculateNormalMatrix (glm::mat4 model_matrix) {
	glm::mat4 aux = this->view_matrix * model_matrix;
	if (glm::determinant (aux) != 0) {
		aux = glm::inverse (aux);
		aux = glm::transpose (aux);
	}
	
	glm::mat3 normal_matrix = glm::mat3 (1.0f);
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			normal_matrix[i][j] = aux[i][j];
	
	return normal_matrix;
}

const char* loadShaderAsString(const char* file)
{
    std::ifstream shader_file(file, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
    return str.c_str();
}

/* funcion auxiliar utilizada por OnKeyDown y axuuuu para escalar los vectores*/
void escalar_aux(glm::vec3& vector,float sensibilidad){
	vector.x = (vector.x /sensibilidad);
	vector.y = (vector.y /sensibilidad);
	vector.z = (vector.z /sensibilidad);
}
void myWindow::axuuu(int a, int b){
	//std::cout<<"x:"<<a<<" y:"<<b<<std::endl;
	//std::cout<<"centroX: "<<width/2<<" centroY: "<<height/2<<std::endl;

	int centroX = (width/2);
	int centroY = (height/2);
	float difx = ( centroX - a);
	float difY = ( centroY -b);
	//std::cout<<"difereciaX: "<<difx<<" diferenciaY: "<<difY<<std::endl;

	glm::vec3 ejeZ(0.0f, 0.0f, 1.0f);
	glm::vec3 dir = (m_pos - m_direct);
	glm::vec3 rotx = glm::normalize(glm::cross(dir, ejeZ));
	glm::vec3 roty = glm::normalize(glm::cross(rotx, dir));

	dir = glm::normalize(dir);

	float sensibilidad = 30.0f;
	escalar_aux(rotx, sensibilidad);
	escalar_aux(roty, sensibilidad);

	if (difx < 0) {
		m_direct = m_pos - dir - rotx;
		glutWarpPointer(width / 2, height / 2);
	} else if (difx > 0) {
		m_direct = m_pos - dir + rotx;
		glutWarpPointer(width / 2, height / 2);
	}
	if (difY > 0) {
		if (roty.z > 0.005f || subio_antes == false) {
			m_direct = m_pos - dir + roty;
		}
		subio_antes = true;
		glutWarpPointer(width/2,height/2);
	}else if (difY<0){
		if (roty.z > 0.005f || subio_antes == true) {
			m_direct = m_pos - dir - roty;
		}
		subio_antes = false;
		glutWarpPointer(width/2,height/2);
	}
	//std::cout<<"ROTY: "<<roty.x<<" , "<<roty.y<<" , "<<roty.z<<std::endl;

	this->Repaint();

}
extern void* instancia;
void aux (int a, int b) {
	((myWindow*)instancia)->axuuu(a,b);
}

myWindow::~myWindow() {
	delete this->mySphere;
	delete this->myCube;
	glDeleteProgram (this->programHandlePhongAndTexture);
	glDeleteProgram (this->programHandlePhongAndTextureAndAnimation);
	glDeleteProgram (this->programHandlePhongAndTextureAndNormalMap);
	glDeleteProgram (this->programHandlePhongAndTextureAndNormalMapAndReflection);
	glDeleteProgram (this->programHandleSoloPhong);
}

myWindow::myWindow():m_pos(8.0f, 0.0f,3.0f), m_direct(1.0f,0.0f,0.0f)
{
	myCube=NULL;
	mySphere=NULL;
    full_screen = false;
    subio_antes = false;
    Cmode = false;
    animar=false;
    glutPassiveMotionFunc(aux);
}

void myWindow::changeObjectColor(float r, float g, float b)
{
    glm::vec3 diffuse_reflectivity = glm::vec3( r, g, b );

    GLuint location_diffuse_reflectivity = glGetUniformLocation( this->programHandleSoloPhong, "Kd"); 
    if( location_diffuse_reflectivity >= 0 ) 
	{ 
		glUniform3fv( location_diffuse_reflectivity, 1, &diffuse_reflectivity[0]); 
	}
}
void myWindow::setDefault(GLuint programHandle){
		this->view_matrix = glm::lookAt ( this->m_pos,this->m_direct, glm::vec3 ( 0.0, 0.0, 1.0 ) );

	    // projection_matrix = glm::mat4 ( 1.0f );
	     glm::mat4 projection_matrix = glm::infinitePerspective( 52.0f , (float)this->width / (float)this->height, 0.1f);

	    glUseProgram( programHandle );

	    // Bind View MAtrix
	    GLuint location_view_matrix = glGetUniformLocation( programHandle, "ViewMatrix");
	    if( location_view_matrix >= 0 )
		{
			glUniformMatrix4fv( location_view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
		}else {throw std::ios_base::failure("Error en Binding: ViewMatrix");}

	    // Bind Projection Matrix
	    GLuint location_projection_matrix = glGetUniformLocation( programHandle, "ProjectionMatrix");
	    if( location_projection_matrix >= 0 )
		{
			glUniformMatrix4fv( location_projection_matrix, 1, GL_FALSE, &projection_matrix[0][0]);
		}else {throw std::ios_base::failure("Error en Binding: ProjectionMatrix");}

	    //////////////////////////////////////
	    // Bind Light Settings
	    glm::vec4 light_position = glm::vec4( 0.0, 0.0, 12.5, 1.0 ); // 8.0, 8.0, 2.0, 1.0
	    light_position = this->view_matrix * light_position; // la llevo al eye space
	    glm::vec3 light_intensity = glm::vec3 (1.0f, 1.0f, 1.0f);
	    glm::vec3 light_La = glm::vec3( 0.1f, 0.15f, 0.25f );
	    glm::vec3 light_Ls = glm::vec3( 1.0f, 1.0f, 1.0f );

	    GLuint location_light_position = glGetUniformLocation( programHandle, "LightPosition");
	    if( location_light_position >= 0 )
		{
	        glUniform4fv( location_light_position, 1, &light_position[0]);
		}else {throw std::ios_base::failure("Error en Binding: LightPosition");}

	    GLuint Ld = glGetUniformLocation( programHandle, "Ld");
	    if( Ld >= 0 )
		{
			glUniform3fv( Ld, 1, &light_intensity[0]);
		}else {throw std::ios_base::failure("Error en Binding: Ld");}

		GLuint location_La = glGetUniformLocation(programHandle, "La");
		if (location_La >= 0) {
			glUniform3fv(location_La, 1, &light_La[0]);
		}else {throw std::ios_base::failure("Error en Binding: La");}

		GLuint location_Ls = glGetUniformLocation(programHandle, "Ls");
		if (location_Ls >= 0) {
			glUniform3fv(location_Ls, 1, &light_Ls[0]);
		}else {throw std::ios_base::failure("Error en Binding: Ls");}
		
		//////////////////////////////////////
	    // Bind Fog Settings
	    glm::vec3 fog_color = this->background_color;
	    float fog_maxDist = 45.0f;
	    float fog_minDist = 10.0f;
	    
	    GLuint location_fog_color = glGetUniformLocation (programHandle, "FogColor");
	    if( location_fog_color >= 0 )
		{
	        glUniform3fv (location_fog_color, 1, &fog_color[0]);
		}else {throw std::ios_base::failure("Error en Binding: FogColor");}

	    GLuint location_fog_maxDist = glGetUniformLocation (programHandle, "FogMaxDist");
	    if( location_fog_maxDist >= 0 )
		{
			glUniform1fv (location_fog_maxDist, 1, &fog_maxDist);
		}else {throw std::ios_base::failure("Error en Binding: FogMaxDist");}
		
		GLuint location_fog_minDist = glGetUniformLocation (programHandle, "FogMinDist");
	    if( location_fog_minDist >= 0 )
		{
			glUniform1fv (location_fog_minDist, 1, &fog_minDist);
		}else {throw std::ios_base::failure("Error en Binding: FogMinDist");}
}
void myWindow::OnRender(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model_matrix = glm::mat4 ( 1.0f );
    
    for (unsigned int i = 0; i < figs.size(); ++i) {
   		figs[i]->renderizar(model_matrix);
   	}
    
    glutSwapBuffers();
}
void myWindow::agregar_figura(Figura* fig){
	this->figs.push_back(fig);
}

void myWindow::OnIdle() {
	if (animar) {
		for (unsigned int i = 0; i < this->figs.size(); ++i) {
			figs[i]->animar();
		}
		OnRender();
	}
}

void myWindow::compilarPrograma(const char* nombreVertexShader, const char* nombreFragmentShader, GLuint& punteroProgramaFinal) {
	// Do your GLEW experiments here:
	if (GLEW_ARB_shading_language_100 ) {
		std::cout<<"compilando: "<<nombreVertexShader<<" y "<<nombreFragmentShader<<"  "<<std::endl;
		std::cout <<"version de glsl de la pc: " ;
		//        int major, minor, revision;
		const GLubyte* sVersion = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
		if (glGetError() == GL_INVALID_ENUM) {
			//            major = 1; minor = 0; revision=51;
		} else {
			std::string version((char*) sVersion);
			std::cout << version.c_str() << std::endl;
		}

		// Load vertex Shader
		this->vertShader = glCreateShader(GL_VERTEX_SHADER);
		if (0 == this->vertShader) {
			std::cout << "Error creating vertex shader" << std::endl;
		}

		std::ifstream v_shader_file(nombreVertexShader, std::ifstream::in);
		std::string v_str((std::istreambuf_iterator<char>(v_shader_file)), std::istreambuf_iterator<char>());
		const char* vs_code_array[] = { v_str.c_str() };

		glShaderSource(this->vertShader, 1, vs_code_array, NULL);

		// Compilar el shader
		glCompileShader(this->vertShader);

		// verificar resultado de la compilacion
		GLint vs_compilation_result;
		glGetShaderiv(this->vertShader, GL_COMPILE_STATUS, &vs_compilation_result);
		if (GL_FALSE == vs_compilation_result) {
			std::cout << "Vertex shader compilation failed!\n" << std::endl;
			GLint logLen;
			glGetShaderiv(this->vertShader, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0) {
				char * log = (char *) malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(vertShader, logLen, &written, log);
				std::cout << "Shader log: " << log << std::endl;
				free(log);
			}
		}

		// Load fragment Shader
		this->fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (0 == this->fragShader) {
			std::cout << "Error creating fragment shader" << std::endl;
		}

		std::ifstream f_shader_file(nombreFragmentShader, std::ifstream::in);
		std::string f_str((std::istreambuf_iterator<char>(f_shader_file)), std::istreambuf_iterator<char>());
		const char* fs_code_array[] = { f_str.c_str() };

		glShaderSource(this->fragShader, 1, fs_code_array, NULL);

		// Compilar el shader
		glCompileShader(this->fragShader);

		// verificar resultado de la compilacion
		GLint fs_compilation_result;
		glGetShaderiv(this->fragShader, GL_COMPILE_STATUS, &fs_compilation_result);
		if (GL_FALSE == fs_compilation_result) {
			std::cout << "Fragment shader compilation failed!\n" << std::endl;
			GLint logLen;
			glGetShaderiv(this->fragShader, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0) {
				char * log = (char *) malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(this->fragShader, logLen, &written, log);
				std::cout << "Shader log: " << log << std::endl;
				free(log);
			}
		}
		// *******************************************

		// *******************************************
		// Linking the shader programms
		// *******************************************
		punteroProgramaFinal = glCreateProgram();
		if (0 == punteroProgramaFinal) {
			std::cout << "Error creating program object" << std::endl;
		} else {
			glAttachShader(punteroProgramaFinal, this->vertShader);
			glAttachShader(punteroProgramaFinal, this->fragShader);

			glLinkProgram(punteroProgramaFinal);

			GLint status;
			glGetProgramiv(punteroProgramaFinal, GL_LINK_STATUS, &status);
			if (GL_FALSE == status) {
				std::cout << "Failed to link shader program!\n" << std::endl;
				GLint logLen;
				glGetProgramiv(punteroProgramaFinal, GL_INFO_LOG_LENGTH, &logLen);
				if (logLen > 0) {
					char * log = (char *) malloc(logLen);
					GLsizei written;
					glGetProgramInfoLog(programHandleSoloPhong, logLen, &written, log);
					std::cout << "Program log: \n%s" << std::endl;
					free(log);
				}
			} else {
				glUseProgram(punteroProgramaFinal);
				long num = punteroProgramaFinal;
				std::cout << "Numero Recibido: "<< num << std::endl;
			}
		}
	}
}
// When OnInit is called, a render context (in this case GLUT-Window)
// is already available!
void  myWindow::OnInit()
{
    this->mySphere = new Esfera (this, 1.0, 32, 32);
    this->myCube = new Cubo (this);
	this->background_color = glm::vec3 (0.0f, 0.15f, 0.3f);
	glClearColor(background_color.x, background_color.y, background_color.z, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	compilarPrograma("VertexPhong+Fog.vert","FragmentPhong+Fog.frag",this->programHandleSoloPhong);
	compilarPrograma("VertexPhong+Texture+Fog.vert", "FragmentPhong+Texture+Fog.frag",this->programHandlePhongAndTexture);
	compilarPrograma("VertexPhong+Texture+Fog+Animation.vert", "FragmentPhong+Texture+Fog+Animation.frag",this->programHandlePhongAndTextureAndAnimation);
	compilarPrograma("VertexPhong+Texture+NormalMap+Fog.vert","FragmentPhong+Texture+NormalMap+Fog.frag",this->programHandlePhongAndTextureAndNormalMap);
	compilarPrograma("VertexPhong+Texture+NormalMap+Reflexion+Fog.vert","FragmentPhong+Texture+NormalMap+Reflexion+Fog.frag",this->programHandlePhongAndTextureAndNormalMapAndReflection);

	animar=false;
}

void myWindow::OnResize(int w, int h) 
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    this->width = w;
    this->height = h;
}

void  myWindow::OnClose(void)
{
}

void myWindow::OnMouseDown(int button, int x, int y)
{
}

void myWindow::OnMouseUp(int button, int x, int y)
{
}

void myWindow::OnMouseWheel(int nWheelNumber, int nDirection, int x, int y)
{
}
//! Called when Mouse is moved (without pressing any button)
void myWindow::OnMouseMove(int x, int y){
//std::cout<<"se movio en x: "<<x<<std::endl;
//std::cout<<"se movio en y: "<<y<<std::endl;
}

void myWindow::OnKeyDown(int nKey, char cAscii)
{
	glm::vec3 ejeZ (0.0f,0.0f,1.0f);
	glm::vec3 ejeY (0.0f,1.0f,0.0f);
	glm::vec3 dir = (m_pos-m_direct);
	glm::vec3 rotx = glm::normalize( glm::cross(dir, ejeZ) );
	glm::vec3 roty = glm::normalize( glm::cross(rotx,dir) );

	dir = glm::normalize(dir);

	float sensibilidad = 5.0f;
	escalar_aux(dir,sensibilidad);
	escalar_aux(rotx,sensibilidad);
	escalar_aux(roty,sensibilidad);

	bool cerrada = false;
	if (cAscii == 27) // 0x1b = ESC
	{
		cerrada = true;
		this->Close(); // Close Window!
	}
	sensibilidad=sensibilidad*2;
	switch(cAscii) {
	case ('4'):
		escalar_aux(rotx,sensibilidad);
		m_direct = m_pos-dir+rotx;
		break;
	case ('6'):
		escalar_aux(rotx,sensibilidad);
		m_direct = m_pos-dir-rotx;
		break;
	case ('8'):
		escalar_aux(roty,sensibilidad);
		if(roty.z>0.002 || subio_antes==false){
		m_direct = m_pos-dir+roty;
		}
		subio_antes=true;
		//std::cout<<"ROTY: "<<roty.x<<" , "<<roty.y<<" , "<<roty.z<<std::endl;
		break;
	case ('5'):
		escalar_aux(roty,sensibilidad);
		if(roty.z>0.002 || subio_antes==true){
			m_direct = m_pos-dir-roty;
		}
		subio_antes=false;
		//std::cout<<"ROTY: "<<roty.x<<" , "<<roty.y<<" , "<<roty.z<<std::endl;
		break;
	case ('w'):
		this->m_pos -= dir;
		this->m_direct = m_pos - dir;
		break;
	case ('s'):
		this->m_pos += dir;
		this->m_direct = m_pos - dir;
		break;
	case ('a'):
		if(!Cmode) {
			this->m_pos += rotx;
			m_direct = m_pos - dir;
		} else {
			if( m_direct.z<-0.1f || m_direct.z>0.1f ) {
				m_direct.x = ( ( m_direct.z/(m_pos.z-m_direct.z) ) * (  m_direct.x - m_pos.x ) ) + m_direct.x;
				m_direct.y = ( ( m_direct.z/(m_pos.z-m_direct.z) ) * ( m_direct.y - m_pos.y ) ) + m_pos.y;
				m_direct.z=0;
			}
			this->m_pos += rotx;
		}
		break;
	case ('d'):
		if(!Cmode){
			this->m_pos -= rotx;
			m_direct = m_pos - dir;
		} else {
			if( m_direct.z<-0.1f || m_direct.z>0.1f ){
				m_direct.x = ( ( m_direct.z/(m_pos.z-m_direct.z) ) * ( m_direct.x - m_pos.x ) ) + m_pos.x;
				m_direct.y = ( ( m_direct.z/(m_pos.z-m_direct.z) ) * ( m_direct.y - m_pos.y ) ) + m_pos.y;
				m_direct.z=0;
			}
			this->m_pos -= rotx;
		}
		break;
	case ('h'):
		if(animar==false){
			animar=true;
		}else{
			animar=false;
		}
		//this->animar=-(this->animar);
		break;
	case ('i'):
		this->m_pos.z+=0.5f;
		this->m_direct.z+=0.5f;
		break;
	case ('k'):
		this->m_pos.z-=0.5f;
		this->m_direct.z-=0.5f;
		break;
	case ('c'):
		Cmode=!Cmode;
	}
	//std::cout<<"at: "<<m_direct.x<<","<<m_direct.y<<","<<m_direct.z<<"from: "<<m_pos.x<<","<<m_pos.y<<","<<m_pos.z<<std::endl;
	if (!cerrada) this->OnRender();
}

void myWindow::OnKeyUp(int nKey, char cAscii)
{
	if (cAscii == 'f') {
		full_screen=!full_screen;
		SetFullscreen(full_screen);
	}
}


