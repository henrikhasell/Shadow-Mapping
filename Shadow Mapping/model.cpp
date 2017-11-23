#include "model.hpp"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model()
{

}

Model::~Model()
{

}

bool Model::load(const char path[])
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_SortByPType |
		aiProcess_FlipUVs
	);

	if(scene)
	{
		load(scene);
	}
	else
	{
		std::cerr << "Failed to load " << path << std::endl;
	}

	return scene != nullptr;
}

bool Model::load(const aiScene *scene)
{
	meshes.resize(scene->mNumMeshes);

	for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes[i].load(scene->mMeshes[i]);
	}

	return true;
}