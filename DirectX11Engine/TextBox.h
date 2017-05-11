//TextBox.h - class for screen aligned 2D rectangles for UI etc 
#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
class  ResourceAllocator;
class Texture;
struct Character;
struct BufferInfo;
struct ID3D11ShaderResourceView;

class TextBox
{
public:
	TextBox() : mVertices(nullptr), mIndices(nullptr), mUVs(nullptr), mAlphaVal(1.0f) {};
	~TextBox() { Shutdown(); }

	bool Init(float xPos, float yPos, float scale, char* text, WCHAR* texturePath, Character* set, int fontHeight, int fontWidth,
		      ResourceAllocator* resourceAllocator, ID3D11Device* device);
	void Shutdown();

	ID3D11Buffer** GetVertBuffer() { return mVertices; }
	ID3D11Buffer* GetIndexBuffer() { return mIndices; }

	ID3D11ShaderResourceView* GetTexture();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:
	bool InitBuffers(BufferInfo* infos, float scale, int fontHeight, int fontWidth, ResourceAllocator* resourceAllocator);
	ID3D11Buffer **mVertices, *mIndices, *mUVs;
	Texture* mTexture;

	//for transparency
	float mAlphaVal;

	int mNumVerts, mNumIndices;
	float mXPos, mYPos;
};
