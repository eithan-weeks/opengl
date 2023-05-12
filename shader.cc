#include "shader.hh"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

shader::shader(char const* vertexPath, char const* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;

	try {
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure const& e) {
		std::cerr << "ERROR:SHADER::FILE_NOT_SUCCESSFULLY_READ" << "\n";
	}

	char const* const vShaderCode {vertexCode.c_str()};
	char const* const fShaderCode {fragmentCode.c_str()};

	int success;
	char infoLog[512];

	unsigned int const vertex {glCreateShader(GL_VERTEX_SHADER)};
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	unsigned int const fragment {glCreateShader(GL_FRAGMENT_SHADER)};
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

shader::~shader() {
	glDeleteProgram(id);
}

void shader::use() {
	glUseProgram(id);
}

void shader::setBool(char const* name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name), static_cast<int>(value));
}

void shader::setInt(char const* name, int value) const {
	glUniform1i(glGetUniformLocation(id, name), value);
}

void shader::setFloat(char const* name, float value) const {
	glUniform1f(glGetUniformLocation(id, name), value);
}
