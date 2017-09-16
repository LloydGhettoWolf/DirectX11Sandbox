//Frustum.cpp
#include <D3D11.h>
#include "Frustum.h"
#include "Camera.h"

bool Frustum::InitFrustum(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertData, indexData;
	HRESULT result;

	D3D11_BUFFER_DESC* vertBufferDesc = new D3D11_BUFFER_DESC;
	D3D11_BUFFER_DESC* indexBufferDesc = new D3D11_BUFFER_DESC;


	vertBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc->ByteWidth = sizeof(XMFLOAT4) * 8; //aabb has 8 points
	vertBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc->CPUAccessFlags = 0;
	vertBufferDesc->MiscFlags = 0;
	vertBufferDesc->StructureByteStride = 0;

	XMFLOAT4 aabbVerts[8] =
	{
		//near
		XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),
		XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),
		XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),

		//far
		XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),
		XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	// Give the subresource structure a pointer to the vertex data.
	vertData.pSysMem = &aabbVerts[0];
	vertData.SysMemPitch = 0;
	vertData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(vertBufferDesc, &vertData, &mVertBuffer);
	if (FAILED(result))
	{
		return false;
	}

	unsigned int indices[24] =
	{
		//top
		0,1, 1,2, 2,3, 3,0,
		//bottom
		4,5, 5,6, 6,7, 7,4,

		//sides
		0,4, 1,5, 2,6, 3,7
	};

	// Set up the description of the static index buffer.
	indexBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc->ByteWidth = sizeof(unsigned int) * 24;
	indexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc->CPUAccessFlags = 0;
	indexBufferDesc->MiscFlags = 0;
	indexBufferDesc->StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void Frustum::UpdateFrustum(Camera* cam)
{
	XMMATRIX view = XMLoadFloat4x4( &cam->GetView());
	XMMATRIX projection = XMLoadFloat4x4( &cam->GetProjection());

	XMMATRIX viewProj = view * projection;

	XMFLOAT4X4 frustumMatrix;
	XMStoreFloat4x4(&frustumMatrix, viewProj);

	// Calculate near plane of frustum.
	mPlanes[0][0] = frustumMatrix._14 + frustumMatrix._13;
	mPlanes[0][1] = frustumMatrix._24 + frustumMatrix._23;
	mPlanes[0][2] = frustumMatrix._34 + frustumMatrix._33;
	mPlanes[0][3] = frustumMatrix._44 + frustumMatrix._43;

	// Normalize the near plane.
	float length = sqrtf((mPlanes[0][0] * mPlanes[0][0]) + (mPlanes[0][1] * mPlanes[0][1]) + (mPlanes[0][2] * mPlanes[0][2]));
	mPlanes[0][0] /= length;
	mPlanes[0][1] /= length;
	mPlanes[0][2] /= length;
	mPlanes[0][3] /= length;

	// Calculate far plane of frustum.
	mPlanes[1][0] = frustumMatrix._14 - frustumMatrix._13;
	mPlanes[1][1] = frustumMatrix._24 - frustumMatrix._23;
	mPlanes[1][2] = frustumMatrix._34 - frustumMatrix._33;
	mPlanes[1][3] = frustumMatrix._44 - frustumMatrix._43;

	// Normalize the far plane.
	length = sqrtf((mPlanes[1][0] * mPlanes[1][0]) + (mPlanes[1][1] * mPlanes[1][1]) + (mPlanes[1][2] * mPlanes[1][2]));
	mPlanes[1][0] /= length;
	mPlanes[1][1] /= length;
	mPlanes[1][2] /= length;
	mPlanes[1][3] /= length;

	// Calculate left plane of frustum.
	mPlanes[2][0] = frustumMatrix._14 + frustumMatrix._11;
	mPlanes[2][1] = frustumMatrix._24 + frustumMatrix._21;
	mPlanes[2][2] = frustumMatrix._34 + frustumMatrix._31;
	mPlanes[2][3] = frustumMatrix._44 + frustumMatrix._41;

	// Normalize the left plane.
	length = sqrtf((mPlanes[2][0] * mPlanes[2][0]) + (mPlanes[2][1] * mPlanes[2][1]) + (mPlanes[2][2] * mPlanes[2][2]));
	mPlanes[2][0] /= length;
	mPlanes[2][1] /= length;
	mPlanes[2][2] /= length;
	mPlanes[2][3] /= length;

	// Calculate right plane of frustum.
	mPlanes[3][0] = frustumMatrix._14 - frustumMatrix._11;
	mPlanes[3][1] = frustumMatrix._24 - frustumMatrix._21;
	mPlanes[3][2] = frustumMatrix._34 - frustumMatrix._31;
	mPlanes[3][3] = frustumMatrix._44 - frustumMatrix._41;

	// Normalize the right plane.
	length = sqrtf((mPlanes[3][0] * mPlanes[3][0]) + (mPlanes[3][1] * mPlanes[3][1]) + (mPlanes[3][2] * mPlanes[3][2]));
	mPlanes[3][0] /= length;
	mPlanes[3][1] /= length;
	mPlanes[3][2] /= length;
	mPlanes[3][3] /= length;

	// Calculate top plane of frustum.
	mPlanes[4][0] = frustumMatrix._14 - frustumMatrix._12;
	mPlanes[4][1] = frustumMatrix._24 - frustumMatrix._22;
	mPlanes[4][2] = frustumMatrix._34 - frustumMatrix._32;
	mPlanes[4][3] = frustumMatrix._44 - frustumMatrix._42;

	// Normalize the top plane.
	length = sqrtf((mPlanes[4][0] * mPlanes[4][0]) + (mPlanes[4][1] * mPlanes[4][1]) + (mPlanes[4][2] * mPlanes[4][2]));
	mPlanes[4][0] /= length;
	mPlanes[4][1] /= length;
	mPlanes[4][2] /= length;
	mPlanes[4][3] /= length;

	// Calculate bottom plane of frustum.
	mPlanes[5][0] = frustumMatrix._14 + frustumMatrix._12;
	mPlanes[5][1] = frustumMatrix._24 + frustumMatrix._22;
	mPlanes[5][2] = frustumMatrix._34 + frustumMatrix._32;
	mPlanes[5][3] = frustumMatrix._44 + frustumMatrix._42;

	// Normalize the bottom plane.
	length = sqrtf((mPlanes[5][0] * mPlanes[5][0]) + (mPlanes[5][1] * mPlanes[5][1]) + (mPlanes[5][2] * mPlanes[5][2]));
	mPlanes[5][0] /= length;
	mPlanes[5][1] /= length;
	mPlanes[5][2] /= length;
	mPlanes[5][3] /= length;
}


