#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "Material.h"
#include "particleSystem.h"

class Object{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 origin;

	void updateUniforms(){

	}

public:
	Object(glm::vec3 origin, Material* material, Texture* orTexDif, Texture* orTexSpec, std::vector<Mesh*>& meshes){
		this->origin = origin;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		for(auto* i : meshes){
			this->meshes.push_back(new Mesh(*i));
		}

		for(auto& i : this->meshes){
			i->move(this->origin);
			i->setOrigin(this->origin);
		}
	}

	~Object(){
		for(auto*& i : this->meshes){
			delete i;
		}
	}

	//Functions
	glm::vec3 getPosition(){
		std::vector<glm::vec3> positions;
		for(auto& i : this->meshes){
			positions.push_back(i->getPosition());
		}
		return positions[0];
	}

	glm::mat4 getModelMatrix(){
		std::vector<glm::mat4> matrices;
		for(auto& i : this->meshes){
			matrices.push_back(i->getModelMatrix());
		}
		return matrices[0];
	}

	void rotate(const glm::vec3 rotation){
		for(auto& i : this->meshes){
			i->rotate(rotation);
		}
	}

	void rotateAroundOrigin(const glm::vec3 rotation){
		for(auto& i : this->meshes){
			i->rotateAroundOrigin(rotation);
		}
	}

	void scaleUp(const glm::vec3 scale){
		for(auto& i : this->meshes){
			i->scaleUp(scale);
		}
	}

	void move(const glm::vec3 position){
		for(auto& i : this->meshes){
			i->move(position);
		}
	}
 
	void update(){

	}

	void render(Shader* shader){
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_TRIANGLES);
		}
	}

	void renderPoints(Shader* shader, float size = 10.f){
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);

		// Set point size
		glPointSize(size);

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_POINTS);
		}
	}

	void renderPatches(Shader* shader){
		shader->Use();
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_PATCHES);
		}
	}
};