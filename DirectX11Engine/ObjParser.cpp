// ObjParserScratchPad.cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include "ObjParser.h"
#include "Texture.h"

using namespace std;

//returns an array of processed mesh data
void ReadBoomFile(string& filePath, string& fileName, unsigned int& numMeshes, unsigned int& numMappedMeshes, unsigned int& numMaterials, unsigned int& numTextures,
				  string** textureNames, ProcessedMeshData** meshes, materialInfo** materials)
{
	string fullPath = filePath + fileName;
	ifstream file(fullPath.c_str(), ios::binary);

	
	file.read((char*)&numMeshes, sizeof(unsigned int));
	file.read((char*)&numMaterials, sizeof(unsigned int));

	numMeshes -= 1;

	*meshes = new ProcessedMeshData[numMeshes];
	
	// Copy across information about submeshes
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		headerInfo info;
		bool isNormalMapped;
		file.read((char*)&info, sizeof(headerInfo));
		file.read((char*)&isNormalMapped, sizeof(bool));

		(*meshes)[i].isNormalMapped = isNormalMapped;
		(*meshes)[i].vertices = new XMFLOAT3[info.numverts];
		(*meshes)[i].normals = new XMFLOAT3[info.numverts];
		(*meshes)[i].uvs = new XMFLOAT2[info.numverts];


		if (isNormalMapped)
		{
			(*meshes)[i].tangents = new XMFLOAT3[info.numverts];
		}
		
		(*meshes)[i].indices = new unsigned int[info.numIndices];
		(*meshes)[i].numVerts = info.numverts;
		(*meshes)[i].numIndices = info.numIndices;
		(*meshes)[i].materialIndex = info.materialIndex;

		file.read((char*)&(*meshes)[i].vertices[0], sizeof(XMFLOAT3) * info.numverts);
		file.read((char*)&(*meshes)[i].normals[0], sizeof(XMFLOAT3) * info.numverts);
		file.read((char*)&(*meshes)[i].uvs[0], sizeof(XMFLOAT2) * info.numverts);

		if (isNormalMapped)
		{
			file.read((char*)&(*meshes)[i].tangents[0], sizeof(XMFLOAT3) * info.numverts);
		}

		
		file.read((char*)&(*meshes)[i].indices[0], sizeof(unsigned int) * info.numIndices);

		file.read((char*)&(*meshes)[i].min, sizeof(XMFLOAT3));
		file.read((char*)&(*meshes)[i].max, sizeof(XMFLOAT3));
		file.read((char*)&(*meshes)[i].center, sizeof(XMFLOAT3));
	}


	*materials = new materialInfo[numMaterials];
	

	// Read in material information
	for (unsigned int i = 0; i < numMaterials; i++)
	{
		file.read((char*)&(*materials)[i], sizeof(materialInfo));
	}

	//now read in textures for this mesh
	
	file.read((char*)&numTextures, sizeof(unsigned int));
	*textureNames = new string[numTextures];

	for (unsigned int i = 0; i < numTextures; i++)
	{
		string texName;
		unsigned int len;
		file.read((char*)&len, sizeof(unsigned int));
		char* temp = new char[len + 1];
		temp[len] = '\0';
		file.read(temp, len);
		string s(temp);
		(*textureNames)[i] = filePath + s;
		delete[] temp;
	}
}