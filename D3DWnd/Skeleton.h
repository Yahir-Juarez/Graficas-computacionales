#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

#include "myVector3.h"
#include "myMatrix.h"

struct MODEL_VERTEX
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float blendWeight[4];  // Pesos de mezcla (hasta 4 huesos)
	float boneIndices[4]; // Índices de huesos (correspondientes a los pesos)

	//////////////////////////////////////////////////////////////////////////////////////////

	float instance_Quaternion[4];       // Cuaternión de la instancia
	float instance_PositionAndScale[4]; // Posición y escala de la instancia
	float instance_ScaleYZ[2];          // Escala YZ de la instancia
	float instance_ColorAndId[4];       // Color e ID de la instancia
};


class Bone
{
public:
	int id = 0; // position of the bone in final upload array
	std::string name = "";
	MyMatrix4 offset = MyMatrix4(1.0f, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0);

	std::vector<Bone> children = {};

	static bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, MyMatrix4>>& boneInfoTable);
	static void loadModel(const aiScene* scene, aiMesh* mesh, std::vector<MODEL_VERTEX>& verticesOutput, std::vector<unsigned int>& indicesOutput, Bone& skeletonOutput, unsigned int& nBoneCount);
};

struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<myVector3> positions = {};
	std::vector<MyQuaternion> rotations = {};
	std::vector<myVector3> scales = {};
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
