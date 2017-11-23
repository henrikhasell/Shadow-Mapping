#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <assimp/mesh.h>
#include <assimp/scene.h>

class AnimatedMesh
{
public:
	AnimatedMesh();
	~AnimatedMesh();
	bool load(const aiMesh *mesh);
};

class AnimatedModel
{
public:
	AnimatedModel();
	~AnimatedModel();
	bool load(const char path[]);
	bool load(const aiScene *scene);
};

#endif