#pragma once
#include <vector>
using namespace std;

class SimpleMesh;
class Camera;
class Frustum;

vector<SimpleMesh*> CullMeshesAgainstFrustum(SimpleMesh* meshes, unsigned int numMeshes, Frustum* frustum);