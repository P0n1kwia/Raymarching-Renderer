#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>

#include <shader.h>

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

//changing shaders
std::vector<std::string> ScanFolderForShaders(const std::string& folderPath);
void ProcessInput(GLFWwindow* window, int& iterator, bool& reload);

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

	// brute force for hot reload;
	std::filesystem::path rootPath(ROOT_DIR); 
	std::filesystem::path shaderPath = rootPath / "shaders"; 

	auto shaders = ScanFolderForShaders(shaderPath.string());

	
	std::string vertexPath = (shaderPath / "ScreenVertex.glsl").string();
	shader screenShader;
	
	int iterator = 0;
	int lastIterator=0;
	glBindVertexArray(0);
	

	bool reload = false;
	screenShader.Compile(vertexPath, shaders[iterator]);
	std::cout << "Loaded: " << shaders[iterator] << "\n";
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		//input and shader loading
		ProcessInput(window, iterator,reload);
		if (iterator < 0)
		{
			iterator = 0;
		}
		else if (iterator >= shaders.size())
		{
			iterator = shaders.size() - 1;
		}
		if (reload || iterator != lastIterator)
		{
			screenShader.Compile(vertexPath, shaders[iterator]);
			if (!reload)
				std::cout << "Loaded: " << shaders[iterator] << "\n";
			else
				std::cout << "Reloaded: " << shaders[iterator] << "\n";
		}
		
		
		

		glBindVertexArray(screenVAO);

		screenShader.use();


		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec2 resolution = glm::vec2(WIDTH, HEIGHT);
		screenShader.setVec2("u_resolution", resolution);
		screenShader.setVec2("u_mouse", { xpos,ypos });
		screenShader.setFloat("u_time", time);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
		reload = false;
		lastIterator = iterator;
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
std::vector<std::string> ScanFolderForShaders(const std::string& folderPath)
{
	std::vector<std::string> shaders;
	std::filesystem::path path{ folderPath };
	std::string vertex = path.string() + "\\ScreenVertex.glsl";
	for (auto const& shader : std::filesystem::directory_iterator{ path })
	{
		if (shader.path().string() == vertex) continue;
		shaders.push_back(shader.path().string());
	}
	return shaders;

}
void ProcessInput(GLFWwindow* window, int& iterator, bool& reload)
{
	static bool rPressed = false;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (!rPressed) 
		{ 
			reload = true;
			rPressed = true;
			
		}
	}
	else 
	{
		rPressed = false; 
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
	static bool qPressed = false;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (!qPressed)
		{
			--iterator;
			qPressed = true;
		}
	}
	else 
	{
		qPressed = false;
	}
	static bool ePressed = false;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (!ePressed) 
		{
			++iterator;
			ePressed = true;
		}
	}
	else 
	{
		ePressed = false;
	}

}