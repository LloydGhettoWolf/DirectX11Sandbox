//ScreenRectangle.h - class for screen aligned 2D rectangles for UI etc 
#pragma once

struct ID3D11Buffer;
struct ID3D11DeviceContext;
class  ResourceAllocator;

class ScreenRectangle
{
public:
	ScreenRectangle() : mVertices(nullptr), mIndices(nullptr), mAlphaVal(1.0f) {};
	~ScreenRectangle() { Shutdown(); }

	bool Init(float width, float height, float xPos, float yPos, ResourceAllocator* resourceAllocator);
	void Shutdown();

	ID3D11Buffer** GetVertBuffer() { return &mVertices; }
	ID3D11Buffer* GetIndexBuffer() { return mIndices; }

private:
	bool InitBuffers(ResourceAllocator* resourceAllocator);
	ID3D11Buffer *mVertices, *mIndices;

	//for transparency
	float mAlphaVal;

	float mRectWidth, mRectHeight;
	float mXPos, mYPos;
};
