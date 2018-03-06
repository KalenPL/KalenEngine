#ifndef KLMESH_H
#define KLMESH_H

#include "Utility.h"
#include "LocalCoordinateSystem.h"
#include "Material.h"
#include "Mesh.h"
#include "File.h"
#include "../XNACollision/xnacollision.h"

//------------------------------------------------------------------------------------------
// Class KLMESH
//------------------------------------------------------------------------------------------
class KLMESH
{
public:
	KLMESH(klmeshFileStruct* klmesh, ID3D11Device* device);
	~KLMESH();

	//Name
	void SetFileName(std::string fileName);
	std::string GetFileName();

	//Link
	void SetLinkToFile(std::string link);
	std::string GetLinkToFile();

	//LODs
	void SetLodAmount(short amount);
	short GetLodAmount();
	void SetDistanceLOD0(UINT distance);					//Set distance to LOD0
	void SetDistanceLOD1(UINT distance);					//Set distance to LOD1
	void SetDistanceLOD2(UINT distance);					//Set distance to LOD2
	void SetDistanceHide(UINT distance);					//Set distance to Hide
	void SetShadowDistance(float shadowDistance);
	UINT GetDistanceLOD0();									//Return distance to LOD0
	UINT GetDistanceLOD1();									//Return distance to LOD1
	UINT GetDistanceLOD2();									//Return distance to LOD2
	UINT GetDistanceHide();									//Return distance to Hide
	float GetShadowDistance();

	//Buffers
	void CreateBufferVertexAndIndex(ID3D11Device* m_pDevice);	// Create Vertex and Index buffer
	ID3D11Buffer** GetVertexBuffer();							// Return pointer to VertexBuffer
	ID3D11Buffer* GetIndexBuffer();								// Return pointer to IndexBuffer

	//Buffers - Collision mesh
	void CreateBufferVertexAndIndexCollision(ID3D11Device* m_pDevice);	// Create Vertex and Index buffer
	ID3D11Buffer** GetVertexBufferCollsion();							// Return pointer to VertexBuffer
	ID3D11Buffer* GetIndexBufferCollsion();								// Return pointer to IndexBuffer
	MeshData GetMeshCollision();

	//Mesh - Chunks
	std::vector<Chunk*>* GetMeshToDraw(UINT distance);	// Return all chunk mesh to draw

	//Materials
	std::vector<Material>* GetMaterialBuffer();
	void SetMaterialBuffer(std::vector<Material>* materialBuffer);
	void SetMaterialShaderName(int materialPosition, std::string name);

	void SetMaterialDiffuseMap(int materialPosition, ID3D11ShaderResourceView* diffuseMap, std::string textureName);
	void SetMaterialNormalMap(int materialPosition, ID3D11ShaderResourceView* normalMap, std::string textureName);
	void SetMaterialASHMap(int materialPosition, ID3D11ShaderResourceView* ashMap, std::string textureName);
	void SetMaterialCubeMap(int materialPosition, ID3D11ShaderResourceView* cubeMap, std::string textureName);
	void SetMaterialAmbientColour(int materialPosition, XMFLOAT4 ambientColour);
	void SetMaterialDiffuseColour(int materialPosition, XMFLOAT4 diffuseColour);
	void SetMaterialSpecularColour(int materialPosition, XMFLOAT4 specularColour);
	void SetMaterialReflectColour(int materialPosition, XMFLOAT4 reflectColour);
	void SetMaterialAlphaColour(int materialPosition, float alphaColour);
	void SetMaterialRoughness(int materialPosition, float roughness);
	void SetMaterialFresnelFactor(int materialPosition, float fresnelFactor);
	void SetMaterialSpecularPower(int materialPosition, float specularPower);
	void SetMaterialTransparency(int materialPosition, float transparency);

	void SetMaterialHasTessellation(int materialPosition, bool hasTessellation);
	void SetMaterialHeightScaleTess(int materialPosition, float heightScaleTess);
	void SetMaterialMaxDistanceTess(int materialPosition, short maxDistanceTess);
	void SetMaterialMinDistanceTess(int materialPosition, short minDistanceTess);
	void SetMaterialMaxFactorTess(int materialPosition, short maxFactorTess);
	void SetMaterialMinFactorTess(int materialPosition, short minFactorTess);

	void SetMaterialDiffuseMapName(int materialPosition, std::string textureName);
	void SetMaterialNormalMapName(int materialPosition, std::string textureName);
	void SetMaterialASHMapName(int materialPosition, std::string textureName);
	void SetMaterialCubeMapName(int materialPosition, std::string textureName);

	void SetMaterialCubeMapDynamic(int materialPosition, bool value);

	

	//Collision
	collisionMeshStruct* GetCollMeshData();

	//AABB - vectors
	void SetBoundingBoxMax(XMVECTOR boundingBoxMax);  //Set BoundingBoxMax for XNA COLLION
	XMVECTOR GetBoundingBoxMax(); //Return BoundingBoxMax for XNA COLLION
	void SetBoundingBoxMin(XMVECTOR boundingBoxMin);  //Set BoundingBoxMin for XNA COLLION
	XMVECTOR GetBoundingBoxMin(); //Return BoundingBoxMin for XNA COLLION
	AxisAlignedBox* GetAxisAlignedBoxOrigin();

	void ConvertMaterialToMaterialStruct();

	ID3D11Buffer** GetBBoxVertexBuffer();
	MeshData GetBBoxMeshData();
	MeshData* GetAllMeshData();

	bool GetIsAllowRender();
	void SetIsAllowRender(bool signal);

private:
	klmeshFileStruct*		mKlmeshFileStruct;
	ID3D11Buffer*           mVertexBufferObject;
	ID3D11Buffer*           mIndexBufferObject;

	ID3D11Buffer*           mVertexBufferCollision;
	ID3D11Buffer*           mIndexBufferCollision;

	ID3D11Device*			mDevice;

	//collisionMeshStruct* mCollisionMesh;

	std::string mFileName;
	std::string mLinkToFile;
	short mLodAmount;

	//Store all vertices and indices of mesh
	MeshData* mAllMeshData;
	ID3D11Buffer*	mVertexBufferBBox;
	Mesh mMesh;
	MeshData mMeshBBox;
	MeshData mMeshCollision;

	//Lod distance
	UINT mDistanceLOD0;
	UINT mDistanceLOD1;
	UINT mDistanceLOD2;
	UINT mDistanceHide;

	std::vector<Chunk*>* mObjectsVector_LOD0;
	std::vector<Chunk*>* mObjectsVector_LOD1;
	std::vector<Chunk*>* mObjectsVector_LOD2;

	std::vector<Material>* mMaterialBuffer;

	//Values to XNA_COLLISION
	XMVECTOR mBoundingBoxMax;
	XMVECTOR mBoundingBoxMin;
	AxisAlignedBox mAxisAlignedBoxOrigin;
	UINT mDistanceShadow;
	bool mAllowRender = true;
};
#endif //KLMESH_H