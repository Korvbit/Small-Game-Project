#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "Mesh.h"
#include "Texture.h"

using namespace std;

class Model
{
public:
	Model(std::string path = "Models/Cube/cube.obj", bool gammaCorrection = false);
	virtual ~Model();

	inline std::vector<Mesh*> GetMeshes() const { return this->meshes; };
	inline std::string GetDirectory() const { return this->directoryPath; };

	void Draw(Shader* shader);

private:
	std::vector<Texture*> loadedTextures;
	bool gammaCorrection;
	std::vector<Mesh*> meshes;
	std::string directoryPath;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	void LoadTexture(std::string path);
};

#endif //MODEL_H