//ObjParser.h
#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>


using namespace std;
using namespace DirectX;

struct headerInfo
{
	int numverts;
	int numIndices;
	unsigned int materialIndex;
};

struct PosNormalUv
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

struct PosNormalUvTan
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 uv;
};

struct materialInfo
{
	XMFLOAT3 diffuse;
	XMFLOAT3 ambient;
	XMFLOAT3 specular;
	float specFactor;
	unsigned int diffTexIndex = 0;
	unsigned int specTexIndex = 0;
	unsigned int normMapIndex = 0;
};

struct ProcessedMeshData
{
	PosNormalUv* vertices;
	PosNormalUvTan* mappedVertices;
	unsigned int* indices;
	unsigned int materialIndex;
	int numVerts;
	int numIndices;
	bool isNormalMapped;
	XMFLOAT3 min, max, center;

	ProcessedMeshData() {};

	~ProcessedMeshData() 
	{
		
	}
};


void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMappedMeshes, unsigned int& numMaterials, unsigned int& numTextures,
					string** textureNames, ProcessedMeshData** meshes, materialInfo** materials);