#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other libs
#include <SOIL.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(std::vector<const GLchar*> faces);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Window dimensions
//const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", primary, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, mode->width, mode->height);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader ourShader("default.vs", "default.frag", "default.gs");
	Shader skyboxShader("skybox.vs", "skybox.frag");

	#pragma region "object_initialization"

	GLfloat skyboxVertices[] = {
		// Positions          
		-5.0f,  5.0f, -5.0f,
		-5.0f, -5.0f, -5.0f,
		5.0f, -5.0f, -5.0f,
		5.0f, -5.0f, -5.0f,
		5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		5.0f, -5.0f, -5.0f,
		5.0f, -5.0f,  5.0f,
		5.0f,  5.0f,  5.0f,
		5.0f,  5.0f,  5.0f,
		5.0f,  5.0f, -5.0f,
		5.0f, -5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		5.0f,  5.0f,  5.0f,
		5.0f,  5.0f,  5.0f,
		5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		-5.0f,  5.0f, -5.0f,
		5.0f,  5.0f, -5.0f,
		5.0f,  5.0f,  5.0f,
		5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		5.0f, -5.0f, -5.0f,
		5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		5.0f, -5.0f,  5.0f
	};

	// Vertex data
	GLfloat points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // Bottom-left
	};
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	// Unbind VAO
	glBindVertexArray(0);

	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	#pragma endregion

	// Cubemap (Skybox)
	std::vector<const GLchar*> faces;
	faces.push_back("Resources/skybox/right.jpg");
	faces.push_back("Resources/skybox/left.jpg");
	faces.push_back("Resources/skybox/top.jpg");
	faces.push_back("Resources/skybox/bottom.jpg");
	faces.push_back("Resources/skybox/back.jpg");
	faces.push_back("Resources/skybox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 projection = glm::perspective(camera.Zoom, (float)mode->width / (float)mode->height, 0.1f, 1000.0f);
	ourShader.Use();
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		Do_Movement();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the scene
		ourShader.Use();

		// Create camera transformation
		glm::mat4 view = camera.GetViewMatrix();
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// Calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);

		//Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion