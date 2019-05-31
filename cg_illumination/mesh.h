#pragma once

#include <iostream>
#include <vector>
#include <glm/gtx/rotate_vector.hpp>
#include <map>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "vertex.h"
#include "primitives.h"
#include "shader.h"
#include "texture.h"
#include "Material.h"

class Mesh{
private:
	Vertex * vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotationAroundOrigin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 model;

	void initVAO(){
		//Create VAO
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		if(this->nrOfIndices > 0){
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}

	void updateUniforms(Shader* shader){
		shader->setMat4fv(this->model, "model");
	}

	void updateModelMatrix(){
		this->model = glm::mat4(1.f);
		this->model = glm::translate(this->model, this->origin);
		this->model = glm::rotate(this->model, glm::radians(this->rotationAroundOrigin.x), glm::vec3(1.f, 0.f, 0.f));
		this->model = glm::rotate(this->model, glm::radians(this->rotationAroundOrigin.y), glm::vec3(0.f, 1.f, 0.f));
		this->model = glm::rotate(this->model, glm::radians(this->rotationAroundOrigin.z), glm::vec3(0.f, 0.f, 1.f));
		this->model = glm::translate(this->model, this->position - this->origin);
		this->model = glm::rotate(this->model, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->model = glm::rotate(this->model, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->model = glm::rotate(this->model, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->model = glm::scale(this->model, this->scale);
	}

public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 localRotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){
		this->position = position;
		this->origin = origin;
		this->rotationAroundOrigin = rotation;
		this->rotation = localRotation;
		this->scale = scale;

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for(size_t i = 0; i < nrOfVertices; i++){
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for(size_t i = 0; i < nrOfIndices; i++){
			this->indexArray[i] = indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(Primitive* primitive, glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f), glm::vec3 localRotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){
		this->position = position;
		this->origin = origin;
		this->rotationAroundOrigin = rotation;
		this->rotation = localRotation;
		this->scale = scale;

		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndices = primitive->getNrOfIndices();

		this->vertexArray = new Vertex[this->nrOfVertices];
		for(size_t i = 0; i < this->nrOfVertices; i++){
			this->vertexArray[i] = primitive->getVertices()[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for(size_t i = 0; i < this->nrOfIndices; i++){
			this->indexArray[i] = primitive->getIndices()[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const Mesh& obj){
		this->position = obj.position;
		this->origin = obj.origin;
		this->rotationAroundOrigin = obj.rotationAroundOrigin;
		this->scale = obj.scale;

		this->nrOfVertices = obj.nrOfVertices;
		this->nrOfIndices = obj.nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for(size_t i = 0; i < this->nrOfVertices; i++){
			this->vertexArray[i] = obj.vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for(size_t i = 0; i < this->nrOfIndices; i++){
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const char *path, glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 localRotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){
		this->position = position;
		this->origin = origin;
		this->rotationAroundOrigin = rotation;
		this->rotation = localRotation;
		this->scale = scale;

		std::vector<unsigned int> vertexIndices;
		std::vector<glm::vec3> temp_vertices;

		FILE * file = fopen(path, "r");
		if(file == NULL){
			printf("Impossible to open the file !\n");
		}

		while(1){
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if(res == EOF) {
				break; // EOF = End Of File. Quit the loop.
			}

			// else : parse lineHeader
			// vertices
			if(strcmp(lineHeader, "v") == 0){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
				// faces
			} else if(strcmp(lineHeader, "f") == 0){
				//std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3];
				int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
				if(matches != 3){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
			}
		}

		this->nrOfVertices = vertexIndices.size();
		this->nrOfIndices = 0;

		// compute normals
		glm::vec3 *unit_norm = new glm::vec3[this->nrOfVertices];
		for(int i = 0; i < this->nrOfVertices; i++){
			unit_norm[i] = glm::vec3(0.f);
		}
		for(int i = 0; i < this->nrOfVertices; i += 3){
			// compute normal for each face (assume winding order ccw)
			glm::vec3 a = temp_vertices[vertexIndices[i + 1] - 1] - temp_vertices[vertexIndices[i] - 1];
			glm::vec3 b = temp_vertices[vertexIndices[i + 2] - 1] - temp_vertices[vertexIndices[i] - 1];
			glm::vec3 norm = glm::normalize(glm::cross(a, b));
			unit_norm[vertexIndices[i] - 1] += norm;
			unit_norm[vertexIndices[i + 1] - 1] += norm;
			unit_norm[vertexIndices[i + 2] - 1] += norm;
			//std::cout << this->nrOfVertices << " " << i << "/" << this->nrOfIndices << std::endl;
		}
		

		this->vertexArray = new Vertex[this->nrOfVertices];
		// For each vertex of each triangle
		for(unsigned int i = 0; i < this->nrOfVertices; i++){
			unsigned int vI = vertexIndices[i] - 1;
			glm::vec3 position = temp_vertices[vI];
			glm::vec4 color = glm::vec4(1.f, 0.f, 1.f, 1.f);
			glm::vec2 texCoord = glm::vec2(0.f);
			// average of normals of adjacent faces
			glm::vec3 normal = glm::normalize(unit_norm[vI]);
			// iterate over all faces
			//for(int j = 0; j < this->nrOfVertices; j += 3){
			//	if(vertexIndices[j] == vI || vertexIndices[j + 1] == vI || vertexIndices[j + 2] == vI){
			//		normal += unit_norm[j / 3];
			//	}
			//}
			Vertex vertex = {position, color, texCoord, normal};
			this->vertexArray[i] = vertex;
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	~Mesh(){
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);

		if(this->nrOfIndices > 0){
			glDeleteBuffers(1, &this->EBO);
		}

		delete[] this->vertexArray;
		delete[] this->indexArray;
	}

	//Accessors

	//Modifiers
	void setPosition(const glm::vec3 position){
		this->position = position;
	}

	glm::vec3 getPosition(){
		return this->position;
	}

	glm::mat4 getModelMatrix(){
		return this->model;
	}

	void setOrigin(const glm::vec3 origin){
		this->origin = origin;
	}

	void setRotation(const glm::vec3 rotation){
		this->rotation = rotation;
	}

	void setRotationAroundOrigin(const glm::vec3 rotation){
		this->rotationAroundOrigin = rotation;
	}

	void setScale(const glm::vec3 scale){
		this->scale = scale;
	}

	//Functions

	void move(const glm::vec3 position){
		this->position += position;
	}

	void rotate(const glm::vec3 rotation){
		this->rotation += rotation;
	}

	void rotateAroundOrigin(const glm::vec3 rotation){
		this->rotationAroundOrigin += rotation;
	}

	void scaleUp(const glm::vec3 scale){
		this->scale += scale;
	}

	void update(){

	}

	void render(Shader* shader, int mode = GL_TRIANGLES, int patchsize = 25){
		//Update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->Use();

		//Bind VAO
		glBindVertexArray(this->VAO);

		if(mode == GL_PATCHES){
			glPatchParameteri(GL_PATCH_VERTICES, patchsize);
		}

		//RENDER
		if(this->nrOfIndices == 0){
			glDrawArrays(mode, 0, this->nrOfVertices);
		}else{
			glDrawElements(mode, this->nrOfIndices, GL_UNSIGNED_INT, 0);
		}

		//Cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};