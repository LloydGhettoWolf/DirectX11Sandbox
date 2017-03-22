#pragma once
#include <vector>
using namespace std;

class SimpleMesh;
class Camera;

vector<SimpleMesh*> CullMeshesAgainstFrustum(SimpleMesh* meshes, unsigned int numMeshes, Camera* cam);