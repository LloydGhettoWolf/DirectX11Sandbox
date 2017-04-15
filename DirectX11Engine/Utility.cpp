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

bool CompareZAscend(SimpleMesh* first, SimpleMesh* second)
{
	return first->thisFrameZ < second->thisFrameZ;
}

bool CompareZDescend(SimpleMesh* first, SimpleMesh* second)
{
	return first->thisFrameZ > second->thisFrameZ;
}

void UpdateThisFrameZ(SimpleMesh* mesh, XMMATRIX* viewProj) 
{
	XMVECTOR center = XMLoadFloat3(&mesh->GetBoxCenter());

	center = XMVector3Transform(center, *viewProj);

	XMFLOAT3 newVec;
	XMStoreFloat3(&newVec, center);

	mesh->thisFrameZ = newVec.z;
}