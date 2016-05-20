#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Scene
{
public:
	std::string name;

	Scene(std::string name) {
		this->name = name;
	}

	GLfloat* getVertices() {
		GLfloat vertices[] = {
			//positions			//colors
			-0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f, // Top-left
			0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // Top-right
			0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, // Bottom-right
			-0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 0.0f  // Bottom-left
		};
		return vertices;
	}

	std::vector<const GLchar*> getFaces() {
		std::vector<const GLchar*> faces;
		faces.push_back("Resources/skybox/right.jpg");
		faces.push_back("Resources/skybox/left.jpg");
		faces.push_back("Resources/skybox/top.jpg");
		faces.push_back("Resources/skybox/bottom.jpg");
		faces.push_back("Resources/skybox/back.jpg");
		faces.push_back("Resources/skybox/front.jpg");
		return faces;
	}

	std::vector<int> getVecAtrib() {
		std::vector<int> va = { 0, 3, 6, 0 };
		return va;
	}

	std::vector<int> getColAtrib() {
		std::vector<int> ca = { 1, 3, 6, 3 };
		return ca;
	}
};

#endif