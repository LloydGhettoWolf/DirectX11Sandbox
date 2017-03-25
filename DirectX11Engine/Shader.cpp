//ColorShader.h
#include <D3D11.h>
#include <fstream>
#include <atlbase.h>
#include <D3DX11async.h>
#include <d3dcompiler.h>
#include "Shader.h"

bool Shader::InitShaderData(ID3D11Device* device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename, D3D11_INPUT_ELEMENT_DESC* inputs, int numElems)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DReadFileToBlob(vsFilename, &vertexShaderBuffer);

	if (result != S_OK)
	{
		return false;
	}

	result = D3DReadFileToBlob(psFilename, &pixelShaderBuffer);

	if (result != S_OK)
	{
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);

	if (result != S_OK)
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

	if (result != S_OK)
	{
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(inputs, numElems, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &mLayout);

	if (result != S_OK)
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void Shader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	// Release the layout.
	if (mLayout)
	{
		mLayout->Release();
	}

	// Release the pixel shader.
	if (mPixelShader)
	{
		mPixelShader->Release();
	}

	// Release the vertex shader.
	if (mVertexShader)
	{
		mVertexShader->Release();
	}

	return;
}

bool Shader::PrepareShader(ID3D11DeviceContext* deviceContext)
{
	// Now render the prepared buffers with the shader.
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);

	return true;
}


void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = (unsigned long)errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}