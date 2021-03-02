#include "GLVAO.h"

using namespace Hydro;

GLVAO::GLVAO() : id(0){
	glGenVertexArrays(1, &id);
}

GLVAO::~GLVAO(){
	glDeleteVertexArrays(1, &id);
}

void GLVAO::Bind() const{
	glBindVertexArray(id);
}

void GLVAO::Unbind() const{
	glBindVertexArray(0);
}

void GLVAO::SetupVertexAttribute(unsigned int id_, unsigned int num_, const unsigned int stride_, const GLvoid* offset_){
	glVertexAttribPointer(id_, num_, GL_FLOAT, GL_FALSE, stride_, offset_);
	glEnableVertexAttribArray(id_);
}

void GLVAO::SetupVertexIntAttribute(unsigned int id_, unsigned int num_, const unsigned int stride_, const GLvoid* offset_){
	glVertexAttribIPointer(id_, num_, GL_INT, stride_, offset_);
	glEnableVertexAttribArray(id_);
}