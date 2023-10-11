#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(std::string p_vertexShaderPath, std::string p_fragmentShaderPath){
	
	_id = glCreateProgram();

	if (_id == 0) {
		std::cerr << "Error creating shader program\n";
		exit(1);
	}

	AddShader(ReadShaderFromFile("shader.vert"), GL_VERTEX_SHADER);
	AddShader(ReadShaderFromFile("shader.frag"), GL_FRAGMENT_SHADER);

	glLinkProgram(_id);

	GLint linkSuccess;
	glGetProgramiv(_id, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess) {
		GLchar dump[1024];
		glGetProgramInfoLog(_id, 1024, NULL, dump);
		std::cerr << "Error while linking shader program: " << dump << "\n";
		exit(1);
	}

	glValidateProgram(_id);

	GLint validateSuccess;
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &validateSuccess);

	if (!validateSuccess) {
		GLchar dump[1024];
		glGetProgramInfoLog(_id, 1024, NULL, dump);
		std::cerr << "Error while validating shader program: " << dump << "\n";
		exit(1);
	}

	glUseProgram(_id);

}

void ShaderProgram::SetVariable(std::string p_variableName, glm::mat4 p_value) {
	
	GLuint location = glGetUniformLocation(_id, p_variableName.c_str());
	if (location == -1) {
		std::cerr << "Variable " << p_variableName <<" not defined in Vertex Shader\n";
	}

	glProgramUniformMatrix4fv(_id, location, 1, GL_FALSE, glm::value_ptr(p_value));
}

void ShaderProgram::SetVariable(std::string p_variableName, float p_value) {

	GLuint location = glGetUniformLocation(_id, p_variableName.c_str());
	if (location == -1) {
		std::cerr << "Variable " << p_variableName << " not defined in Vertex Shader\n";
		return;
	}

	glProgramUniform1f(_id, location, p_value);
}

void ShaderProgram::AddShader(std::string p_shaderText, GLenum p_shaderType){
	
	GLuint shader = glCreateShader(p_shaderType);

	if (shader == 0) {
		std::cerr << "Error creating shader object \n";
		exit(1);
	}

	const GLchar* shaderText[] = { p_shaderText.c_str() };
	GLint shaderLengths[] = { p_shaderText.length() };

	glShaderSource(shader, 1, shaderText, shaderLengths);

	glCompileShader(shader);

	GLint compileSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess) {
		//TODO: extract this to some dumpSize constexpr
		GLchar dump[1024];
		glGetShaderInfoLog(shader, 1024, NULL, dump);
		std::cerr << "Error while compiling shader: " << dump << "\n";
		exit(1);
	}

	glAttachShader(_id, shader);

	glDeleteShader(shader);
}

std::string ShaderProgram::ReadShaderFromFile(std::string p_fileName){
	
	std::ifstream file(p_fileName);

	if (!file.is_open()) {
		std::cerr << "Error while reading file " << p_fileName << "\n";
		//TODO: replace exits with exceptions
		exit(1);
	}

	std::string content;
	std::string line;
	while (std::getline(file, line)) {
		content += line + "\n";
	}
	return content;
}