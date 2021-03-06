//D3DClass.cpp
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMesh.h"
#include "Camera.h"
#include "D3DClass.h"
#include "Shader.h"

DXGI_FORMAT BUFFER_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;

D3DClass::D3DClass() :  mSwapChain(0),
						mDevice (0),
						mDeviceContext (0),
						mDepthStencilBuffer (0),
						mDepthStencilState (0),
						mDepthStencilView (0),
						mRasterState (0),
						mNumRenderTargets(0)
{

}

bool D3DClass::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
					float screenDepth, float screenNear, int numSamples, int numRenderTargets)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Store the vsync setting.
	mVSyncEnabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
		return false;

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
		return false;

	// Get the number of modes that fit the bufferFormat display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(BUFFER_FORMAT, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
		return false;

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(BUFFER_FORMAT, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
		return false;

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
		return false;

	// Store the dedicated video card memory in megabytes.
	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Release the display mode list.
	delete[] displayModeList;

	// release interfaces
	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 2;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = BUFFER_FORMAT;

	// Set the refresh rate of the back buffer.
	if (mVSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = numSamples;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mDeviceContext);

	if (FAILED(result))
		return false;

	// Get the pointer to the back buffer.
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
		return false;

	mNumRenderTargets = numRenderTargets;
	mRenderTargetView = new ID3D11RenderTargetView*[mNumRenderTargets];
	mRenderSRV = new ID3D11ShaderResourceView*[mNumRenderTargets - 1];

	// Create the render target view with the back buffer pointer.
	//create render targets / buffers
	
	result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView[mNumRenderTargets-1]);
	if (FAILED(result))
		return false;

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = numSamples;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(result))
		return false;

	//Create a 2d texture for a second render target view
	depthBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	depthBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (unsigned int i = 0; i < mNumRenderTargets-1; i++)
	{
		ID3D11Texture2D* renderBuffer;
		result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &renderBuffer);
		if (FAILED(result))
			return false;

		// Create the render target view with the back buffer pointer.
		result = mDevice->CreateRenderTargetView(renderBuffer, NULL, &mRenderTargetView[i]);
		if (FAILED(result))
			return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC render_srv;
		render_srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		render_srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		render_srv.Texture2D.MostDetailedMip = 0;
		render_srv.Texture2D.MipLevels = 1;

		result = mDevice->CreateShaderResourceView(renderBuffer, &render_srv, &mRenderSRV[i]);
		if (FAILED(result))
			return false;

		renderBuffer->Release();
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	// Create the depth stencil state.
	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result))
		return false;

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	// Create the depth stencil state.
	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilStateAlpha);
	if (FAILED(result))
		return false;


	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC depth_srv;
	depth_srv.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depth_srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	depth_srv.Texture2D.MostDetailedMip = 0;
	depth_srv.Texture2D.MipLevels = 1;

	result = mDevice->CreateShaderResourceView(mDepthStencilBuffer, &depth_srv, &mDepthSRV);
	if (FAILED(result))
		return false;

	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(result))
		return false;

	// Now set the rasterizer state.
	mDeviceContext->RSSetState(mRasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	mDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float) XM_PI/ 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;
	
	return true;
}

void D3DClass::SetRenderTarget(int index, bool depth)
{
	assert(index < mNumRenderTargets && "index out of range for render target");

	if(depth)
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView[index], mDepthStencilView);
	else
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView[index], NULL);

	currentView = mRenderTargetView[index];
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}

	if (mRasterState)
	{
		mRasterState->Release();
		mRasterState = 0;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}

	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}

	if (mRenderTargetView)
	{
		mRenderTargetView[0]->Release();
		mRenderTargetView[0] = 0;
		//mRenderTargetView[1]->Release();
		//mRenderTargetView[1] = 0;
	}

	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = 0;
	}

	if (mDevice)
	{
		mDevice->Release();
		mDevice = 0;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}	 
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha, bool depth)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	mDeviceContext->ClearRenderTargetView(currentView, color);

	// Clear the depth buffer->
	if(depth)
		mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete->
	if (mVSyncEnabled)
		// Lock to screen refresh rate->
		mSwapChain->Present(1, 0);
	else
		// Present as fast as possible->
		mSwapChain->Present(0, 0);

	return;
}

void D3DClass::SwitchOffDepthWrites()
{
	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilStateAlpha, 1);
}

void D3DClass::SwitchOnDepthWrites()
{
	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
}

void D3DClass::RenderToFullScreenTriangle(Shader* shader, void* data)
{
	mDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	mDeviceContext->IASetIndexBuffer(NULL, (DXGI_FORMAT)0, NULL);
	mDeviceContext->IASetInputLayout(NULL);

	shader->PrepareShader(mDeviceContext);
	shader->SetPerMeshParameters(data, mDeviceContext);

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(3, 0);
}