#ifndef HYDRO_GL_SHADER_H
#define HYDRO_GL_SHADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "GFX/Color.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Hydro{
	//Forward declarations
	class GLUniform;
	class GLShader;

	class GLUniform{
	public:
		GLUniform(GLShader* shader_, const std::string& name_);

		GLuint id;
	};

	class GLShader{
	public:
		GLShader(const std::string& vertPath_, const std::string& fragPath_);
		~GLShader();

		void Bind();
		void Unbind();

		GLuint GetUniformID(const std::string& value_) const;

		void BindInt(GLuint uniformID_, int value_);
		void BindInt(const GLUniform& uniform_, int value_);
		void BindInt(const std::string& uniformName_, int value_);

		void BindFloat(GLuint uniformID_, float value_);
		void BindFloat(const GLUniform& uniform_, float value_);
		void BindFloat(const std::string& uniformName_, float value_);

		void BindVector2(GLuint uniformID_, const Vector2& value_);
		void BindVector2(const GLUniform& uniform_, const Vector2& value_);
		void BindVector2(const std::string& uniformName_, const Vector2& value_);

		void BindVector3(GLuint uniformID_, const Vector3& value_);
		void BindVector3(const GLUniform& uniform_, const Vector3& value_);
		void BindVector3(const std::string& uniformName_, const Vector3& value_);

		void BindColor(GLuint uniformID_, const Color& value_);
		void BindColor(const GLUniform& uniform_, const Color& value_);
		void BindColor(const std::string& uniformName_, const Color& value_);

		void BindMatrix3(GLuint uniformID_, const Matrix3& value_);
		void BindMatrix3(const GLUniform& uniform_, const Matrix3& value_);
		void BindMatrix3(const std::string& uniformName_, const Matrix3& value_);

		void BindMatrix4(GLuint uniformID_, const Matrix4& value_);
		void BindMatrix4(const GLUniform& uniform_, const Matrix4& value_);
		void BindMatrix4(const std::string& uniformName_, const Matrix4& value_);

		void BindTexture(int textureNumber_, GLuint textureID_);
		void BindTexture(const GLUniform& uniform_, GLuint textureID_);
		void BindTexture(const std::string& uniformName_, GLuint textureID_);

		void BindTexture3D(int textureNumber_, GLuint textureID_);
		void BindTexture3D(const GLUniform& uniform_, GLuint textureID_);
		void BindTexture3D(const std::string& uniformName_, GLuint textureID_);

		void BindCubeMap(int textureNumber_, GLuint textureID_);
		void UnbindTexture(int textureNumber_);
		void UnbindCubeMap(int textureNumber_);

	private:
		GLuint shader;
		std::unordered_map<std::string, GLUniform*> uniforms;
		int currentBoundTextures;

		static int maxTextureUnits; //TODO - Should this be part of GLRenderer instead?

		std::string GetShaderLog(GLuint shader_);
	};
}

#endif //!HYDRO_GL_SHADER_H