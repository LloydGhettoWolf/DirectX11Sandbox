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
	XMFLOAT4 position;
	XMFLOAT4 normal;
	XMFLOAT4 uvs;
};

struct MeshData
{
	vector<VertexType> vertices;
	vector<unsigned int> indices;
};

struct materialInfo
{
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
	VertexType* vertices;
	unsigned int* indices;
	unsigned int materialIndex;
	int numVerts;
	int numIndices;
	XMFLOAT4 min, max;

	ProcessedMeshData() {};

	~ProcessedMeshData() 
	{
		
	}
};

MeshData* ReadObjFile(string& fileName);
void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMaterials, unsigned int& numTextures,
					string** textureNames, ProcessedMeshData** meshes, materialInfo** materials);