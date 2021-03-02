#include "GL/glew.h"
#include "Program.h"
#include "GLMath.h"
#include <vector>
#include <string>
#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model {
private:
	std::vector<Mesh*> meshes;
	std::string directory;
	std::vector<MeshTexture> textures_loaded;

public:
	Model(std::string path);
	~Model();

	void draw(Program& program);

private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};