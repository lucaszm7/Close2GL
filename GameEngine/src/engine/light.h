#pragma once

#include <GLM/glm.hpp>

struct Light
{
protected:
	glm::vec3 color;

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 lightColor)
		: color(lightColor)
	{
		this->ambient  = lightColor * glm::vec3(0.05f);
		this->diffuse  = lightColor * glm::vec3(0.5f);
		this->specular = lightColor;
	}

	void SetLightColor(glm::vec3 lightColor)
	{
		this->color = lightColor;
		this->ambient = lightColor * glm::vec3(0.05f);
		this->diffuse = lightColor * glm::vec3(0.5f);
		this->specular = lightColor;
	}

	inline glm::vec3 GetLightColor() const
	{
		return this->color;
	}
};

struct DirectionalLight : public Light
{
	glm::vec3 direction;

	DirectionalLight(glm::vec3 lightColor, glm::vec3 dir)
		: Light(lightColor), direction(dir) 
	{
		diffuse  *= lightColor * glm::vec3(0.5f);
		specular *= lightColor * glm::vec3(0.5f);
	}
};

struct PointLight : public Light
{
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 lightColor, glm::vec3 pos, float cons = 1.0f, float lin = 0.09f, float quad = 0.032f)
		: Light(lightColor), position(pos), constant(cons), linear(lin), quadratic(quad) 
	{
		diffuse = lightColor * glm::vec3(0.8f);
	}
};

struct SpotLight : public Light
{
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	SpotLight(glm::vec3 lightColor, glm::vec3 pos, glm::vec3 dir, float cut = glm::cos(glm::radians(12.5f)), float outerCut = glm::cos(glm::radians(17.5f)),
			  float cons = 1.0f, float lin = 0.09f, float quad = 0.032f)
		: Light(lightColor), position(pos), direction(dir), cutOff(cut), outerCutOff(outerCut), constant(cons), linear(lin), quadratic(quad) 
	{
		this->ambient = glm::vec3(0.0f);
		this->diffuse = lightColor;
	}
};