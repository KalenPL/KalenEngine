#include "../Header Files/Mesh.h"


//******************************************************************************************************************************
//Constructor
//******************************************************************************************************************************
Mesh::Mesh(){}

//******************************************************************************************************************************
//Destructor
//******************************************************************************************************************************
Mesh::~Mesh(){}



//******************************************************************************************************************************
//CreateBoundingBoxView - to display boundingBox
//******************************************************************************************************************************
void Mesh::CreateBoundingBoxView(XMFLOAT3 vectorMax, XMFLOAT3 vectorMin, MeshData& meshData)
{
	Vertex vertex;

	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);

	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	meshData.Vertices.push_back(vertex);
	//Floor
	//point 1
	/*vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);


	meshData.Indices.push_back(0);
	meshData.Indices.push_back(1);
	meshData.Indices.push_back(2);

	meshData.Indices.push_back(0);
	meshData.Indices.push_back(2);
	meshData.Indices.push_back(3);




	//Top
	//point 1
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);


	meshData.Indices.push_back(4);
	meshData.Indices.push_back(5);
	meshData.Indices.push_back(6);

	meshData.Indices.push_back(4);
	meshData.Indices.push_back(6);
	meshData.Indices.push_back(7);




	//Wall A
	//point 1
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);



	meshData.Indices.push_back(8);
	meshData.Indices.push_back(9);
	meshData.Indices.push_back(10);

	meshData.Indices.push_back(8);
	meshData.Indices.push_back(10);
	meshData.Indices.push_back(11);



	//Wall B
	//point 1
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	meshData.Indices.push_back(12); 
	meshData.Indices.push_back(13); //1
	meshData.Indices.push_back(14); //2

	meshData.Indices.push_back(13); //
	meshData.Indices.push_back(15);
	meshData.Indices.push_back(14);


	//Wall C
	//point 1
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	meshData.Indices.push_back(16);
	meshData.Indices.push_back(17); //1
	meshData.Indices.push_back(18); //2

	meshData.Indices.push_back(17); //
	meshData.Indices.push_back(19);
	meshData.Indices.push_back(18);


	//Wall D
	//point 1
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 2 
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	vertex.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 4
	vertex.Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	vertex.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);


	meshData.Indices.push_back(20);
	meshData.Indices.push_back(21);
	meshData.Indices.push_back(22);

	meshData.Indices.push_back(21); //
	meshData.Indices.push_back(23);
	meshData.Indices.push_back(22);
	*/

}
//******************************************************************************************************************************
//To load and create collision mesh data
//******************************************************************************************************************************
void Mesh::CreateCollision(std::vector<XMFLOAT3> verticesPositionVector, std::vector<XMFLOAT3> verticesNormalVector, std::vector<XMUINT2> faceDataVector, MeshData& meshData)
{
	Vertex vertex;

	vertex.Tangent = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);

	UINT vertexPos = 0;
	UINT normalPos = 0;
	
	for (int i = 0; i < (int)faceDataVector.size(); ++i)
	{
		vertexPos = faceDataVector[i].x - 1;
		normalPos = faceDataVector[i].y - 1;
		vertex.Pos = verticesPositionVector[vertexPos];
		vertex.Normal = verticesPositionVector[normalPos];

		meshData.Vertices.push_back(vertex);
		meshData.Indices.push_back(i);
	}
}
//******************************************************************************************************************************
//CreateFullscreenQuad - to display shadow map
//******************************************************************************************************************************
void Mesh::CreateFullscreenQuad(MeshData& meshData)
{
//	meshData.Vertices.resize(4);
//	meshData.Indices.resize(6);

	Vertex vertex;

	//point 1
	vertex.Pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 1.0f);
	meshData.Vertices.push_back(vertex);


	//point 2
	vertex.Pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(vertex);

	//point 3
	vertex.Pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(1.0f, 0.0f);
	meshData.Vertices.push_back(vertex);


	//point 4
	vertex.Pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.TexC = XMFLOAT2(1.0f, 1.0f);
	meshData.Vertices.push_back(vertex);


	//Indices
	meshData.Indices.push_back(0);
	meshData.Indices.push_back(1);
	meshData.Indices.push_back(2);

	meshData.Indices.push_back(0);
	meshData.Indices.push_back(2);
	meshData.Indices.push_back(3);



}


