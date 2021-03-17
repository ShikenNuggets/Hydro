#include "GLShader.h"

#include "FileSystem.h"
#include "GFX/OpenGL/GLRenderer.h"

using namespace Hydro;

GLUniform::GLUniform(GLShader* shader_, const std::string& name_) : id(0){
	id = shader_->GetUniformID(name_);
}

int GLShader::maxTextureUnits = 0;

GLShader::GLShader(const std::string& vertPath_, const std::string& fragPath_) : shader(0), uniforms(), currentBoundTextures(0){
	std::string vString = FileSystem::ReadFileToString(vertPath_);
	std::string fString = FileSystem::ReadFileToString(fragPath_);

	//Add in code shared by all shaders if include directive is found
	//std::string toReplace = "#include \"_shared.glsl\"";
	//size_t pos = vString.find(toReplace);
	//if(pos != std::string::npos){
	//	vString = vString.replace(pos, toReplace.size(), GLRenderer::GetSharedShaderCode());
	//}

	//pos = fString.find(toReplace);
	//if(pos != std::string::npos){
	//	fString = fString.replace(pos, toReplace.size(), GLRenderer::GetSharedShaderCode());
	//}

	if(vString.empty() || fString.empty()){
		Debug::LogError("Could not load shader files!", __FILE__, __LINE__);
		throw std::exception("Could not load shader files!");
	}

	//Our shader code needs to be stored in a const char* to be passed to OpenGL
	const char* vsText = vString.c_str();
	const char* fsText = fString.c_str();

	//GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Check for errors
	if(vertShader == 0 || fragShader == 0){
		Debug::LogError("Can't create a new shader!", __FILE__, __LINE__);
		throw std::exception("Can't create a new shader!");
	}

	glShaderSource(vertShader, 1, &vsText, 0);
	glShaderSource(fragShader, 1, &fsText, 0);

	glCompileShader(vertShader);
	//Check for errors
	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		Debug::LogError("VERT: " + GetShaderLog(vertShader));
		throw std::exception("Vertex shader compile error!");
	}

	glCompileShader(fragShader);
	//Check for errors
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		Debug::LogError("FRAG: " + GetShaderLog(fragShader));
		throw std::exception("Fragment shader compile error!");
	}

	shader = glCreateProgram();
	glAttachShader(shader, fragShader);
	glAttachShader(shader, vertShader);

	//--------------------------------------------------------------------------------------------------------

	glLinkProgram(shader);
	/// Check for errors
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		Debug::LogError(GetShaderLog(shader));
		throw std::exception("Shader linking error!");
	}

	glDeleteShader(fragShader);
	glDeleteShader(vertShader);
	glDetachShader(shader, fragShader);
	glDetachShader(shader, vertShader);

	//--------------------------------------------------------------------------------------------------------

	int count;
	glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &count);
	Debug::Log("There are " + std::to_string(count) + " active uniforms on shader " + vertPath_, __FILE__, __LINE__);

	//--------------------------------------------------------------------------------------------------------

	GLint maxUniformLength = 0;
	glGetProgramiv(shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

	GLchar* name = new GLchar[maxUniformLength];
	GLint size;
	GLenum type;
	GLsizei length;

	for(GLuint i = 0; i < static_cast<GLuint>(count); i++){
		glGetActiveUniform(shader, i, 1024, &length, &size, &type, name);

		if(size <= 1){
			uniforms[name] = new GLUniform(this, name);
			continue;
		}

		//Handle arrays of uniforms
		std::string uniformName = std::string(name).substr(0, length - 3);
		for(int j = 0; j < size; j++){
			std::string newName = uniformName + "[" + std::to_string(j) + "]";
			uniforms[newName] = new GLUniform(this, newName);
		}
	}

	//We allocated our character buffer with new[] so we have to delete it with delete[]
	delete[] name;

	if(maxTextureUnits == 0){
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	}
}

GLShader::~GLShader(){
	for(auto& u : uniforms){
		delete u.second;
	}

	if(shader != 0){
		glDeleteProgram(shader);
		shader = 0;
	}
}

void GLShader::Bind(){
	glUseProgram(shader);
}

void GLShader::Unbind(){
	int boundTextures = GL_TEXTURE0 + currentBoundTextures;
	for(int i = GL_TEXTURE0; i < boundTextures; i++){
		UnbindTexture(i);
	}

	glUseProgram(0);
}

GLuint GLShader::GetUniformID(const std::string& uniformName_) const{
	_ASSERT(!uniformName_.empty());
	_ASSERT(glGetUniformLocation(shader, uniformName_.c_str()) != GL_INVALID_INDEX);
	return glGetUniformLocation(shader, uniformName_.c_str());
}

std::string GLShader::GetShaderLog(GLuint shader_){
	std::string errorLog;

	GLsizei errorLogSize = 0;
	glGetProgramiv(shader_, GL_INFO_LOG_LENGTH, &errorLogSize);
	errorLog.resize(errorLogSize);
	glGetProgramInfoLog(shader_, errorLogSize, &errorLogSize, &errorLog[0]);

	return errorLog;
}

void GLShader::BindInt(GLuint uniformID_, int value_){
	glUniform1iv(uniformID_, 1, &value_);
}

