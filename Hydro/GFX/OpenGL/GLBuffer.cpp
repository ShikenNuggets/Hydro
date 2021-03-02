#include "GLBuffer.h"

using namespace Hydro;

GLBuffer::GLBuffer(GLenum type_) : bufferType(type_){
	glGenBuffers(1, &id);
}

GLBuffer::~GLBuffer(){
	glDeleteBuffers(1, &id);
}

void GLBuffer::Bind() const{
	glBindBuffer(bufferType, id);
}

void GLBuffer::Unbind() const{
	glBindBuffer(bufferType, 0);
}

void GLBuffer::SetBufferData(size_t size_, const void* data_, GLenum usage_){
	glBufferData(bufferType, size_, data_, usage_);
}

void GLBuffer::SetBufferSubData(GLintptr offset_, GLsizeiptr size_, const GLvoid* data_){
	glBufferSubData(bufferType, offset_, size_, data_);
}