//******************************************************************************************************************************
//CreateBox CUBE
//******************************************************************************************************************************
void Mesh::CreateBox(float size, XMFLOAT4 color, MeshData& Box)
{
	Vertex point;
	//point.Color = color; //Kolor dla wszystkich

	point.Pos = XMFLOAT3(-size, size, -size);    Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(size, size, -size);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(size, size, size);      Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-size, size, size);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-size, -size, -size);   Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(size, -size, -size);    Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(size, -size, size);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-size, -size, size);    Box.Vertices.push_back(point);

	Box.Indices.push_back(3); Box.Indices.push_back(1); Box.Indices.push_back(0);
	Box.Indices.push_back(2); Box.Indices.push_back(1); Box.Indices.push_back(3);

	Box.Indices.push_back(0); Box.Indices.push_back(5); Box.Indices.push_back(4);
	Box.Indices.push_back(1); Box.Indices.push_back(5); Box.Indices.push_back(0);

	Box.Indices.push_back(3); Box.Indices.push_back(4); Box.Indices.push_back(7);
	Box.Indices.push_back(0); Box.Indices.push_back(4); Box.Indices.push_back(3);

	Box.Indices.push_back(1); Box.Indices.push_back(6); Box.Indices.push_back(5);
	Box.Indices.push_back(2); Box.Indices.push_back(6); Box.Indices.push_back(1);


	Box.Indices.push_back(2); Box.Indices.push_back(7); Box.Indices.push_back(6);
	Box.Indices.push_back(3); Box.Indices.push_back(7); Box.Indices.push_back(2);

	Box.Indices.push_back(6); Box.Indices.push_back(4); Box.Indices.push_back(5);
	Box.Indices.push_back(7); Box.Indices.push_back(4); Box.Indices.push_back(6);
}
//******************************************************************************************************************************
//CreateBox BOX(X,Y,Z)
//******************************************************************************************************************************
void Mesh::CreateBox(float x, float y, float z, XMFLOAT4 color, MeshData& Box)
{

	Vertex point;
	//point.Color = color; //Kolor dla wszystkich

	point.Pos = XMFLOAT3(-x, y, -z);    Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, -z);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, z);      Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, z);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, -y, -z);   Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, -z);    Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, z);     Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, -y, z);    Box.Vertices.push_back(point);

	Box.Indices.push_back(3); Box.Indices.push_back(1); Box.Indices.push_back(0);
	Box.Indices.push_back(2); Box.Indices.push_back(1); Box.Indices.push_back(3);

	Box.Indices.push_back(0); Box.Indices.push_back(5); Box.Indices.push_back(4);
	Box.Indices.push_back(1); Box.Indices.push_back(5); Box.Indices.push_back(0);

	Box.Indices.push_back(3); Box.Indices.push_back(4); Box.Indices.push_back(7);
	Box.Indices.push_back(0); Box.Indices.push_back(4); Box.Indices.push_back(3);

	Box.Indices.push_back(1); Box.Indices.push_back(6); Box.Indices.push_back(5);
	Box.Indices.push_back(2); Box.Indices.push_back(6); Box.Indices.push_back(1);


	Box.Indices.push_back(2); Box.Indices.push_back(7); Box.Indices.push_back(6);
	Box.Indices.push_back(3); Box.Indices.push_back(7); Box.Indices.push_back(2);

	Box.Indices.push_back(6); Box.Indices.push_back(4); Box.Indices.push_back(5);
	Box.Indices.push_back(7); Box.Indices.push_back(4); Box.Indices.push_back(6);
}
//******************************************************************************************************************************
// CreateSphere
//******************************************************************************************************************************
void Mesh::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	Vertex topSphereVertex;
	topSphereVertex.Pos = XMFLOAT3(0.0f, +radius, 0.0f);
	topSphereVertex.Normal = XMFLOAT3(0.0f, +1.0f, 0.0f); 
	topSphereVertex.TexC = XMFLOAT2(0.0f, 0.0f);
	meshData.Vertices.push_back(topSphereVertex);

	//meshData.Vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;
			// spherical to cartesian
			v.Pos.x = radius*sinf(phi)*cosf(theta);
			v.Pos.y = radius*cosf(phi);
			v.Pos.z = radius*sinf(phi)*sinf(theta);

			XMVECTOR p = XMLoadFloat3(&v.Pos);
		    XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back(v);
		}
	}

	Vertex bottomSphereVertex;
	bottomSphereVertex.Pos = XMFLOAT3(0.0f, -radius, 0.0f); 
	topSphereVertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	topSphereVertex.TexC = XMFLOAT2(0.0f, 1.0f);

	meshData.Vertices.push_back(bottomSphereVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//



	//meshData.Indices.push_back(2);
	//meshData.Indices.push_back(3);
	//Repair first steps
	/*for (UINT i = 1; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(i);
		meshData.Indices.push_back(i + 1);
	}
	*/

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i);
		meshData.Indices.push_back(i + 1);

	}


	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}



	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}



}
//******************************************************************************************************************************
// CreateGridWorld as white wires
//******************************************************************************************************************************
void Mesh::CreateGridWorld(int size, XMFLOAT4 color, MeshData& Grid)
{	//Kolor - jest definiowany za pomoca zmiennej "Normal". W hlsl (fx) jego wartosc jest konwertowana z postaci
	//normal do color --> np. dla Normal XMFLOAT3(1.0f, 0.0f, 0.0f) ==> Color { 1.0f, 0.0f, 0.0f, 1.0f} ostatnia wartosc 
	//jest dodawana recznie w shaderze
	// Funkcja cieniujaca, które odpowiada za kolor -- 'PS_WIRE' w 'ShandardShader.fx'

	Vertex point;

	//Color as Normal
	point.Normal.x = color.x; point.Normal.y = color.y;  point.Normal.z = color.z;

	for (int i = -size; i < size + 1; i++)
	{
		/*if (i == 0)
		{
			point.Normal.x = 1.0f; point.Normal.y = 0.0f;  point.Normal.z = 0.0f;
			point.Pos = XMFLOAT3(size * 10, 0.0f, i * 10);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(0, 0.0f, i * 10);
			Grid.Vertices.push_back(point);

			point.Normal.x = color.x; point.Normal.y = color.y;  point.Normal.z = color.z;
			point.Pos = XMFLOAT3(0, 0.0f, i * 10);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(-size * 10, 0.0f, i * 10);
			Grid.Vertices.push_back(point);
		}
		else
		{*/
			point.Normal.x = color.x; point.Normal.y = color.y;  point.Normal.z = color.z;
			point.Pos = XMFLOAT3(size * 10, 0.0f, i * 10);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(-size * 10, 0.0f, i * 10);
			Grid.Vertices.push_back(point);
		//}

	}

	for (int i = -size; i < size + 1; i++)
	{
		/*if (i == 0)
		{
			//point.Normal.x = 0.0f; point.Normal.y = 1.0f;  point.Normal.z = 0.0f;
			point.Normal.x = 0.0f; point.Normal.y = 1.0f;  point.Normal.z = 0.0f;
			point.Pos = XMFLOAT3(i * 10, 0.0f, size * 10);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(i * 10, 0.0f, 0);
			Grid.Vertices.push_back(point);

			point.Normal.x = color.x; point.Normal.y = color.y;  point.Normal.z = color.z;
			point.Pos = XMFLOAT3(i * 10, 0.0f, 0);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(i * 10, 0.0f, -size * 10);
			Grid.Vertices.push_back(point);
		}
		else
		{*/
			point.Normal.x = color.x; point.Normal.y = color.y;  point.Normal.z = color.z;
			point.Pos = XMFLOAT3(i * 10, 0.0f, size * 10);
			Grid.Vertices.push_back(point);
			point.Pos = XMFLOAT3(i * 10, 0.0f, -size * 10);
			Grid.Vertices.push_back(point);
		//}
	}

	/*point.Normal.x = 0.0f; point.Normal.y = 0.0f;  point.Normal.z = 1.0f;
	point.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Grid.Vertices.push_back(point);
	point.Pos = XMFLOAT3(0.0f, size * 10, 0.0f);
	Grid.Vertices.push_back(point);*/
}



