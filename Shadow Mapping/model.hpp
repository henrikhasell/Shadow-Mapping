#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <assimp/scene.h>
#include "mesh.hpp"

class Model
{
	friend class Renderer;
	friend class ShadowMap;
public:
	Model();
	~Model();
	bool load(const char path[]);
	bool load(const aiScene *scene);
private:
	std::vector<Mesh> meshes;
};

#endif