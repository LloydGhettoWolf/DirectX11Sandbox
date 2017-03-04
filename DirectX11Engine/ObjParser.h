//ObjParser.h
#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

struct VertexType
{
	XMFLOAT3 position;
	//float padding;
	XMFLOAT3 normal;
	//float padding2;
	XMFLOAT2 uvs;
	//float morePadding[2];
};

struct MeshData
{
	vector<VertexType> vertices;
	vector<unsigned int> indices;
};

struct ProcessedMeshData
{
	int numMeshes;
	VertexType** vertices;
	unsigned int** indices;
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
	}
};

MeshData* ReadObjFile(string& fileName);
ProcessedMeshData* ReadBoomFile(string& fileName);