//******************************************************************************************************************************
// CreateAxisCircle X
//******************************************************************************************************************************
void Mesh::CreateAxisCircleX(UINT multiplySize, MeshData &meshData)
{
	Vertex point;
	float baseSize = 3.0f;
	XMVECTOR normalVector;
	//Set position of vector
	/*float _y1 = cos(0 * XM_PI / 45) * (baseSize * multiplySize);
	float _z1 = sin(0 * XM_PI / 45) * (baseSize * multiplySize);
	point.Pos = XMFLOAT3(0.0f, _y1, _z1);

	//Normalize vector
	XMVECTOR normalVector = XMLoadFloat3(&point.Pos);
	XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));

	//Add vector to mesh
	meshData.Vertices.push_back(point);
	*/
	float _y = 0.0f, _z = 0.0f;
	int iter = 90;
	for (int i = 0; i < iter; ++i)
	{
		//angle = (360 / iter)
		_y = cos(i * XM_PI / (360 / iter)) * (baseSize * multiplySize); 
		_z = sin(i * XM_PI / (360 / iter)) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(0.0f, _y, _z);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		meshData.Vertices.push_back(point);
	}


	/*
	for (int i = 1; i <= 71; i++) //create 70 points x2
	{
		//First point of the line
		_y = cos(i * XM_PI / 36) * (baseSize * multiplySize); //angle = 10
		_z = sin(i * XM_PI / 36) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(0.0f, _y, _z);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		meshData.Vertices.push_back(point);

		//Second point of the line
		_y = cos(i * XM_PI / 36) * (baseSize * multiplySize);
		_z = sin(i * XM_PI / 36) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(0.0f, _y, _z);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		meshData.Vertices.push_back(point); meshData.Vertices.push_back(point);
	}
	
	_y1 = cos(72 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(72 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(0.0f, _y1, _z1);
	meshData.Vertices.push_back(point);


	_y1 = cos(36 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(36 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(0.0f, _y1, _z1);
	meshData.Vertices.push_back(point);


	_y1 = cos(0 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(0 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(0.0f, _y1, _z1);
	meshData.Vertices.push_back(point);


	*/
}


//******************************************************************************************************************************
// CreateAxisCircle Y
//******************************************************************************************************************************

void Mesh::CreateAxisCircleY(UINT multiplySize, MeshData &meshData)
{
	//Kolor - jest definiowany za pomoca zmiennej "Normal". W hlsl (fx) jego wartosc jest konwertowana z postaci
	//normal do color --> np. dla Normal XMFLOAT3(1.0f, 0.0f, 0.0f) ==> Color { 1.0f, 0.0f, 0.0f, 1.0f} ostatnia wartosc 
	//jest dodawana recznie w shaderze
	// Funkcja cieniujaca, które odpowiada za kolor -- 'PS_WIRE' w 'ShandardShader.fx'

	Vertex point;

	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Kolor dla wszystkich

	float _x1 = cos(0 * XM_PI / 45) * (3.0f * multiplySize);
	float _z1 = sin(0 * XM_PI / 45) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, 0.0f, 0.0f);
	meshData.Vertices.push_back(point);

	for (int i = 1; i <= 71; i++)
	{
		float _x = cos(i * XM_PI / 36) * (3.0f * multiplySize);
		float _z = sin(i * XM_PI / 36) * (3.0f * multiplySize);
		point.Pos = XMFLOAT3(_x, 0.0f, _z);
		meshData.Vertices.push_back(point);

		_x = cos(i * XM_PI / 36) * (3.0f * multiplySize);
		_z = sin(i * XM_PI / 36) * (3.0f * multiplySize);
		point.Pos = XMFLOAT3(_x, 0.0f, _z);
		meshData.Vertices.push_back(point);

	}

	_x1 = cos(72 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(72 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, 0.0f, _z1);
	meshData.Vertices.push_back(point);



	_x1 = cos(18 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(18 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, 0.0f, _z1);
	meshData.Vertices.push_back(point);


	_x1 = cos(-18 * XM_PI / 36) * (3.0f * multiplySize);
	_z1 = sin(-18 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, 0.0f, _z1);
	meshData.Vertices.push_back(point);





}

//******************************************************************************************************************************
// CreateAxisCircle Z
//******************************************************************************************************************************
void Mesh::CreateAxisCircleZ(UINT multiplySize, MeshData &meshData)
{
	//Kolor - jest definiowany za pomoca zmiennej "Normal". W hlsl (fx) jego wartosc jest konwertowana z postaci
	//normal do color --> np. dla Normal XMFLOAT3(1.0f, 0.0f, 0.0f) ==> Color { 1.0f, 0.0f, 0.0f, 1.0f} ostatnia wartosc 
	//jest dodawana recznie w shaderze
	// Funkcja cieniujaca, które odpowiada za kolor -- 'PS_WIRE' w 'ShandardShader.fx'

	Vertex point;
	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Kolor dla wszystkich

	float _x1 = cos(0 * XM_PI / 45) * (3.0f * multiplySize);
	float _y1 = sin(0 * XM_PI / 45) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, _y1, 0.0f);
	meshData.Vertices.push_back(point);

	for (int i = 1; i <= 71; i++)
	{
		float _x = cos(i * XM_PI / 36) * (3.0f * multiplySize);
		float _y = sin(i * XM_PI / 36) * (3.0f * multiplySize);
		point.Pos = XMFLOAT3(_x, _y, 0.0f);
		meshData.Vertices.push_back(point);

		_x = cos(i * XM_PI / 36) * (3.0f * multiplySize);
		_y = sin(i * XM_PI / 36) * (3.0f * multiplySize);
		point.Pos = XMFLOAT3(_x, _y, 0.0f);
		meshData.Vertices.push_back(point);

	}

	_x1 = cos(72 * XM_PI / 36) * (3.0f * multiplySize);
	_y1 = sin(72 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, _y1, 0.0f);
	meshData.Vertices.push_back(point);


	_x1 = cos(36 * XM_PI / 36) * (3.0f * multiplySize);
	_y1 = sin(36 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, _y1, 0.0f);
	meshData.Vertices.push_back(point);

	_x1 = cos(0 * XM_PI / 36) * (3.0f * multiplySize);
	_y1 = sin(0 * XM_PI / 36) * (3.0f * multiplySize);
	point.Pos = XMFLOAT3(_x1, _y1, 0.0f);
	meshData.Vertices.push_back(point);


}



