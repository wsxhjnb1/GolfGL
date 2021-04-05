#pragma once

namespace Render {

	class Renderer;
	class Mesh;
	
	class Shader
	{
	public:
		Shader(const std::string& shaderName);

		bool Compile();

		inline void ActivateShader()
        {
            glUseProgram( m_ID );
        }
		template <typename T = int>
		void SetValue(const std::string& name, T value) const;
		
		void SetValue(const std::string& name, float value) const;
		
		void SetValue(const char* name, float val1, float val2, float val3) const;

		void setVec2(const std::string& name, const glm::vec2& value) const;

		void setVec2(const std::string& name, float x, float y) const;

		void setVec3(const std::string& name, const glm::vec3& value) const;

		void setVec3(const std::string& name, float x, float y, float z) const;

		void setVec4(const std::string& name, const glm::vec4& value) const;

		void setVec4(const std::string& name, float x, float y, float z, float w);

		void setMat2(const std::string& name, const glm::mat2& mat) const;

		void setMat3(const std::string& name, const glm::mat3& mat) const;

		void setMat4(const std::string& name, const glm::mat4& mat) const;

		inline	static const char* shadersDir = "Resources/Shaders/";

	private:
		unsigned m_ID;
		std::string m_name;
				
		bool checkCompileErrors(unsigned int shader, const char* type) const;


		friend Renderer;
		friend Mesh;
	};
    template <typename T> 
	inline void Shader::SetValue( const std::string &name, T value ) const
    {
        glUniform1i( glGetUniformLocation( m_ID, name.c_str() ), value );
    }
} // namespace Render