#include <precomp.h>
#include "Shader.h"


Render::Shader::Shader(const std::string& shaderName)
{		
	std::string shaderPath{ shadersDir + shaderName };	
	std::string vertexCode;
	std::string fragCode;
	std::ifstream vertexStream;
	std::ifstream fragStream;

	vertexStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);	

	try
	{		
		vertexStream.open( shaderPath + (".vs") );
		fragStream.open( shaderPath + (".fs") );
		std::stringstream vertexBuf;
		vertexBuf << vertexStream.rdbuf();
		std::stringstream fragBuf;
		fragBuf << fragStream.rdbuf();

		vertexCode = vertexBuf.str();
		fragCode = fragBuf.str();
	}
	catch (std::ifstream::failure& e)
	{
		// LOG HERE INSTEAD
		LOG_ERROR("ERROR: Shader error (path= {} ):\n{}", shaderPath, e.what());
		
	}
	
	const char* vShaderCode = vertexCode.c_str();
	auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	const char* fShaderCode = fragCode.c_str();
	auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	checkCompileErrors(m_ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Render::Shader::ActivateShader()
{
	glUseProgram(m_ID);
}

void Render::Shader::SetValue(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}


void Render::Shader::SetValue(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}


void Render::Shader::SetValue(const char* name, float val1, float val2, float val3) const
{
	glUniform3f( glGetUniformLocation(m_ID, name), val1, val2, val3 );
}

void Render::Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}
void Render::Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Render::Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}
void Render::Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Render::Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}
void Render::Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Render::Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Render::Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Render::Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

inline void Render::Shader::checkCompileErrors(unsigned int shader, const char* type) const
{
	int success;
	char infoLog[1024];
	if (std::strcmp(type, "PROGRAM"))
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {} \n{} \n-- -------------------------------------------------- - -- \n", type, infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n -- --------------------------------------------------- -- ", type, infoLog);
		}
	}
}
