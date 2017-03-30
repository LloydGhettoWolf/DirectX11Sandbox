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
		bool isNormalMapped;
		data.read((char*)&info, sizeof(headerInfo));
		data.read((char*)&isNormalMapped, sizeof(bool));

		if (isNormalMapped)
		{
			(*meshes)[i].isNormalMapped = true;
			(*meshes)[i].mappedVertices = new PosNormalUvTan[info.numverts];
		}
		else
		{
			(*meshes)[i].isNormalMapped = false;
			(*meshes)[i].vertices = new PosNormalUv[info.numverts];
		}
		
		(*meshes)[i].indices = new unsigned int[info.numIndices];
		(*meshes)[i].numVerts = info.numverts;
		(*meshes)[i].numIndices = info.numIndices;
		(*meshes)[i].materialIndex = info.materialIndex;

		if (isNormalMapped)
		{
			data.read((char*)&(*meshes)[i].mappedVertices[0], sizeof(PosNormalUvTan) * info.numverts);
		}
		else
		{
			data.read((char*)&(*meshes)[i].vertices[0], sizeof(PosNormalUv) * info.numverts);
		}

		
		data.read((char*)&(*meshes)[i].indices[0], sizeof(unsigned int) * info.numIndices);

		data.read((char*)&(*meshes)[i].min, sizeof(XMFLOAT4));
		data.read((char*)&(*meshes)[i].max, sizeof(XMFLOAT4));

		(*meshes)[i].center.x = 0.5f * ((*meshes)[i].min.x + (*meshes)[i].max.x);
		(*meshes)[i].center.y = 0.5f * ((*meshes)[i].min.y + (*meshes)[i].max.y);
		(*meshes)[i].center.z = 0.5f * ((*meshes)[i].min.z + (*meshes)[i].max.z);
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