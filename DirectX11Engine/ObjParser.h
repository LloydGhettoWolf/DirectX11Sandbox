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
	XMFLOAT3 normal;
};

struct MeshData
{
	vector<VertexType> vertices;
	vector<unsigned int> indices;
};

struct ProcessedMeshData
{
	VertexType* vertices;
	unsigned int* indices;
	int numVerts;
	int numIndices;

	ProcessedMeshData() {};
	~ProcessedMeshData() { delete [] vertices; delete [] indices; }
};

MeshData* ReadObjFile(string& fileName);
ProcessedMeshData* ReadBoomFile(string& fileName);