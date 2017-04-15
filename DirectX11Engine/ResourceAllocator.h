#pragma once

struct ID3D11Buffer;
struct ID3D11Device;

class ResourceAllocator
{
public:
	ResourceAllocator(ID3D11Device* device) : mDevice(device) {};
	~ResourceAllocator() {};

	ID3D11Buffer* AllocateVertexBuffer(void* data, unsigned int elemSize, unsigned int count);
	ID3D11Buffer* AllocateIndexBuffer(void* data, unsigned int elemSize, unsigned int count);
	ID3D11Buffer* AllocateVInstanceBuffer(void* data, unsigned int elemSize, unsigned int count);

private:
	ID3D11Device* mDevice;
};