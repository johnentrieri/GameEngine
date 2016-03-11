#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>



GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath)
{
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		fatalError("Could not create Vertex Shader");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		fatalError("Could not create Fragment Shader");
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);	

}

void GLSLProgram::linkShaders()
{
	_programID = glCreateProgram();

	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	glLinkProgram(_programID);

	//error check
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Could not link Shaders");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);

	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string &attributeName)
{
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

void GLSLProgram::use()
{
	glUseProgram(_programID);

	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);

	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}


void GLSLProgram::compileShader(const std::string &filePath, GLuint id)
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail())
	{
		fatalError("Failed to open " + filePath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char *contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	//error check
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success = GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + filePath + " failed to compile");
	}
}