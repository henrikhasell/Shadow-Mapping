#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <assimp/mesh.h>

class Mesh
{
	friend class Renderer;
	friend class ShadowMap;
public:
	Mesh();
	~Mesh();
	void load(
		const GLfloat *vertexArray,
		const GLfloat *normalArray,
		const GLfloat *texCoordArray,
		const GLuint *indexArray,
		GLsizei v_count,
		GLsizei i_count);
	void load(const aiMesh *mesh);
private:
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint texCoordBuffer;
	GLuint indexBuffer;
	GLsizei indices;
};

#endif