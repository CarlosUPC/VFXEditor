#include "Shader.h"

#include <string>
#include <fstream> //ifstream
#include <sstream> //stringstream
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Read vertex and fragment shaders files
	const char* vShaderCode = getShaderCode(vertexPath);
	const char* fShaderCode = getShaderCode(fragmentPath);

	//Compile both shaders
	uint vertexShader = compileShader(vShaderCode, GL_VERTEX_SHADER);
	uint fragmentShader = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

	//Create program
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	//Check for errors
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		LOG("Error linking program: %s", infoLog);
	}

	//Delete shaders (already linked into program)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::Use()
{
	glUseProgram(programID);
}

const char* Shader::getShaderCode(const char* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.open(shaderPath);
	if (!shaderFile)
	{
		LOG("Error opening shader file: %s", shaderPath);
	}
	else
	{
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}

	return shaderCode.c_str();
}

uint Shader::compileShader(const char* shaderCode, GLenum shaderType)
{
	uint shader = 0;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	//Check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		LOG("Error compiling shader: %s", infoLog);
	}

	return shader;
}