//******************************************************************************************************************************
// CreateAxisArrow X
//******************************************************************************************************************************
void Mesh::CreateAxisArrowX(UINT multiplySize, MeshData &meshData)
{

	Vertex point;

	UINT x = multiplySize;


	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*0.15f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*-0.15f, x*0.0f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*0.15f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*-0.15f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*0.15f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*0.15f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*0.15f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*-0.15f, x*0.0f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*-0.15f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*0.15f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*2.6f, x*0.0f, x*-0.15f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.6f, x*-0.15f, x*0.0f);  meshData.Vertices.push_back(point);

}


//******************************************************************************************************************************
// CreateAxisArrow Y
//******************************************************************************************************************************
void Mesh::CreateAxisArrowY(UINT multiplySize, MeshData &meshData)
{

	Vertex point;

	UINT x = multiplySize;


	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*2.6f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*2.6f, x*0.0f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*0.15f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*-0.15f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*0.15f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*2.6f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*0.15f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*2.6f, x*0.0f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*-0.15f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*2.6f, x*0.0f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*2.6f, x*-0.15f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*2.6f, x*0.0f);  meshData.Vertices.push_back(point);


}


//******************************************************************************************************************************
// CreateAxisArrow Z
//******************************************************************************************************************************
void Mesh::CreateAxisArrowZ(UINT multiplySize, MeshData &meshData)
{

	Vertex point;

	UINT x = multiplySize;

	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*2.6f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*2.6f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*2.6f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*2.6f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*2.6f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*2.6f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*2.6f);   meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*2.6f);  meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*2.6f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*2.6f);   meshData.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*2.6f);  meshData.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*2.6f);  meshData.Vertices.push_back(point);


}





UINT Mesh::GetCountAxisRotate()
{
	return mCountAxisRotate;
}




