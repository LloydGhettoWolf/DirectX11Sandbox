// ObjParserScratchPad.cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include "ObjParser.h"

using namespace std;

struct headerInfo
{
	int numverts;
	int numIndices;
};


inline void ProcessOneLenTokens(MeshData* data,
	stringstream& ss,
	char firstToken)
{
	VertexType vert;
	unsigned int index;
	switch (firstToken)
	{
	case 'v':
		ss >> vert.position.x;
		ss >> vert.position.y;
		ss >> vert.position.z;
		data->vertices.emplace_back(vert);
		break;
	case 'f':
		ss >> index;
		data->indices.emplace_back(index - 1);
		ss >> index;
		data->indices.emplace_back(index - 1);
		ss >> index;
		data->indices.emplace_back(index - 1);
		break;
	case '#':
		ss.ignore(INT_MAX, '\n');
		break;
	}
}


inline void CreateNormals(MeshData* data)
{
	// Go over each face
	// create a normal for that face
	// for each XMFLOAT3 create a list of face numbers
	// sum up the normals from each face for each XMFLOAT3
	// then normalize
	// hey vertices!

	
	unsigned int numFaces = data->indices.size() / 3;
	unsigned int numVerts = data->vertices.size();

	// A vector of vectors storing the faces associated with each XMFLOAT3
	vector<XMVECTOR> faceNormals;
	faceNormals.reserve(numVerts);

	for (unsigned int i = 0; i < numVerts; i++)
	{
		faceNormals.emplace_back( XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) );
	}

	for (unsigned int face = 0; face < numFaces; face++) 
	{
		unsigned int index = face * 3;
		unsigned int indexA = data->indices[index];
		unsigned int indexB = data->indices[index + 1];
		unsigned int indexC = data->indices[index + 2];
		XMVECTOR A = XMLoadFloat3(&data->vertices[indexA].position);
		XMVECTOR B = XMLoadFloat3(&data->vertices[indexB].position);
		XMVECTOR C = XMLoadFloat3(&data->vertices[indexC].position);

		XMVECTOR first = B - A;
		XMVECTOR second = C - A;

		XMFLOAT3 faceNormal;
		XMStoreFloat3(&faceNormal, XMVector3Cross(second, first));
		//faceNormals.emplace_back(XMFLOAT3(faceNormal));

		XMVECTOR faceNormalVec = XMLoadFloat3(&faceNormal);
		faceNormals[indexA] += faceNormalVec;
		faceNormals[indexB] += faceNormalVec;
		faceNormals[indexC] += faceNormalVec;
	}

	// Now normalize all normals


	for (unsigned int i = 0; i < numVerts; i++)
	{
		faceNormals[i] = XMVector3Normalize(faceNormals[i]);
		XMFLOAT3 destnorm;
		XMStoreFloat3(&destnorm, faceNormals[i]);
		data->vertices[i].normal = destnorm;
	}

}

MeshData* ReadObjFile(string& fileName)
{
	ifstream inFile;
	string line;

	MeshData* data = new MeshData();

	int numVerts = 0;
	int numFaces = 0;
	int numNorms = 0;

	inFile.open(fileName.c_str());

	stringstream ss;

	ss << inFile.rdbuf();

	inFile.close();


	while (ss.rdbuf()->in_avail() != 0)
	{
		string firstToken;
		ss >> firstToken;

		int len = firstToken.length();

		switch (len)
		{
		case 1:
			ProcessOneLenTokens(data, ss, firstToken[0]);
			break;
		}

	}

	cout << data->vertices.size() << " " << data->indices.size() << " " << endl;

	CreateNormals(data);

	inFile.close();

	return data;
}

ProcessedMeshData* ReadBoomFile(string& fileName)
{
	ifstream file;
	file.open(fileName.c_str(), ios::binary);
	int numMeshes = 0;
	file >> numMeshes;

	ProcessedMeshData* newMesh = new ProcessedMeshData();
	newMesh->numMeshes = numMeshes;

	newMesh->vertices = new VertexType*[numMeshes];
	newMesh->indices  = new unsigned int*[numMeshes];

	newMesh->numVerts = new int[numMeshes];
	newMesh->numIndices = new int[numMeshes];

	for (int i = 0; i < numMeshes; i++)
	{
		headerInfo info;
		file.read((char*)&info, sizeof(headerInfo));

		newMesh->vertices[i] = new VertexType[info.numverts];
		newMesh->indices[i] = new unsigned int[info.numIndices];
		newMesh->numVerts[i] = info.numverts;
		newMesh->numIndices[i] = info.numIndices;

		file.read((char*)&newMesh->vertices[i][0], sizeof(VertexType) * info.numverts);
		file.read((char*)&newMesh->indices[i][0], sizeof(unsigned int) * info.numIndices);
	}

	file.close();

	return newMesh;
}