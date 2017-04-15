#pragma once
#include <vector>
#include <DirectXMath.h>
using namespace std;
using namespace DirectX;

class SimpleMesh;
class Camera;
class Frustum;

vector<SimpleMesh*> CullMeshesAgainstFrustum(SimpleMesh* meshes, unsigned int numMeshes, Frustum* frustum);

bool CompareZAscend(SimpleMesh* first, SimpleMesh* second);
bool CompareZDescend(SimpleMesh* first, SimpleMesh* second);

void UpdateThisFrameZ(SimpleMesh* mesh, XMMATRIX* viewProj);