//******************************************************************************************************************************
// CreateAxisArrows in center of scene
//******************************************************************************************************************************
void Mesh::CreateAxisArrows(UINT multiplySize, MeshData &AxisArrows)
{
	Vertex point;

	UINT x = multiplySize;
	
	mCountAxisRotate = 0;
	//Kolor - jest definiowany za pomoca zmiennej "Normal". W hlsl (fx) jego wartosc jest konwertowana z postaci
	//normal do color --> np. dla Normal XMFLOAT3(1.0f, 0.0f, 0.0f) ==> Color { 1.0f, 0.0f, 0.0f, 1.0f} ostatnia wartosc 
	//jest dodawana recznie w shaderze
	// Funkcja cieniujaca, które odpowiada za kolor -- 'PS_WIRE' w 'ShandardShader.fx'


	// Dla X
	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.4f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.15f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.4f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*-0.15f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.4f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.4f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.15f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*-0.15f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.15f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*-0.15f, x*0.0f);  AxisArrows.Vertices.push_back(point);


	//X & Y
	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.5f, 0.0f, 0.5f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*1.5f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.5f, 0.0f, 0.5f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*1.5f, x*0.0f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	//X & Z 
	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.5f, 0.5f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*1.5f);  AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.5f, 0.5f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*1.5f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	//Y & Z 
	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.0f, 0.5f, 0.5f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*1.5f);  AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.0f, 0.5f, 0.5f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*1.5f);  AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //colour
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);  AxisArrows.Vertices.push_back(point);



	// Dla Y
	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.4f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*3.0f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.4f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*3.0f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.4f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.4f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*3.0f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.15f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*3.0f, x*0.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*3.0f, x*0.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*-0.15f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*3.0f, x*0.0f);  AxisArrows.Vertices.push_back(point);






	// Dla Z
	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Kolor dla wszystkich

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.4f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*3.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.4f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*3.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.4f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*3.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.4f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*3.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*3.0f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*3.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.15f, x*3.0f);   AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*3.0f);  AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*3.0f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.15f, x*0.0f, x*3.0f);   AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*-0.15f, x*3.0f);  AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.15f, x*0.0f, x*3.0f);  AxisArrows.Vertices.push_back(point);

	//***********************************************************************************************************8

	float baseSize = 3.0f;
	XMVECTOR normalVector;

	//------------------------------------------------------
	float _xa = 0.0f, _ya = 0.0f, _za = 0.0f;
	int iter = 90;
	//----------------------
	// X - circle
	//---------------------
	point.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); //line colou
	for (int i = 0; i < iter; ++i)
	{
		_ya = cos(i * XM_PI / 36) * (baseSize * multiplySize);
		_za = sin(i * XM_PI / 36) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(0.0f, _ya, _za);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}
	//----------------------
	// Y - circle
	//---------------------
	point.Tangent = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); //line colour
	for (int i = 0; i < iter; ++i)
	{
		_xa = cos(i * XM_PI / 36) * (baseSize * multiplySize);
		_za = sin(i * XM_PI / 36) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(_xa, 0.0f, _za);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}

	//----------------------
	// Z - circle
	//---------------------
	point.Tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f); //line colour
	for (int i = 0; i < iter; ++i)
	{
		_xa = cos(i * XM_PI / 36) * (baseSize * multiplySize);
		_ya = sin(i * XM_PI / 36) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(_xa, _ya, 0.0f);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}



	//---------------------------------------
	//Scale X
	//---------------------------------------

	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Colour

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*3.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	//Create box
	CreateAxisBox(0.8f, AxisArrows, XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(16.0f, 0.0f, 0.0f));

	//---------------------------------------
	//Scale Y
	//---------------------------------------

	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Colour

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*3.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	//Create box
	CreateAxisBox(0.8f, AxisArrows, XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 16.0f, 0.0f));

	//---------------------------------------
	//Scale Z
	//---------------------------------------

	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Colour

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*3.0f);    AxisArrows.Vertices.push_back(point);
	//Create box
	CreateAxisBox(0.8f, AxisArrows, XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 16.0f));

	//---------------------------------------
	//Scale join
	//---------------------------------------

	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);


	point.Normal = XMFLOAT3(1.0f, 1.0f, 1.0f); //Colour
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);



	//Gizmo triangles (move)
	// XZ - move gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);

	// XY - move gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*1.5f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	// YZ - move gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*1.5f);    AxisArrows.Vertices.push_back(point);


	//Gizmo triangles (scale) - detect only
	// XY - scale gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	// XZ - scale gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);

	// YZ - scale gizmo
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);

	// XYZ - scale gizmo
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);

	//Gizmo triangles (scale) - draw
	// XY - scale gizmo - selected area
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	// XZ - scale gizmo - selected area
	point.Pos = XMFLOAT3(x*1.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*2.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);

	// YZ - scale gizmo - selected area
	point.Pos = XMFLOAT3(x*0.0f, x*1.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*2.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*1.5f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*2.5f);    AxisArrows.Vertices.push_back(point);






	
	//------------------------------------------------------
	//Points to draw rotation (as triagles) 
	//------------------------------------------------------
	_xa = 0.0f, _ya = 0.0f, _za = 0.0f;
	iter = 360; //angle = 1
	//----------------------
	// X - circle
	//---------------------
	point.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); //line colou
	for (int i = 0; i < iter; ++i)
	{
		_ya = cos(i * XM_PI / 180) * (baseSize * multiplySize);
		_za = sin(i * XM_PI / 180) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(0.0f, _ya, _za);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}
	for (int i = (int)AxisArrows.Vertices.size() - 360; i < (int)AxisArrows.Vertices.size() - 1; ++i)
	{
		AxisArrows.Indices.push_back(0);//(0, 0, 0)
		AxisArrows.Indices.push_back((UINT)i);
		AxisArrows.Indices.push_back((UINT)(i+1));
	}

	//Last triangle of circle
	AxisArrows.Indices.push_back(0);//(0, 0, 0)
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 1));
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 360));

	//----------------------
	// Y - circle
	//---------------------
	point.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); //line colou
	for (int i = 0; i < iter; ++i)
	{
		_xa = cos(i * XM_PI / 180) * (baseSize * multiplySize);
		_za = sin(i * XM_PI / 180) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(_xa, 0.0f, _za);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}
	for (int i = (int)AxisArrows.Vertices.size() - 360; i < (int)AxisArrows.Vertices.size() - 1; ++i)
	{
		AxisArrows.Indices.push_back(0);//(0, 0, 0)
		AxisArrows.Indices.push_back((UINT)i);
		AxisArrows.Indices.push_back((UINT)(i + 1));
	}

	//Last triangle of circle
	AxisArrows.Indices.push_back(0);//(0, 0, 0)
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 1));
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 360));


	//----------------------
	// Z - circle
	//---------------------
	point.Tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); //line colou
	for (int i = 0; i < iter; ++i)
	{
		_xa = cos(i * XM_PI / 180) * (baseSize * multiplySize);
		_ya = sin(i * XM_PI / 180) * (baseSize * multiplySize);
		point.Pos = XMFLOAT3(_xa, _ya, 0.0f);
		normalVector = XMLoadFloat3(&point.Pos);
		XMStoreFloat3(&point.Normal, XMVector3Normalize(normalVector));
		AxisArrows.Vertices.push_back(point);
	}
	for (int i = (int)AxisArrows.Vertices.size() - 360; i < (int)AxisArrows.Vertices.size() - 1; ++i)
	{
		AxisArrows.Indices.push_back(0);//(0, 0, 0)
		AxisArrows.Indices.push_back((UINT)i);
		AxisArrows.Indices.push_back((UINT)(i + 1));
	}

	//Last triangle of circle
	AxisArrows.Indices.push_back(0);//(0, 0, 0)
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 1));
	AxisArrows.Indices.push_back((UINT)(AxisArrows.Vertices.size() - 360));



	// Line to center of circle
	point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.5f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.5f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.5f);    AxisArrows.Vertices.push_back(point);

	point.Normal = XMFLOAT3(0.5f, 0.5f, 0.5f); //Colour
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*-0.6f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*-0.6f, x*0.0f);    AxisArrows.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*0.0f);    AxisArrows.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x*0.0f, x*0.0f, x*-0.6f);    AxisArrows.Vertices.push_back(point);


}
//-----------------------------------------------------------------------------------------------------------------------
void Mesh::CreateAxisBox(float boxSize, MeshData &AxisBox, XMFLOAT3 colour, XMFLOAT3 offset)
{
	Vertex point;
	point.Normal = colour;
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);

	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);

	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);

	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*-1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*-1.0f + offset.z);    AxisBox.Vertices.push_back(point);
	point.Pos = XMFLOAT3(boxSize*1.0f + offset.x, boxSize*-1.0f + offset.y, boxSize*1.0f + offset.z);    AxisBox.Vertices.push_back(point);
}





//******************************************************************************************************************************
// CountFunctionValue
//******************************************************************************************************************************
float Mesh::CountFunctionValue(float x, float z)
{
	//__draw_min_max_Axis = true;
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}





