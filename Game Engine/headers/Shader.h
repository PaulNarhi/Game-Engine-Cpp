#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource
{
	std::string Vertex;
	std::string Fragment;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniform3f(const std::string& name, float f0, float f1, float f2);
	void SetUniform2f(const std::string& name, float f0, float f1);
	void SetUniform1f(const std::string& name, float f0);

	void SetUniform4i(const std::string& name, int i0, int i1, int i2, int i3);
	void SetUniform3i(const std::string& name, int i0, int i1, int i2);
	void SetUniform2i(const std::string& name, int i0, int i1);
	void SetUniform1i(const std::string& name, int i0);

	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

private:
	int GetUniformLocation(const std::string& name);

	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource ParseShader(const std::string& filepath);
};
