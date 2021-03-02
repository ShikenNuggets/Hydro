#include "GLRenderInfo.h"

#include "Debug.h"

using namespace Hydro;

GLRenderInfo::GLRenderInfo(Model* model_, Texture* texture_) : RenderInfo(model_, texture_), vao(), vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER){
	Debug::Assert(model_ != nullptr && texture_ != nullptr);

	vao.Bind();
	vbo.Bind();
	vbo.SetBufferData(model->vertices.size() * sizeof(Vertex), &model->vertices[0], GL_STATIC_DRAW);

	ebo.Bind();
	ebo.SetBufferData(model->indices.size() * sizeof(uint32_t), &model->indices[0], GL_STATIC_DRAW);

	vao.SetupVertexAttribute(0, 3, sizeof(Vertex), (GLvoid*)(0));
	vao.SetupVertexAttribute(1, 3, sizeof(Vertex), (GLvoid*)(sizeof(Vector3)));
	vao.SetupVertexAttribute(2, 2, sizeof(Vertex), ((GLvoid*)(sizeof(Vector3) + sizeof(Vector3))));

	vbo.Unbind();
	vao.Unbind();
	ebo.Unbind();
}

void GLRenderInfo::Render(){
	//Bind the VAO that you want to use for drawing
	vao.Bind();
	ebo.Bind();

	//Render the array stored in the bound VAO by the indices in the EBO
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model->indices.size()), GL_UNSIGNED_INT, 0);

	//Clear the vertex array for future use
	vao.Unbind();
	ebo.Unbind();
}