//******************************************************************************************************************************
//CreateBox BOX(X,Y,Z)
//******************************************************************************************************************************
void Mesh::CreateBoxNEW(float x, float y, float z, XMFLOAT4 color, MeshData& Box)
{

	Vertex point;
	//point.Color = color; //Kolor dla wszystkich

	point.Pos = XMFLOAT3(-x, y, -z); point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, -z);  point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, z); point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);   Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, z); point.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); Box.Vertices.push_back(point);

	point.Pos = XMFLOAT3(-x, -y, -z); point.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, -z); point.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, z); point.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, -y, z); point.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f); Box.Vertices.push_back(point);

	point.Pos = XMFLOAT3(-x, -y, z); point.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, -y, -z); point.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, -z); point.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, z); point.Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);

	point.Pos = XMFLOAT3(x, -y, z); point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, -z); point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, -z); point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, z); point.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f); Box.Vertices.push_back(point);

	point.Pos = XMFLOAT3(-x, -y, -z); point.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, -z); point.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, -z); point.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, -z); point.Normal = XMFLOAT3(0.0f, 0.0f, -1.0f); Box.Vertices.push_back(point);

	point.Pos = XMFLOAT3(-x, -y, z); point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, -y, z); point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(x, y, z); point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); Box.Vertices.push_back(point);
	point.Pos = XMFLOAT3(-x, y, z); point.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f); Box.Vertices.push_back(point);





	Box.Indices.push_back(3); Box.Indices.push_back(1); Box.Indices.push_back(0);
	Box.Indices.push_back(2); Box.Indices.push_back(1); Box.Indices.push_back(3);

	Box.Indices.push_back(6); Box.Indices.push_back(4); Box.Indices.push_back(5);
	Box.Indices.push_back(7); Box.Indices.push_back(4); Box.Indices.push_back(6);

	Box.Indices.push_back(11); Box.Indices.push_back(9); Box.Indices.push_back(8);
	Box.Indices.push_back(10); Box.Indices.push_back(9); Box.Indices.push_back(11);

	Box.Indices.push_back(14); Box.Indices.push_back(12); Box.Indices.push_back(13);
	Box.Indices.push_back(15); Box.Indices.push_back(12); Box.Indices.push_back(14);

	Box.Indices.push_back(19); Box.Indices.push_back(17); Box.Indices.push_back(16);
	Box.Indices.push_back(18); Box.Indices.push_back(17); Box.Indices.push_back(19);

	Box.Indices.push_back(22); Box.Indices.push_back(20); Box.Indices.push_back(21);
	Box.Indices.push_back(23); Box.Indices.push_back(20); Box.Indices.push_back(22);
}

