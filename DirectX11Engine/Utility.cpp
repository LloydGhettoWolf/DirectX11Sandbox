#include "Utility.h"
#include "Camera.h"
#include "SimpleMesh.h"
#include <DirectXMath.h>
#include "Frustum.h"

using namespace DirectX;

vector<SimpleMesh*> CullMeshesAgainstFrustum(SimpleMesh* meshes, unsigned int numMeshes, Frustum* frustum)
{
	vector<SimpleMesh*> unculledMeshes;

	//for each mesh in the list to render, use it's bounding box to determine if it is inside or outside the frustum
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		XMFLOAT3 min = meshes[i].GetBoxMin();
		XMFLOAT3 max = meshes[i].GetBoxMax();

		if (frustum->CheckBox(min, max))
		{
			unculledMeshes.emplace_back(&meshes[i]);
		}
	}

	return unculledMeshes;
}