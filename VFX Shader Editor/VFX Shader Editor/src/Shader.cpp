#include "Shader.h"

#include <string>
#include <fstream> //ifstream
#include <sstream> //stringstream
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Parse shaders files
	const char* vShaderSource = ParseShader(vertexPath);
	const char* fShaderSource = ParseShader(fragmentPath);

	m_programID = CreateShader(vShaderSource, fShaderSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_programID);
}

void Shader::Bind()
{
	glUseProgram(m_programID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

const char* Shader::ParseShader(const char* shaderPath)
{
	std::ifstream shaderFile;
	shaderFile.open(shaderPath);

	std::string shaderSource;

	if (!shaderFile)
	{
		LOG("Error opening shader file: %s", shaderPath);
	}
	else
	{
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderSource = shaderStream.str();
	}

	return shaderSource.c_str();
}

uint Shader::CompileShader(const char*& shaderSource, uint type)
{
	uint id = 0;
	
	id = glCreateShader(type);
	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	
	//Check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		LOG("Error compiling shader: %s", infoLog);
	}
	
	return id;
	
}

uint Shader::CreateShader(const char*& vertexShader, const char*& fragmentShader)
{
	
	//Compile shaders
	uint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	uint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	//Create program
	uint program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	//Check for errors
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		LOG("Error linking program: %s", infoLog);
	}

	//Delete shaders (already linked into program)
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

//uint Shader::compileShader(const char* shaderCode, GLenum shaderType)
//{
//	uint shader = 0;
//
//	shader = glCreateShader(shaderType);
//	glShaderSource(shader, 1, &shaderCode, NULL);
//	glCompileShader(shader);
//
//	//Check for errors
//	int success;
//	char infoLog[512];
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(shader, 512, NULL, infoLog);
//		LOG("Error compiling shader: %s", infoLog);
//	}
//
//	return shader;
//}