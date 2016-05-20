#pragma once
#ifndef ALTSCENE_H
#define ALTSCENE_H

#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Scene.h"

class altScene : public Scene {
	GLfloat* getVertices() {
		GLfloat vertices[] = {
			//positions				//colors
			0.5f,  -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // Top-left
			-0.5f,  -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // Top-right
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, // Bottom-right
		};
		return vertices;
	}
};

#endif