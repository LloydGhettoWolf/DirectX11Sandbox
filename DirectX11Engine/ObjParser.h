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
	VertexType* vertices;
	unsigned int* indices;
	unsigned int materialIndex;
	int numVerts;
	int numIndices;

	ProcessedMeshData() {};

	~ProcessedMeshData() 
	{
		
	}
};

MeshData* ReadObjFile(string& fileName);
void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMaterials, ProcessedMeshData** meshes, materialInfo** materials);