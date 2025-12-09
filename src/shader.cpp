#include <shader/shader.h>
#include <glad/glad.h>

#include <sstream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	/* Ensure ifstream objects can throw exceptions: */
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
	
		/* Open Files */
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		/* Read file's buffer contents into streams */
		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		/* Close file handlers */
		vertexShaderFile.close();
		fragmentShaderFile.close();

		/* Convert stream into a string */
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	} catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::NOT_READ: " << e.what() << std::endl; 
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	/* Compile Shaders */
	unsigned int vertex, fragment;

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// Shader program 
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// Delete Shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];

	if(type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION of type: " << type << "\n"
				<< infoLog << std::endl;
		}
		return;
	}

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINK of type: "<< type << "\n" << infoLog << std::endl;
	}
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), value);
}
