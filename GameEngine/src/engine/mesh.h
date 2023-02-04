#pragma once

#include <GLM/glm.hpp>
#include <vector>
#include <format>

// Core
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(const std::vector<Vertex>& vert, const std::vector<unsigned int>& indi, const std::vector<Texture>& text);
	void Draw(Shader& shader);

private:
	VertexArray VAO;
	VertexBuffer VBO;
	IndexBuffer EBO;
	VertexBufferLayout VBL;

	void setupMesh();
};

Mesh::Mesh(const std::vector<Vertex>& vert, const std::vector<unsigned int>& indi, const std::vector<Texture>& text)
	: vertices(vert), indices(indi), textures(text), 
	  VBO(&vertices[0], static_cast<unsigned int>(vertices.size() * sizeof(Vertex)),        GL_STATIC_DRAW),
	  EBO(&indices[0],  static_cast<unsigned int>(indices.size()  * sizeof(unsigned int)), GL_STATIC_DRAW)
{
	this->setupMesh();
}

void Mesh::setupMesh()
{
	VAO.Bind();
	VBO.Bind();
	EBO.Bind();
	VBL.Push<float>(3); // positions
	VBL.Push<float>(3); // normals
	VBL.Push<float>(2); // texCoord
	VAO.AddBuffer(VBO, VBL);

	VAO.Unbind();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseCounter = 0;
	unsigned int specularCounter = 0;
	unsigned int emissionCounter = 0;

	for (int i = 0; i < textures.size(); ++i)
	{
		using enum Texture::Type;
		unsigned int counter = 0;
		if (textures[i].type      == DIFFUSE)
			counter = diffuseCounter++;
		else if (textures[i].type == SPECULAR)
			counter = specularCounter++;
		else if (textures[i].type == EMISSION)
			counter = emissionCounter++;
		else
			std::cout << "ERROR::NO TEXTURE TYPE FIND\n";

		textures[i].Bind(i);
		std::string uniformName = std::format("material.{}[{}]", Texture::to_string(textures[i].type), std::to_string(counter));
		shader.SetUniform1i(uniformName, i);
	}
	glActiveTexture(GL_TEXTURE0);

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
	VAO.Unbind();
}
