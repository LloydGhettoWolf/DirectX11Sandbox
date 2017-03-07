//ObjParser.h
#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Texture.h"

using namespace std;
using namespace DirectX;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uvs;
};

struct MeshData
{
	vector<VertexType> vertices;
	vector<unsigned int> indices;
};

struct materialInfo
{
	string materialName;
	XMFLOAT3 diffuse;
	XMFLOAT3 ambient;
	XMFLOAT3 specular;
	float specFactor;
	unsigned int diffTexIndex;
	unsigned int specTexIndex;
	unsigned int normMapIndex;
};

struct ProcessedMeshData
{
	int numMeshes;
	int numTextures;
	int numMaterials;
	VertexType** vertices;
	unsigned int** indices;
	unsigned int* materialIndices;
	materialInfo* materialTable;
	string* textureNames;
	int* numVerts;
	int* numIndices;

	ProcessedMeshData() {};

	~ProcessedMeshData() 
	{
		for (int i = 0; i < numMeshes; i++)
		{
			delete [] vertices[i];
			delete [] indices[i];
		}

		delete [] vertices;
		delete [] indices;
		delete [] numVerts;
		delete [] numIndices;
		delete [] textureNames;
	}
};

MeshData* ReadObjFile(string& fileName);
ProcessedMeshData* ReadBoomFile(string& filePath, string& fileName);