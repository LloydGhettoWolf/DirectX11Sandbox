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
	unsigned int materialIndex;
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


//inline void CreateNormals(MeshData* data)
//{
//	// Go over each face
//	// create a normal for that face
//	// for each XMFLOAT3 create a list of face numbers
//	// sum up the normals from each face for each XMFLOAT3
//	// then normalize
//	// hey vertices!
//
//	
//	unsigned int numFaces = data->indices.size() / 3;
//	unsigned int numVerts = data->vertices.size();
//
//	// A vector of vectors storing the faces associated with each XMFLOAT3
//	vector<XMVECTOR> faceNormals;
//	faceNormals.reserve(numVerts);
//
//	for (unsigned int i = 0; i < numVerts; i++)
//	{
//		faceNormals.emplace_back( XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) );
//	}
//
//	for (unsigned int face = 0; face < numFaces; face++) 
//	{
//		unsigned int index = face * 3;
//		unsigned int indexA = data->indices[index];
//		unsigned int indexB = data->indices[index + 1];
//		unsigned int indexC = data->indices[index + 2];
//		XMVECTOR A = XMLoadFloat3(&data->vertices[indexA].position);
//		XMVECTOR B = XMLoadFloat3(&data->vertices[indexB].position);
//		XMVECTOR C = XMLoadFloat3(&data->vertices[indexC].position);
//
//		XMVECTOR first = B - A;
//		XMVECTOR second = C - A;
//
//		XMFLOAT3 faceNormal;
//		XMStoreFloat3(&faceNormal, XMVector3Cross(second, first));
//		//faceNormals.emplace_back(XMFLOAT3(faceNormal));
//
//		XMVECTOR faceNormalVec = XMLoadFloat3(&faceNormal);
//		faceNormals[indexA] += faceNormalVec;
//		faceNormals[indexB] += faceNormalVec;
//		faceNormals[indexC] += faceNormalVec;
//	}
//
//	// Now normalize all normals
//
//
//	for (unsigned int i = 0; i < numVerts; i++)
//	{
//		faceNormals[i] = XMVector3Normalize(faceNormals[i]);
//		XMFLOAT3 destnorm;
//		XMStoreFloat3(&destnorm, faceNormals[i]);
//		data->vertices[i].normal = destnorm;
//	}
//
//}

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

	//CreateNormals(data);

	inFile.close();

	return data;
}

//returns an array of processed mesh data
void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMaterials, unsigned int& numTextures, 
				  string** textureNames, ProcessedMeshData** meshes, materialInfo** materials)
{
	string fullPath = filePath + fileName;
	ifstream file(fullPath.c_str(), ios::binary);

	stringstream data;
	data << file.rdbuf();
	file.close();

	
	data.read((char*)&numMeshes, sizeof(unsigned int));
	data.read((char*)&numMaterials, sizeof(unsigned int));

	*meshes = new ProcessedMeshData[numMeshes];

	
	// Copy across information about submeshes
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		headerInfo info;
		data.read((char*)&info, sizeof(headerInfo));

		(*meshes)[i].vertices = new VertexType[info.numverts];
		(*meshes)[i].indices = new unsigned int[info.numIndices];
		(*meshes)[i].numVerts = info.numverts;
		(*meshes)[i].numIndices = info.numIndices;
		(*meshes)[i].materialIndex = info.materialIndex;

		data.read((char*)&(*meshes)[i].vertices[0], sizeof(VertexType) * info.numverts);
		data.read((char*)&(*meshes)[i].indices[0], sizeof(unsigned int) * info.numIndices);
	}
	
	*materials = new materialInfo[numMaterials];
	

	// Read in material information
	for (unsigned int i = 0; i < numMaterials; i++)
	{
		data.read((char*)&(*materials)[i], sizeof(materialInfo));
	}

	//now read in textures for this mesh
	
	data.read((char*)&numTextures, sizeof(unsigned int));
	*textureNames = new string[numTextures];

	for (unsigned int i = 0; i < numTextures; i++)
	{
		string texName;
		unsigned int len;
		data.read((char*)&len, sizeof(unsigned int));
		char* temp = new char[len + 1];
		temp[len] = '\0';
		data.read(temp, len);
		string s(temp);
		(*textureNames)[i] = filePath + s;
		delete[] temp;
	}
}