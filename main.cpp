#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



static std::string ParseShader(const std::string& filepath){
	std::ifstream stream(filepath);
	enum class ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		ss << line << "\n";
	}
	return ss.str();
}

static unsigned int CompileShader(const std::string& source, const unsigned int& type){
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE){
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "New Cpp Game", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewInit();


	std::cout << glGetString(GL_VERSION) << "  " << std::endl;

	float positions[] = {
		-0.5, -0.5,
		 0.5, -0.5,
		 0.5,  0.5,
		-0.5,  0.5,	
	};
	unsigned int indices[] {
		0, 1, 2,
			2, 3, 0
	};
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int ibo;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	std::string VertexshaderSrc = ParseShader("res/shaders/vertex_shader.glsl");
	std::string FragmentshaderSrc = ParseShader("res/shaders/fragment_shader.glsl");

	unsigned int shader = CreateShader(VertexshaderSrc, FragmentshaderSrc);
	glUseProgram(shader);

	float a = 0;
	float b = 0.05f;
	int location = glGetUniformLocation(shader,"u_Color");
	while (!glfwWindowShouldClose(window))
	{
		
		
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform4f(location, a, 1.0, 1.0, 1.0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		if (a > 1.0){
			b = -0.05f;
		}
		else if(a < 0){
			b = 0.05f;
		}
		a += b;
		std::cout << a << std::endl;
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}