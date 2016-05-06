//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//int main(void)
//{
//	GLFWwindow* window;  //what we'll draw openGL to
//
//	// initialize GLFW
//	if (!glfwInit())
//	{
//		return -1;
//	}
//
//	// create a windowed mode and its openGL context
//	window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
//
//	// check if window created properly
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	// make the window's context current
//	glfwMakeContextCurrent(window);
//
//	// loop until the user closes the window
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glClear(GL_COLOR_BUFFER_BIT);
//		// TODO: render the OpenGL here
//
//		// swap front and back buffers
//		glfwSwapBuffers(window);
//
//		// poll for and process events
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}