void Frustum::UpdateFrustumFast(Camera* cam)
{
	XMMATRIX view = XMLoadFloat4x4(&cam->GetView());
	XMMATRIX projection = XMLoadFloat4x4(&cam->GetProjection());

	XMMATRIX viewProj = XMMatrixTranspose(view * projection);
	XMVECTOR currPlane, len;
	//near
	currPlane = viewProj.r[3] + viewProj.r[2];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[0], currPlane);
	//far
	currPlane = viewProj.r[3] - viewProj.r[2];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[1], currPlane);

    //top
	currPlane = viewProj.r[3] + viewProj.r[1];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[2], currPlane);
	//bottom
	currPlane = viewProj.r[3] - viewProj.r[1];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[3], currPlane);

	//left
	currPlane = viewProj.r[3] + viewProj.r[0];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[4], currPlane);
	//right
	currPlane = viewProj.r[3] - viewProj.r[0];
	len = XMVector3Length(currPlane);
	currPlane /= len;
	XMStoreFloat4(&mPlanesFast[5], currPlane);
}

bool Frustum::RenderFrustum(ID3D11DeviceContext* deviceContext)
{
	unsigned int offset;
	unsigned int stride;
	offset = 0;

	// Set vertex buffer stride and offset.
	stride = sizeof(XMFLOAT4);

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mVertBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//now we're set up, let's render!
	deviceContext->DrawIndexed(24, 0, 0);

	return true;
}

bool Frustum::CheckBox(XMFLOAT3& min, XMFLOAT3& max)
{
	// Check each of the six planes to see if the cube is inside the frustum.
	for (int plane = 0; plane < 6; plane++)
	{
		int out = 0;
		// Check all eight points of the cube to see if they all reside within the frustum.
		float dotProduct = (mPlanesFast[plane].x * min.x) + (mPlanesFast[plane].y * min.y) + (mPlanesFast[plane].z * min.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * max.x) + (mPlanesFast[plane].y * min.y) + (mPlanesFast[plane].z * min.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * min.x) + (mPlanesFast[plane].y * max.y) + (mPlanesFast[plane].z * min.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * min.x) + (mPlanesFast[plane].y * min.y) + (mPlanesFast[plane].z * max.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * max.x) + (mPlanesFast[plane].y * max.y) + (mPlanesFast[plane].z * min.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * min.x) + (mPlanesFast[plane].y * max.y) + (mPlanesFast[plane].z * max.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * max.x) + (mPlanesFast[plane].y * min.y) + (mPlanesFast[plane].z * max.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		dotProduct = (mPlanesFast[plane].x * max.x) + (mPlanesFast[plane].y * max.y) + (mPlanesFast[plane].z * max.z) + (mPlanesFast[plane].w * 1.0f);
		if (dotProduct < 0.0f)
		{
			out++;
		}

		if (out == 8)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckBoxFast(XMFLOAT3& min, XMFLOAT3& max)
{
	//construct corners

	
	// Check each of the six planes to see if the cube is inside the frustum.
	for (int plane = 0; plane < 6; plane++)
	{
		int out = 0;
		// Check all eight points of the cube to see if they all reside within the frustum.
		float minX = mPlanesFast[plane].x * min.x;
		float minY = mPlanesFast[plane].y * min.y;
		float minZ = mPlanesFast[plane].z * min.z;

		float W    = mPlanesFast[plane].w * 1.0f;

		float maxX = mPlanesFast[plane].x * max.x;
		float maxY = mPlanesFast[plane].y * max.y;
		float maxZ = mPlanesFast[plane].z * max.z;

		float dotProduct = minX + minY + minZ + W;
		dotProduct < 0.0f ? out++ : out;
		
		dotProduct = maxX + minY + minZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = minX + maxY + minZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = minX + minY + maxZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = maxX + maxY + minZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = maxX + minY + maxZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = minX + maxY + maxZ + W;
		dotProduct < 0.0f ? out++ : out;

		dotProduct = maxX + maxY + maxZ + W;
		dotProduct < 0.0f ? out++ : out;

		if (out == 8)
		{
			return false;
		}
	}

	return true;
}