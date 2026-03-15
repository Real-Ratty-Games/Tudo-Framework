/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
--------------------------------------------------------
	Valid Arguments:
	<input> <output>
======================================================*/
#include <iostream>
#include <fstream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SystemTypes.hpp"
#include "Globals.hpp"
#include "FileSystem.hpp"

using namespace Tudo;

static std::vector<MeshData> _Model;

static void Load(strgv filename);
static void ProcessNode(aiNode* node, const aiScene* scene);
static void ProcessMesh(aiMesh* mesh, const aiScene* scene);
static void Save(strgv filename);

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		Load(argv[1]);
		Save(argv[2]);
	}
	else std::cout << "Invalid Arguments" << std::endl;
	return 0;
}

void Load(strgv filename)
{
	if (!FileSystem::Exists(filename))
	{
		std::cout << "Model file could not be found: " << filename << std::endl;
		return;
	}

	Assimp::Importer importer;
	uint flags = aiProcess_FlipUVs
		| aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_SplitLargeMeshes
		| aiProcess_OptimizeMeshes
		| aiProcess_MakeLeftHanded
		| aiProcess_FlipWindingOrder;

	const aiScene* scene = importer.ReadFile(filename.data(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed loading model: " << filename << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene);
}

void ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	MeshData result;
	result.Vertices.resize(mesh->mNumVertices);

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex& vertex = result.Vertices[i];

		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.NX = mesh->mNormals[i].x;
			vertex.NY = mesh->mNormals[i].y;
			vertex.NZ = mesh->mNormals[i].z;
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.U = mesh->mTextureCoords[0][i].x;
			vertex.V = mesh->mTextureCoords[0][i].y;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.TX = mesh->mTangents[i].x;
			vertex.TY = mesh->mTangents[i].y;
			vertex.TZ = mesh->mTangents[i].z;

			vertex.BX = mesh->mBitangents[i].x;
			vertex.BY = mesh->mBitangents[i].y;
			vertex.BZ = mesh->mBitangents[i].z;
		}
	}

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			result.Indices.push_back(face.mIndices[j]);
	}

	_Model.push_back(result);
}

void Save(strgv filename)
{
	std::ofstream out(filename.data(), std::ios::binary | std::ios::trunc);

	if (!out)
	{
		std::cout << "Failed to create file: " << filename << std::endl;
		return;
	}

	M3DCHeader header{ M3DC_MAGIC , M3DC_VERSION , _Model.size() };
	out.write(reinterpret_cast<const char*>(&header), sizeof(M3DCHeader));

	for (auto& mesh : _Model)
	{
		const uint64 vsize = mesh.Vertices.size();
		const uint64 isize = mesh.Indices.size();
		out.write(reinterpret_cast<const char*>(&vsize), sizeof(uint64));
		out.write(reinterpret_cast<const char*>(&isize), sizeof(uint64));
		out.write(reinterpret_cast<const char*>(mesh.Vertices.data()), vsize * sizeof(MeshVertex));
		out.write(reinterpret_cast<const char*>(mesh.Indices.data()), isize * sizeof(uint16));
	}

	if (!out.good())
		std::cout << "Write error while saving: " << filename << std::endl;
}
