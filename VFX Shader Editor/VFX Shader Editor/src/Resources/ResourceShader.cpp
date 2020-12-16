#include "Application.h"
#include "ResourceShader.h"

#include "ModuleResources.h"
#include <string>
#include <fstream> 
#include <sstream> 


ResourceShader::ResourceShader()
{
	type = Type::RESOURCE_SHADER;
}
ResourceShader::ResourceShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	type = Type::RESOURCE_SHADER;

	//Read vertex and fragment shaders files
	std::string vertexString = ReadShaderFromFile(vertexShaderPath);
	const char* vertexSource = vertexString.c_str();

	std::string fragString = ReadShaderFromFile(fragmentShaderPath);
	const char* fragmentSource = fragString.c_str();


	//const char* vShaderCode2 = getShaderCode(vertexShaderPath).c_str(); error junk chars
	
	std::cout << "\nVERTEX SHADER: \n\n" << vertexSource << std::endl;

	std::cout << "\nFRAGMENT SHADER: \n\n" << fragmentSource << std::endl;

	programID = CreateShader(vertexSource, fragmentSource);
}


std::string ResourceShader::ReadShaderFromFile(const char* filename)
{
	std::ifstream file(filename, std::ios::in);
	std::string content;
	if (file.good())
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
	}
	return content;
}

ResourceShader::~ResourceShader()
{
	glDeleteProgram(programID);
}

void ResourceShader::Bind()
{
	glUseProgram(programID);
}

void ResourceShader::Unbind()
{
	glUseProgram(0);
}

bool ResourceShader::LoadMemory()
{
	//Read vertex and fragment shaders files
	std::string vertexString = ReadShaderFromFile(vertex_path);
	const char* vertexSource = vertexString.c_str();

	std::string fragString = ReadShaderFromFile(fragment_path);
	const char* fragmentSource = fragString.c_str();


	//const char* vShaderCode2 = getShaderCode(vertexShaderPath).c_str(); error junk chars

	std::cout << "\nVERTEX SHADER: \n\n" << vertexSource << std::endl;

	std::cout << "\nFRAGMENT SHADER: \n\n" << fragmentSource << std::endl;

	//Compile Shader
	programID = CreateShader(vertexSource, fragmentSource);
	return true;
}

bool ResourceShader::FreeMemory()
{
	 glDeleteProgram(programID);
	 return true;
}

void ResourceShader::LoadShaderResource(const char* vertexSource, const char* fragmentSource)
{
	this->vertex_path = vertexSource;
	this->fragment_path = fragmentSource;

	App->resources->AddResource(this);
}

void ResourceShader::SetUniformMat4f(const char* name, float* value) const
{
	int location = glGetUniformLocation(programID, name);
	if(location == -1)
		std::cout << "[WARNING]: uniform: " << name << "doesn't exist!" << std::endl;

	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}


uint ResourceShader::CompileShader(const char* shaderSource, GLenum shaderType)
{
	uint shader = 0;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
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

uint ResourceShader::CreateShader(const char* vertexSource, const char* fragmentSource)
{
	//Compile both shaders
	uint vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	uint fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	//Create program
	uint program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
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
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