//*******************************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************************
void Mesh::LoadMeshFromFile(std::string fileName, MeshData &LoadMesh)
{
	std::ifstream fin(fileName);

	if (!fin)
	{
		MessageBox(0, L"Obiekt nie istnieje!!!", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;




	Vertex point;
	//point.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	std::wostringstream xy__;
	xy__.precision(6);


	//std::vector<Vertex::PosNormal> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{

		fin >> point.Pos.x >> point.Pos.y >> point.Pos.z;
		//xy__ << L"x = " << point.Pos.x << L"  y = " << point.Pos.y << L"  z = " << point.Pos.z;
		//MessageBox(NULL, xy__.str().c_str(), L"x,y,z", NULL);
		//	fin >> LoadMesh.Vertices[i].Pos.x >> LoadMesh.Vertices[i].Pos.y >> LoadMesh.Vertices[i].Pos.z;
		//	fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;ignore
		fin >> point.Normal.x >> point.Normal.y >> point.Normal.z;
		//fin >> ignore >> ignore >> ignore;
		LoadMesh.Vertices.push_back(point);
	}



	//MessageBox(0, L"Models/skull.txt nie istnieje123123!.", 0, 0);
	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	//LoadMeshCountIndices = 3 * tcount;
	//std::vector<UINT> indices(mSkullIndexCount
	UINT TmpIndex_1 = 0;
	UINT TmpIndex_2 = 0;
	UINT TmpIndex_3 = 0;
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> TmpIndex_1 >> TmpIndex_2 >> TmpIndex_3;
		LoadMesh.Indices.push_back(TmpIndex_1);
		LoadMesh.Indices.push_back(TmpIndex_2);
		LoadMesh.Indices.push_back(TmpIndex_3);

		//xy__ << L"x = " << TmpIndex_1 << L"  y = " << TmpIndex_2 << L"  z = " << TmpIndex_3;
		//MessageBox(NULL, xy__.str().c_str(), L"x,y,z", NULL);
		//fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	UINT LoadMeshCountIndices_02 = 3 * LoadMesh.Indices.size();


	//xy__ << L"LoadMeshCountIndices = " << LoadMeshCountIndices;
	//MessageBox(NULL, xy__.str().c_str(), L"x,y,z", NULL);

	fin.close();

}

void Mesh::LoadMeshFromFileOBJCount(std::string fileName, int& v_count, int& vn_count, int& vt_count, int& index_count)
{

	std::ifstream fin(fileName);

	if (!fin)
	{
		MessageBox(0, L"Nie mozna wczytac pliku!!!", 0, 0);
		return;
	}


	v_count = 0;
	vn_count = 0;
	vt_count = 0;
	index_count = 0;

	wchar_t charIn;
	charIn = fin.get();
	std::string ignore;

	while (fin)
	{
		switch (charIn)
		{
			//------------------------------------------------------------
			// # case - wczytuje komentarze
			//------------------------------------------------------------
		case '#':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// m case - wczytuje material
			//------------------------------------------------------------
		case 'm':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// u case - wczytuje uzywany typ materialu
			//------------------------------------------------------------
		case 'u':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// g case - wczytuje grupe
			//------------------------------------------------------------
		case 'g':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// s case - wczytuje smooth group
			//------------------------------------------------------------
		case 's':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;


			//------------------------------------------------------------
			// v case - typy vertex'ow 
			//------------------------------------------------------------
		case 'v':
			charIn = fin.get();
			switch (charIn)
			{
				//**********************
				//wczytanie vertex'ow
				//**********************
			case ' ':
				fin >> ignore >> ignore >> ignore;
				++v_count; // To jest wazne !!!
				while (charIn != '\n') //pomijanie calej linijki
					charIn = fin.get();
				break;
				//**********************
				//wczytanie normali
				//**********************
			case 'n':
				fin >> ignore >> ignore >> ignore;
				++vn_count; // To jest wazne !!!
				while (charIn != '\n') //pomijanie calej linijki
					charIn = fin.get();
				break;
				//**********************
				//wczytanie wspolrzednych tekstur
				//**********************
			case 't':


				fin >> ignore >> ignore;
				++vt_count; // To jest wazne !!!
				while (charIn != '\n') //pomijanie calej linijki
					charIn = fin.get();
				break;

			}
			break;

			//------------------------------------------------------------
			// f case - wczytuje "faces"
			//------------------------------------------------------------
		case 'f':
			//++index_count; // To jest wazne !!!
			//while (charIn != '\n') //pomijanie calej linijki
			//	charIn = fin.get();
			index_count = vn_count; //Powinno byc tyle, ile vertex'ow normalnych!
			fin.close(); // Zamkniecie pliku.
			return; //Wyjscie z funkcji.

			break;
			//------------------------------------------------------------
			//default
			//------------------------------------------------------------
		default:
			charIn = fin.get();
			break;
		}
	}

	fin.close(); // Zamkniecie pliku.
	return; //Wyjscie z funkcji.


}
//*******************************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************************
bool Mesh::LoadMeshFromFileOBJ(std::string fileName, MeshData &LoadMesh, XMFLOAT3 &center, XMFLOAT3 &extents, XMVECTOR &maxVec, XMVECTOR &minVec)
{

	//Sprawdzenie ilosci vertex'ow -- potrzebne do tworzenia vectora o takiej wielkosci

	int v_count = 0;
	int vn_count = 0;
	int vt_count = 0;
	int index_count = 0;
	LoadMeshFromFileOBJCount(fileName, v_count, vn_count, vt_count, index_count); //Funkcja prywatna.
	//INFO_MESSAGE_3(v_count, vn_count, index_count);



	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity); //+inf, +inf, +inf
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity); //-inf, -inf, -inf
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);



	std::ifstream fin(fileName);

	if (!fin)
	{
		MessageBox(0, L"Nie mozna wczytac pliku!!!", 0, 0);
		return false;
	}

	//std::vector<XMFLOAT3> vertexPosition(v_count); //Set size
	//std::vector<XMFLOAT3> vertexNormal(vn_count);//Set size
	//std::vector<XMFLOAT2> vertexTexCoord(vt_count);//Set size
	//std::vector<XMINT3> indicesTmp(index_count);//Set size

	std::vector<XMFLOAT3> vertexPosition(v_count, XMFLOAT3(0, 0, 0)); //Set size and set value
	std::vector<XMFLOAT3> vertexNormal(vn_count, XMFLOAT3(0, 0, 0)); //Set size and set value
	std::vector<XMFLOAT2> vertexTexCoord(vt_count, XMFLOAT2(0, 0)); //Set size and set value
	std::vector<XMUINT3> indicesTmp(index_count, XMUINT3(0, 0, 0)); //Set size and set value


	wchar_t charIn;

	charIn = fin.get();
	//UINT vcount = 0;
	//UINT tcount = 0;
	std::string ignore;

	int vcount = 0;
	int ncount = 0;
	int tcount = 0;
	int indexcount = 0;

	//Zmienne dla 'f' case "Faces"
	std::string jeden = "";
	std::string dwa = "";
	std::string trzy = "";
	UINT jedenINT = 0;
	UINT dwaINT = 0;
	UINT trzyINT = 0;

	XMVECTOR P;


	while (fin)
	{
		switch (charIn)
		{
			//------------------------------------------------------------
			// # case - wczytuje komentarze
			//------------------------------------------------------------
		case '#':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// m case - wczytuje material
			//------------------------------------------------------------
		case 'm':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// u case - wczytuje uzywany typ materialu
			//------------------------------------------------------------
		case 'u':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// g case - wczytuje grupe
			//------------------------------------------------------------
		case 'g':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;
			//------------------------------------------------------------
			// s case - wczytuje smooth group
			//------------------------------------------------------------
		case 's':
			charIn = fin.get();
			while (charIn != '\n')
				charIn = fin.get();
			break;


			//------------------------------------------------------------
			// v case - typy vertex'ow 
			//------------------------------------------------------------
		case 'v':
			charIn = fin.get();
			switch (charIn)
			{
				//**********************
				//wczytanie vertex'ow
				//**********************
			case ' ':

				float v_x, v_y, v_z;
				fin >> v_x >> v_y >> v_z;
				//vertexPosition.push_back(XMFLOAT3(v_x, v_y, v_z));
				vertexPosition[vcount] = XMFLOAT3(v_x, v_y, v_z);

				//Wyznaczanie max i min
				P = XMLoadFloat3(&XMFLOAT3(v_x, v_y, v_z));
				vMin = XMVectorMin(vMin, P);
				vMax = XMVectorMax(vMax, P);

				++vcount;

				while (charIn != '\n')
					charIn = fin.get();
				break;
				//**********************
				//wczytanie normali
				//**********************
			case 'n':

				float vn_x, vn_y, vn_z;
				fin >> vn_x >> vn_y >> vn_z;
				//vertexNormal.push_back(XMFLOAT3(vn_x, vn_y, vn_z));
				vertexNormal[ncount] = XMFLOAT3(vn_x, vn_y, vn_z);

				++ncount;
				while (charIn != '\n')
					charIn = fin.get();
				break;
				//**********************
				//wczytanie wspolrzednych tekstur
				//**********************
			case 't':

				float vt_x, vt_y;
				float dirty;
				fin >> vt_x >> vt_y >> dirty;
				//vertexTexCoord.push_back(XMFLOAT2(vt_x, vt_y));
				vertexTexCoord[tcount] = XMFLOAT2(vt_x, vt_y);

				++tcount;
				while (charIn != '\n')
					charIn = fin.get();
				break;

			}
			break;

			//------------------------------------------------------------
			// f case - wczytuje "faces"
			//------------------------------------------------------------
			case 'f':
				//Wczytaj spacje (brak akcji dla tego char'a)
				charIn = fin.get();

				jeden = "";
				dwa = "";
				trzy = "";
				charIn = fin.get();
				while (charIn != '\n')
				{
					//-------------------------------
					while (charIn != '/')
					{
						jeden += charIn;
						charIn = fin.get();
					}
					//-------------------------------
					charIn = fin.get();
					while (charIn != '/')
					{
						dwa += charIn;
						charIn = fin.get();
					}
					//-------------------------------
					charIn = fin.get();
					while (charIn != ' ')
					{
						trzy += charIn;
						charIn = fin.get();
					}
					//-------------------------------
					//-------------------------------


					//konwersja z 'string' do 'int' 
					std::istringstream iss1(jeden);
					iss1 >> jedenINT;
					std::istringstream iss2(dwa);
					iss2 >> dwaINT;
					std::istringstream iss3(trzy);
					iss3 >> trzyINT;

					//indicesTmp.push_back(XMUINT3(jedenINT, dwaINT, trzyINT));
					indicesTmp[indexcount] = XMUINT3(jedenINT, dwaINT, trzyINT);

					++indexcount;

					
					//Czyszczenie zmiennych
					jeden = "";
					dwa = "";
					trzy = "";

					charIn = fin.get();

					//--------------------------------------------------------------
				}
				break;
		//------------------------------------------------------------
		//default
		//------------------------------------------------------------
		default:
			charIn = fin.get();
			break;
		}

	}

	//---------------------------------------------------------------------------------
	//Interpretowanie danych
	//---------------------------------------------------------------------------------
	Vertex point;

	XMFLOAT3 sdir;
	XMFLOAT3 tdir;
	

	int counterIndex = 0;

	//Przykladowy 'Face'
	//	4/4/13	5/5/14	8/8/15 
	//  pierwsza wartosc = v
	//  druga wartosc = vt
	//  trzecia wartosc = vn

	//Numerowanie zaczyna sie od 1. Trzeba zmienic, aby zaczynalo sie od 0 !!!
	LoadMesh.Vertices.resize(indexcount); //resize vector !!!
	LoadMesh.Indices.resize(indexcount);
	for (int i = 0; i < (int)indicesTmp.size(); ++i)
	{
		//Dla v
		point.Pos.x = vertexPosition[(indicesTmp[i].x) - 1].x;
		point.Pos.y = vertexPosition[(indicesTmp[i].x) - 1].y;
		point.Pos.z = vertexPosition[(indicesTmp[i].x) - 1].z;
		//INFO_MESSAGE_3(point.Pos.x, point.Pos.y, point.Pos.z);

		//Dla vt
		//Dopisac!!!!

		point.TexC.x = vertexTexCoord[(indicesTmp[i].y) - 1].x;
		point.TexC.y = vertexTexCoord[(indicesTmp[i].y) - 1].y;


		//Dla vn
		point.Normal.x = vertexNormal[(indicesTmp[i].z) - 1].x;
		point.Normal.y = vertexNormal[(indicesTmp[i].z) - 1].y;
		point.Normal.z = vertexNormal[(indicesTmp[i].z) - 1].z;
		//INFO_MESSAGE_3(point.Normal.x, point.Normal.y, point.Normal.z);
		
		LoadMesh.Vertices[i] = point;
		//LoadMesh.Vertices.push_back(point);
		++counterIndex;

		//Jesli wystapily 3 indeksy, to dodajemy jej do pamieci
		//Nalezy wczytywac 3 wartosc!!!
		if (counterIndex % 3 == 0)
		{
			//LoadMesh.Indices.push_back((indicesTmp[i-2].z) -1); //index 1
			//LoadMesh.Indices.push_back((indicesTmp[i - 1].z) - 1); //index 2
			//LoadMesh.Indices.push_back((indicesTmp[i].z) - 1); //index 3

			LoadMesh.Indices[i - 2] = (indicesTmp[i - 2].z) - 1; //index 1
			LoadMesh.Indices[i - 1] = (indicesTmp[i - 1].z) - 1; //index 2
			LoadMesh.Indices[i] = (indicesTmp[i].z) - 1; //index 3
		}
	}

	
	//***************************************************************************************
	//Tworzenie stycznej do punktow (Tangent)
	//***************************************************************************************
	for (int i = 0; i < (int)LoadMesh.Vertices.size(); i += 3)
	{
		Vertex vertex0 = LoadMesh.Vertices[LoadMesh.Indices[i + 0]];
		Vertex vertex1 = LoadMesh.Vertices[LoadMesh.Indices[i + 1]];
		Vertex vertex2 = LoadMesh.Vertices[LoadMesh.Indices[i + 2]];

		vertex0.Tangent.x = 0.0f;
		vertex0.Tangent.y = 0.0f;
		vertex0.Tangent.z = 0.0f;

		vertex1.Tangent.x = 0.0f;
		vertex1.Tangent.y = 0.0f;
		vertex1.Tangent.z = 0.0f;

		vertex2.Tangent.x = 0.0f;
		vertex2.Tangent.y = 0.0f;
		vertex2.Tangent.z = 0.0f;

		float x_1 = vertex1.Pos.x - vertex0.Pos.x;
		float y_1 = vertex1.Pos.y - vertex0.Pos.y;
		float z_1 = vertex1.Pos.z - vertex0.Pos.z;
		XMFLOAT3 Edge1 = XMFLOAT3(x_1, y_1, z_1);

		float x_2 = vertex2.Pos.x - vertex0.Pos.x;
		float y_2 = vertex2.Pos.y - vertex0.Pos.y;
		float z_2 = vertex2.Pos.z - vertex0.Pos.z;
		XMFLOAT3 Edge2 = XMFLOAT3(x_2, y_2, z_2);

		float U1 = vertex1.TexC.x - vertex0.TexC.x;
		float V1 = vertex1.TexC.y - vertex0.TexC.y;

		float U2 = vertex2.TexC.x - vertex0.TexC.x;
		float V2 = vertex2.TexC.y - vertex0.TexC.y;


		float f = 1.0f / (U1 * V2 - U2 * V1);


		XMFLOAT3 tangent;
		tangent.x = (V2 * Edge1.x - V1 * Edge2.x) * f;
		tangent.y = (V2 * Edge1.y - V1 * Edge2.y) * f;
		tangent.z = (V2 * Edge1.z - V1 * Edge2.z) * f;


		//Normalizacja
		XMVECTOR vector = XMLoadFloat3(&tangent);
		vector = XMVector3Normalize(vector);
		tangent.x = XMVectorGetX(vector);
		tangent.y = XMVectorGetY(vector);
		tangent.z = XMVectorGetZ(vector);


		vertex0.Tangent.x += tangent.x;
		vertex0.Tangent.y += tangent.y;
		vertex0.Tangent.z += tangent.z;
		LoadMesh.Vertices[i] = vertex0;

		vertex1.Tangent.x += tangent.x;
		vertex1.Tangent.y += tangent.y;
		vertex1.Tangent.z += tangent.z;
		LoadMesh.Vertices[i + 1] = vertex1;

		vertex2.Tangent.x += tangent.x;
		vertex2.Tangent.y += tangent.y;
		vertex2.Tangent.z += tangent.z;
		LoadMesh.Vertices[i + 2] = vertex2;
	}
	//***************************************************************************************
	
	fin.close();

	XMStoreFloat3(&center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&extents, 0.5f*(vMax - vMin));

	maxVec = vMax;
	minVec = vMin;

	return true;
}