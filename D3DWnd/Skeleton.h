#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
};

struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<myVector3> positions = {};
	//std::vector<glm::quat> rotations = {};
	std::vector<myVector3> scales = {};
};

struct Animation {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

// a recursive function to read all bones and form skeleton
bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, MyMatrix4>>& boneInfoTable) {

	if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end()) { // if node is actually a bone
		boneOutput.name = node->mName.C_Str();
		boneOutput.id = boneInfoTable[boneOutput.name].first;
		boneOutput.offset = boneInfoTable[boneOutput.name].second;

		for (int i = 0; i < node->mNumChildren; i++) {
			Bone child;
			readSkeleton(child, node->mChildren[i], boneInfoTable);
			boneOutput.children.push_back(child);
		}
		return true;
	}
	else { // find bones in children
		for (int i = 0; i < node->mNumChildren; i++) {
			if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable)) {
				return true;
			}

		}
	}
	return false;
}

void loadModel(const aiScene* scene, aiMesh* mesh, std::vector<MODEL_VERTEX>& verticesOutput, std::vector<unsigned int>& indicesOutput, Bone& skeletonOutput, unsigned int& nBoneCount) {

	verticesOutput = {};
	indicesOutput = {};
	//load position, normal, uv
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		//process position 
		MODEL_VERTEX vertex;
		myVector3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.x = vector.x;
		vertex.y = vector.y;
		vertex.z = vector.z;
		
		////process normal
		//vector.x = mesh->mNormals[i].x;
		//vector.y = mesh->mNormals[i].y;
		//vector.z = mesh->mNormals[i].z;
		//vertex.normal = vector;
		
		//process uv
		/*glm::vec2 vec;
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;*/

		vertex.u = mesh->mTextureCoords[0][i].x;
		vertex.v = mesh->mTextureCoords[0][i].y;

		for (int i = 0; i < 4; i++)
		{
			vertex.boneIndices[i] = 0.0f;
			vertex.blendWeight[i] = 0.0f;
		}
		verticesOutput.push_back(vertex);
	}

	//load boneData to vertices
	std::unordered_map<std::string, std::pair<int, MyMatrix4>> boneInfo = {};
	std::vector<unsigned int> boneCounts;
	boneCounts.resize(verticesOutput.size(), 0);
	nBoneCount = mesh->mNumBones;

	//loop through each bone
	for (int i = 0; i < nBoneCount; i++) {
		aiBone* bone = mesh->mBones[i];

		//////////////////// OJO!!!!!!!!!!!!!!!!!!!!!!!!! Row Major o Column Major?!!!!!!!!!!!!! //////////

		MyMatrix4 m = MyMatrix4(bone->mOffsetMatrix.a1, bone->mOffsetMatrix.b1, bone->mOffsetMatrix.c1, bone->mOffsetMatrix.d1,
								bone->mOffsetMatrix.a2, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.d2,
								bone->mOffsetMatrix.a3, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.d3,
								bone->mOffsetMatrix.a4, bone->mOffsetMatrix.b4, bone->mOffsetMatrix.c4, bone->mOffsetMatrix.d4);

		boneInfo[bone->mName.C_Str()] = { i, m };

		//loop through each vertex that have that bone
		for (int j = 0; j < bone->mNumWeights; j++) {
			unsigned int id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;
			switch (boneCounts[id]) {
			case 1:
				verticesOutput[id].boneIndices[0] = i;
				verticesOutput[id].blendWeight[0] = weight;
				break;
			case 2:
				verticesOutput[id].boneIndices[1] = i;
				verticesOutput[id].blendWeight[1] = weight;
				break;
			case 3:
				verticesOutput[id].boneIndices[2] = i;
				verticesOutput[id].blendWeight[2] = weight;
				break;
			case 4:
				verticesOutput[id].boneIndices[3] = i;
				verticesOutput[id].blendWeight[3] = weight;
				break;
			default:
				//std::cout << "err: unable to allocate bone to vertex" << std::endl;
				break;

			}
		}
	}



	//normalize weights to make all weights sum 1

	///////////////??????????????????????????//////////////////////////////////

	for (int i = 0; i < verticesOutput.size(); i++) {
		float boneWeights[4];

			  boneWeights[0] = verticesOutput[i].blendWeight[0];
			  boneWeights[1] = verticesOutput[i].blendWeight[1];
			  boneWeights[2] = verticesOutput[i].blendWeight[2];
			  boneWeights[3] = verticesOutput[i].blendWeight[3];

		float totalWeight = boneWeights[0] + boneWeights[1] + boneWeights[2] + boneWeights[3];
		if (totalWeight > 0.0f) 
		{
			verticesOutput[i].blendWeight[0] = boneWeights[0] / totalWeight;
			verticesOutput[i].blendWeight[1] = boneWeights[1] / totalWeight;
			verticesOutput[i].blendWeight[2] = boneWeights[2] / totalWeight;
			verticesOutput[i].blendWeight[3] = boneWeights[3] / totalWeight;
		}
	}


	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicesOutput.push_back(face.mIndices[j]);
	}

	// create bone hirerchy
	readSkeleton(skeletonOutput, scene->mRootNode, boneInfo);
}

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
