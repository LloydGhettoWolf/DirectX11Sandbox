//ObjParser.h
#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>


using namespace std;
using namespace DirectX;

class Shader;
class ID3D11ShaderResourceView;

struct headerInfo
{
	int numverts;
	int numIndices;
	unsigned int materialIndex;
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
	unsigned int maskIndex = 0;
};

struct processedMaterialInfo
{
	XMFLOAT3 diffuse;
	XMFLOAT3 ambient;
	XMFLOAT3 specular;
	float specFactor;
	ID3D11ShaderResourceView* diffSrv = 0;
	ID3D11ShaderResourceView* specSrv = 0;
	ID3D11ShaderResourceView* normSrv = 0;
	ID3D11ShaderResourceView* maskSrv = 0;
	Shader* shader = 0;
};

struct ProcessedMeshData
{
	XMFLOAT3* vertices = nullptr;
	XMFLOAT3* normals = nullptr;
	XMFLOAT2* uvs = nullptr;
	XMFLOAT3* tangents = nullptr;
	unsigned int* indices = nullptr;
	unsigned int materialIndex;
	int numVerts;
	int numIndices;
	bool isNormalMapped;
	XMFLOAT3 min, max, center;

	ProcessedMeshData() {};

	~ProcessedMeshData() 
	{
		delete[] vertices;
		delete[] normals;
		delete[] uvs;

		if (tangents != nullptr)
		{
			delete[] tangents;
		}

		delete[] indices;
	}
};


void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMappedMeshes, unsigned int& numMaterials, unsigned int& numTextures,
					string** textureNames, ProcessedMeshData** meshes, materialInfo** materials);