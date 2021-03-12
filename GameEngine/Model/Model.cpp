#pragma once
#include "Model.h"
#include <iostream>
#include "stb_image.h"

Model::Model(std::string path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	
	processNode(scene->mRootNode,scene);
}

Model::~Model(){
	for (unsigned int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
}

void Model::draw(Program& program){
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->draw(program);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene){
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene){
	std::vector<Vertex>* vertices = new std::vector<Vertex>;
	std::vector<unsigned int>* indices = new std::vector<unsigned int>;
	std::vector<MeshTexture>* textures = new std::vector<MeshTexture>;
	Material meshMat = {0};

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.position = { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z};

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord = { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y };
		}
		else {
			vertex.texCoord = { 0.0f , 0.0f };
		}

		vertices->push_back(vertex);
	}

	for (unsigned int i = 0;i< mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures->insert(textures->end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures->insert(textures->end(), specularMaps.begin(), specularMaps.end());

		aiColor3D color;
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		meshMat.ambient = {color.r,color.g ,color.b };

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		meshMat.diffuse = { color.r,color.g ,color.b };

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		meshMat.specular = { color.r,color.g ,color.b };

		float shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);
		meshMat.shininess = shininess;
	}


	if (meshMat.shininess == 0.0f)
		return new Mesh(vertices, indices, textures);
	else
		return new Mesh(vertices, indices, textures, meshMat);
}

int TextureFromFile(const char* path, std::string directory);

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){

	std::vector<MeshTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type,i,&str);
		bool skip = false;

		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			MeshTexture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

int TextureFromFile(const char* path, std::string directory) {
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;

	unsigned int textureID;
	glGenTextures(1,&textureID);

	int width, height, bpp;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

	if (image == nullptr) {
		std::cout << "ERROR::ASSIMP::Image not found::" << fileName << std::endl;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
	}

	return textureID;
}












