#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


#include <shader.h>

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

constexpr unsigned int WIDTH = 1024;
constexpr unsigned int HEIGHT = 800;

int main()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to load glfw!\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Raymarching Renderer", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create glfw window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//screen quad
	float screenVerticies[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f
	};

	unsigned int screenVAO, screenVBO;
	glGenBuffers(1, &screenVBO);
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVerticies), screenVerticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	shader screenShader("shaders/ScreenVertex.glsl", "shaders/ScreenFragment.glsl");

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(screenVAO);
		screenShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &screenVBO);
	glDeleteVertexArrays(1, &screenVAO);
	glfwTerminate();
	return 0;
}
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}