#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Skeleton
{
public:

};

//bool nodeIsBone(const aiNode* node, const aiScene* scene) {
//	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
//		const aiMesh* mesh = scene->mMeshes[i];
//		for (unsigned int j = 0; j < mesh->mNumBones; j++) {
//			if (std::string(mesh->mBones[j]->mName.C_Str()) == node->mName.C_Str()) {
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//void ProcessSkeleton(const aiNode* node, const aiScene* scene) {
//	for (unsigned int i = 0; i < node->mNumChildren; i++) {
//		ProcessSkeleton(node->mChildren[i], scene);
//	}
//
//	// Si este nodo es un hueso, obtén su información
//	if (nodeIsBone(node, scene)) {
//		string x = node->mName.C_Str();
//		// Procesa la información del hueso, como su nombre, posición, rotación, etc.
//	}
//}
