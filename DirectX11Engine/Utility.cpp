#include "Utility.h"
#include "Camera.h"
#include "SimpleMesh.h"
#include <DirectXMath.h>

using namespace DirectX;

vector<SimpleMesh*> CullMeshesAgainstFrustum(SimpleMesh* meshes, unsigned int numMeshes, Camera* cam)
{
	vector<SimpleMesh*> unculledMeshes;

	XMMATRIX view = cam->GetView();
	XMMATRIX projection = cam->GetProjection();

	XMMATRIX viewProj = XMMatrixTranspose(XMMatrixMultiply(view, projection));

	XMVECTOR planes[6];

	//extract frustum planes
	planes[0] = viewProj.r[3] + viewProj.r[0];
	planes[1] = viewProj.r[3] - viewProj.r[0];

	planes[2] = viewProj.r[3] + viewProj.r[1];
	planes[3] = viewProj.r[3] - viewProj.r[1];

	planes[4] = viewProj.r[3] + viewProj.r[2];
	planes[5] = viewProj.r[3] - viewProj.r[2];

	for (unsigned int plane = 0; plane < 6; plane++)
	{
		planes[plane] = XMPlaneNormalize(planes[plane]);
	}

	//for each mesh in the list to render, use it's bounding box to determine if it is inside or outside the frustum
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		XMFLOAT4 min = meshes[i].GetBoxMin();
		XMFLOAT4 max = meshes[i].GetBoxMax();

		bool inSideFrustum = true;

		// if all eight points are outside one of the planes, then it can't be in the frustum
		for (unsigned int plane = 0; plane < 6; plane++)
		{
			int out = 0;
			XMFLOAT4 result;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(min.x, min.y, min.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(max.x, min.y, min.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(min.x, max.y, min.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(min.x, min.y, max.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(max.x, max.y, min.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(max.x, min.y, max.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(min.x, max.y, max.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			XMStoreFloat4(&result, XMPlaneDot(planes[plane], XMVectorSet(max.x, max.y, max.z, 1.0f)));
			out += result.x < 0.0f ? 1 : 0;

			// If fully outside one of the planes, then not in the frustum!
			if (out == 8)
			{
				inSideFrustum = false;
				break;
			}
		}

		if (inSideFrustum)
		{
			unculledMeshes.emplace_back(&meshes[i]);
		}
	}

	return unculledMeshes;
}