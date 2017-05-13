//TextBox.h - class for screen aligned 2D rectangles for UI etc 
#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
class  ResourceAllocator;
class Texture;
struct Character;
struct ID3D11ShaderResourceView;

class TextBox
{
public:
	TextBox() : mVertices(nullptr), mIndices(nullptr), mUVs(nullptr), mAlphaVal(1.0f) {};
	~TextBox() { Shutdown(); }

	bool Init(float xPos, float yPos, float scale, char* text, Character* set, int fontHeight, int fontWidth,
		      ResourceAllocator* resourceAllocator, Texture* fontTexture = nullptr);
	void Shutdown();

	ID3D11Buffer** GetVertBuffer() { return mVertices; }
	ID3D11Buffer* GetIndexBuffer() { return mIndices; }

	Texture* GetTexture();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:
	ID3D11Buffer **mVertices, *mIndices, *mUVs;
	Texture* mTexture;

	//for transparency
	float mAlphaVal;

	int mNumVerts, mNumIndices, mFontHeight, mFontWidth;
	float mXPos, mYPos;
};
