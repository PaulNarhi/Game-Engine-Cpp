#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string& filepath)
	: m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.Vertex, source.Fragment);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile" <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());

	if (!program) {
		std::cout << "Error!" << std::endl;
	}

	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform4f(location, f0, f1, f2, f3));
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform3f(location, f0, f1, f2));
}

void Shader::SetUniform2f(const std::string& name, float f0, float f1)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform2f(location, f0, f1));
}

void Shader::SetUniform1f(const std::string& name, float f0)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform1f(location, f0));
}

void Shader::SetUniform4i(const std::string& name, int i0, int i1, int i2, int i3)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform4i(location, i0, i1, i2, i3));
}

void Shader::SetUniform3i(const std::string& name, int i0, int i1, int i2)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform3i(location, i0, i1, i2));
}

void Shader::SetUniform2i(const std::string& name, int i0, int i1)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform2i(location, i0, i1));
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform1i(location, i0));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	int location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
