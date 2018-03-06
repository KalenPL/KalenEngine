#ifndef MESH_H
#define MESH_H

#include "Utility.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;
	XMFLOAT4 Tangent;
};


struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<WORD> Indices; //Maksymalnie 2^16 = 65536 indeksów !!!
};


struct VertexColor
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct MeshDataColor
{
	std::vector<VertexColor> Vertices;
	std::vector<WORD> Indices;
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	void CreateBox(float size, XMFLOAT4 color, MeshData& Box);
	void CreateBox(float x, float y, float z, XMFLOAT4 color, MeshData& meshData);
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void CreateGridWorld(int size, XMFLOAT4 color, MeshData& Grid); 
	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);
	void CreateFullscreenQuad(MeshData& meshData);

	void CreateBoundingBoxView(XMFLOAT3 vectorMax, XMFLOAT3 vectorMin, MeshData& meshData);
	void CreateCollision(std::vector<XMFLOAT3> verticesPositionVector, std::vector<XMFLOAT3> verticesNormalVector, std::vector<XMUINT2> faceDataVector, MeshData& meshData);



	void CreateAxisArrows(UINT multiplySize, MeshData &meshData);
	UINT GetCountAxisRotate();


	void CreateAxisArrowX(UINT multiplySize, MeshData &meshData);
	void CreateAxisArrowY(UINT multiplySize, MeshData &meshData);
	void CreateAxisArrowZ(UINT multiplySize, MeshData &meshData);

	void CreateAxisCircleX(UINT multiplySize, MeshData &meshData);
	void CreateAxisCircleY(UINT multiplySize, MeshData &meshData);
	void CreateAxisCircleZ(UINT multiplySize, MeshData &meshData);


	void CreateAxisBox(float boxSize, MeshData &AxisBox, XMFLOAT3 colour, XMFLOAT3 offset);


	//void CreateBoxNEW(float x, float y, float z, XMFLOAT4 color, MeshData& Box);
	void CreateBoxNEW(float x, float y, float z, XMFLOAT4 color, MeshData& Box);

	void LoadMeshFromFile(std::string fileName, MeshData &LoadMesh);
	bool LoadMeshFromFileOBJ(std::string fileName, MeshData &LoadMesh, XMFLOAT3 &center, XMFLOAT3 &extents, XMVECTOR &maxVec, XMVECTOR &minVec);


private:
	float CountFunctionValue(float x, float z); //For Y value

	void LoadMeshFromFileOBJCount(std::string fileName, int& v_count, int& vn_count, int& vt_count, int& index_count);



	UINT mCountAxisRotate;

};
#endif; //MESH_H