#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;

class Renderer
{
	public:
		Renderer(ID3D11Device* device, ID3D11DeviceContext* context);
		~Renderer() {};

		void DrawIndexed(ID3D11Buffer* const* vertexInfoStreams, unsigned int numStreams, const unsigned int* strides, ID3D11Buffer* indices, unsigned int numIndices, D3D11_PRIMITIVE_TOPOLOGY primitiveType);

	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		ID3D11RenderTargetView*		mRenderTarget;
};