void GLShader::BindInt(const GLUniform& uniform_, int value_){
	BindInt(uniform_.id, value_);
}

void GLShader::BindInt(const std::string& uniformName_, int value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindInt(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindInt(GetUniformID(uniformName_), value_);
}

void GLShader::BindFloat(GLuint uniformID_, float value_){
	glUniform1fv(uniformID_, 1, &value_);
}

void GLShader::BindFloat(const GLUniform& uniform_, float value_){
	BindFloat(uniform_.id, value_);
}

void GLShader::BindFloat(const std::string& uniformName_, float value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindFloat(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindFloat(GetUniformID(uniformName_), value_);
}

void GLShader::BindVector2(GLuint uniformID_, const Vector2& value_){
	glUniform2fv(uniformID_, 1, value_);
}

void GLShader::BindVector2(const GLUniform& uniform_, const Vector2& value_){
	BindVector2(uniform_.id, value_);
}

void GLShader::BindVector2(const std::string& uniformName_, const Vector2& value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindVector2(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindVector2(GetUniformID(uniformName_), value_);
}

void GLShader::BindVector3(GLuint uniformID_, const Vector3& value_){
	glUniform3fv(uniformID_, 1, value_);
}

void GLShader::BindVector3(const GLUniform& uniform_, const Vector3& value_){
	BindVector3(uniform_.id, value_);
}

void GLShader::BindVector3(const std::string& uniformName_, const Vector3& value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindVector3(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindVector3(GetUniformID(uniformName_), value_);
}

void GLShader::BindColor(GLuint uniformID_, const Color& value_){
	glUniform4fv(uniformID_, 1, value_);
}

void GLShader::BindColor(const GLUniform& uniform_, const Color& value_){
	BindColor(uniform_.id, value_);
}

void GLShader::BindColor(const std::string& uniformName_, const Color& value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindColor(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindColor(GetUniformID(uniformName_), value_);
}

void GLShader::BindMatrix3(GLuint uniformID_, const Matrix3& value_){
	glUniformMatrix3fv(uniformID_, 1, GL_FALSE, value_);
}

void GLShader::BindMatrix3(const GLUniform& uniform_, const Matrix3& value_){
	BindMatrix3(uniform_.id, value_);
}

void GLShader::BindMatrix3(const std::string& uniformName_, const Matrix3& value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindMatrix3(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindMatrix3(GetUniformID(uniformName_), value_);
}

void GLShader::BindMatrix4(GLuint uniformID_, const Matrix4& value_){
	glUniformMatrix4fv(uniformID_, 1, GL_FALSE, value_);
}

void GLShader::BindMatrix4(const GLUniform& uniform_, const Matrix4& value_){
	BindMatrix4(uniform_.id, value_);
}

void GLShader::BindMatrix4(const std::string& uniformName_, const Matrix4& value_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindMatrix4(search->second->id, value_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindMatrix4(GetUniformID(uniformName_), value_);
}

void GLShader::BindTexture(int textureNumber_, GLuint textureID_){
	glActiveTexture(textureNumber_);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID_);
	glDisable(GL_TEXTURE_2D);
	currentBoundTextures++;
}

void GLShader::BindTexture(const GLUniform& uniform_, GLuint textureID_){
	if(currentBoundTextures >= maxTextureUnits){
		Debug::LogError("Attempted to bind invalid texture number!", __FILE__, __LINE__);
		return;
	}

	glUniform1i(uniform_.id, currentBoundTextures);
	BindTexture(GL_TEXTURE0 + currentBoundTextures, textureID_);
}

void GLShader::BindTexture(const std::string& uniformName_, GLuint textureID_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindTexture(*search->second, textureID_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindTexture(GetUniformID(uniformName_), textureID_);
}

void GLShader::BindTexture3D(int textureNumber_, GLuint textureID_){
	glActiveTexture(textureNumber_);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, textureID_);
	glDisable(GL_TEXTURE_3D);
	currentBoundTextures++;
}

void GLShader::BindTexture3D(const GLUniform& uniform_, GLuint textureID_){
	if(currentBoundTextures >= maxTextureUnits){
		Debug::LogError("Attempted to bind invalid texture number!", __FILE__, __LINE__);
		return;
	}

	glUniform1i(uniform_.id, currentBoundTextures);
	BindTexture3D(GL_TEXTURE0 + currentBoundTextures, textureID_);
}

void GLShader::BindTexture3D(const std::string& uniformName_, GLuint textureID_){
	auto search = uniforms.find(uniformName_);
	if(search != uniforms.end()){
		BindTexture3D(*search->second, textureID_);
		return;
	}

	Debug::LogWarning("Uniform [" + uniformName_ + "] not found in uniform list!", __FILE__, __LINE__);
	BindTexture3D(GetUniformID(uniformName_), textureID_);
}

void GLShader::BindCubeMap(int textureNumber_, GLuint textureID_){
	glActiveTexture(textureNumber_);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void GLShader::UnbindTexture(int textureNumber_){
	glActiveTexture(textureNumber_);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	currentBoundTextures--;
}

void GLShader::UnbindCubeMap(int textureNumber_){
	glActiveTexture(textureNumber_);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}