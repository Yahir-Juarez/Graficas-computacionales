#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Bone
{
public:
	int id = 0; // position of the bone in final upload array
	std::string name = "";
	//glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};
};

struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	//std::vector<glm::vec3> positions = {};
	//std::vector<glm::quat> rotations = {};
	//std::vector<glm::vec3> scales = {};
};

struct Animation {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
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
