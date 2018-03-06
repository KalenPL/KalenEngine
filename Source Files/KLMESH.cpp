#include "../Header Files/KLMESH.h"
#include "../Header Files/Utility.h"
#include "../Header Files/File.h"


KLMESH::KLMESH(klmeshFileStruct* klmesh, ID3D11Device* device)
{
	mKlmeshFileStruct = nullptr;
	mKlmeshFileStruct = new klmeshFileStruct();
	mKlmeshFileStruct->AllMeshData = klmesh->AllMeshData;
	mKlmeshFileStruct->boundingBoxMax = klmesh->boundingBoxMax;
	mKlmeshFileStruct->boundingBoxMin = klmesh->boundingBoxMin;
	mKlmeshFileStruct->castShadow = klmesh->castShadow;
	mKlmeshFileStruct->collisionMeshData = klmesh->collisionMeshData;
	mKlmeshFileStruct->lodAmount = klmesh->lodAmount;
	mKlmeshFileStruct->materialsBuffer = klmesh->materialsBuffer;
	mKlmeshFileStruct->mDistanceHide = klmesh->mDistanceHide;
	mKlmeshFileStruct->mDistanceLOD0 = klmesh->mDistanceLOD0;
	mKlmeshFileStruct->mDistanceLOD1 = klmesh->mDistanceLOD1;
	mKlmeshFileStruct->mDistanceLOD2 = klmesh->mDistanceLOD2;
	mKlmeshFileStruct->mObjectsVector_LOD0 = klmesh->mObjectsVector_LOD0;
	mKlmeshFileStruct->mObjectsVector_LOD1 = klmesh->mObjectsVector_LOD1;
	mKlmeshFileStruct->mObjectsVector_LOD2 = klmesh->mObjectsVector_LOD2;
	mKlmeshFileStruct->name = klmesh->name;
	mKlmeshFileStruct->shadowDistance = klmesh->shadowDistance;




	mDevice = device;



	mVertexBufferObject = nullptr;
	mIndexBufferObject = nullptr;
	mMaterialBuffer = nullptr;
	mObjectsVector_LOD0 = nullptr;
	mObjectsVector_LOD1 = nullptr;
	mObjectsVector_LOD2 = nullptr;

	mObjectsVector_LOD0 = new std::vector<Chunk*>();
	mObjectsVector_LOD1 = new std::vector<Chunk*>();
	mObjectsVector_LOD2 = new std::vector<Chunk*>();

	for (int i = 0; i < (int)mKlmeshFileStruct->mObjectsVector_LOD0.size(); ++i)
	{
		mObjectsVector_LOD0->push_back(&mKlmeshFileStruct->mObjectsVector_LOD0.at(i));
	}
	for (int i = 0; i < (int)mKlmeshFileStruct->mObjectsVector_LOD1.size(); ++i)
	{
		mObjectsVector_LOD1->push_back(&mKlmeshFileStruct->mObjectsVector_LOD1.at(i));
	}
	for (int i = 0; i < (int)mKlmeshFileStruct->mObjectsVector_LOD2.size(); ++i)
	{
		mObjectsVector_LOD2->push_back(&mKlmeshFileStruct->mObjectsVector_LOD2.at(i));
	}

	mAllMeshData = nullptr;
	mAllMeshData = &mKlmeshFileStruct->AllMeshData;

	mDistanceLOD0 = mKlmeshFileStruct->mDistanceLOD0;
	mDistanceLOD1 = mKlmeshFileStruct->mDistanceLOD1;
	mDistanceLOD2 = mKlmeshFileStruct->mDistanceLOD2;
	mDistanceHide = mKlmeshFileStruct->mDistanceHide;

	mBoundingBoxMax = XMLoadFloat3(&mKlmeshFileStruct->boundingBoxMax);
	mBoundingBoxMin = XMLoadFloat3(&mKlmeshFileStruct->boundingBoxMin);

	XMFLOAT3 temp = XMFLOAT3(0.0f, 0.0f, 0.0f);
	temp.x = (mKlmeshFileStruct->boundingBoxMin.x + mKlmeshFileStruct->boundingBoxMax.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMin.y + mKlmeshFileStruct->boundingBoxMax.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMin.z + mKlmeshFileStruct->boundingBoxMax.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Center = temp;

	temp.x = (mKlmeshFileStruct->boundingBoxMax.x - mKlmeshFileStruct->boundingBoxMin.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMax.y - mKlmeshFileStruct->boundingBoxMin.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMax.z - mKlmeshFileStruct->boundingBoxMin.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Extents = temp;

	this->mFileName = mKlmeshFileStruct->name;
	this->mLinkToFile = mKlmeshFileStruct->name;
	this->mLodAmount = mKlmeshFileStruct->lodAmount;

	this->mDistanceShadow = mKlmeshFileStruct->shadowDistance;
	this->mDistanceLOD0 = mKlmeshFileStruct->mDistanceLOD0;
	this->mDistanceLOD1 = mKlmeshFileStruct->mDistanceLOD1;
	this->mDistanceLOD2 = mKlmeshFileStruct->mDistanceLOD2;
	this->mDistanceHide = mKlmeshFileStruct->mDistanceHide;

	//Create collision mesh
	mMesh.CreateCollision(mKlmeshFileStruct->collisionMeshData.verticesPositionVector, mKlmeshFileStruct->collisionMeshData.verticesNormalVector, mKlmeshFileStruct->collisionMeshData.faceDataVector, mMeshCollision);


	//Material
	mMaterialBuffer = new std::vector<Material>();

	Material* mat = new Material();
		
	for (int i = 0; i < (int)mKlmeshFileStruct->materialsBuffer.size(); ++i)
	{
		
		mat->shaderName = mKlmeshFileStruct->materialsBuffer[i].shaderName;
		mat->materialName = mKlmeshFileStruct->materialsBuffer[i].materialName;

		mat->ambientColour = XMFLOAT4(mKlmeshFileStruct->materialsBuffer[i].ambientColour.x, mKlmeshFileStruct->materialsBuffer[i].ambientColour.y, mKlmeshFileStruct->materialsBuffer[i].ambientColour.z, 1.0f);
		mat->materialHlsl.Ambient = mat->ambientColour;

		mat->diffuseColour = XMFLOAT4(mKlmeshFileStruct->materialsBuffer[i].diffuseColour.x, mKlmeshFileStruct->materialsBuffer[i].diffuseColour.y, mKlmeshFileStruct->materialsBuffer[i].diffuseColour.z, 1.0f);
		mat->materialHlsl.Diffuse = mat->diffuseColour;

		mat->specularColour = XMFLOAT4(mKlmeshFileStruct->materialsBuffer[i].specularColour.x, mKlmeshFileStruct->materialsBuffer[i].specularColour.y, mKlmeshFileStruct->materialsBuffer[i].specularColour.z, 1.0f);
		mat->materialHlsl.Specular = mat->specularColour;

		mat->reflectColour = XMFLOAT4(mKlmeshFileStruct->materialsBuffer[i].reflectColour.x, mKlmeshFileStruct->materialsBuffer[i].reflectColour.y, mKlmeshFileStruct->materialsBuffer[i].reflectColour.z, 1.0f);
		mat->materialHlsl.Reflect = mat->reflectColour;

		mat->alphaColour = mKlmeshFileStruct->materialsBuffer[i].alphaColour;

		mat->diffuseMapName = mKlmeshFileStruct->materialsBuffer[i].diffuseMapName;
		mat->normalMapName = mKlmeshFileStruct->materialsBuffer[i].normalMapName;
		mat->ashMapName = mKlmeshFileStruct->materialsBuffer[i].ashMapName;
		mat->cubeMapName = mKlmeshFileStruct->materialsBuffer[i].cubeMapName;

		mat->diffuseMap = nullptr;
		mat->normalMap = nullptr;
		mat->ashMap = nullptr;
		mat->cubeMap = nullptr;

		mat->fresnelFactor = mKlmeshFileStruct->materialsBuffer[i].fresnelFactor;
		mat->materialHlsl.fresnelFactor = mKlmeshFileStruct->materialsBuffer[i].fresnelFactor;

		mat->roughness = mKlmeshFileStruct->materialsBuffer[i].roughness;
		mat->materialHlsl.roughness = mKlmeshFileStruct->materialsBuffer[i].roughness;

		mat->specularPower = mKlmeshFileStruct->materialsBuffer[i].specularPower;
		mat->materialHlsl.specularPower = mKlmeshFileStruct->materialsBuffer[i].specularPower;

		//transparency
		mat->transparency = mKlmeshFileStruct->materialsBuffer[i].transparency;

		mat->hasTessellation = mKlmeshFileStruct->materialsBuffer[i].hasTessellation;
		mat->maxDistanceTess = mKlmeshFileStruct->materialsBuffer[i].maxDistanceTess;
		mat->materialHlsl.maxDistanceTess = mKlmeshFileStruct->materialsBuffer[i].maxDistanceTess;
		mat->minDistanceTess = mKlmeshFileStruct->materialsBuffer[i].minDistanceTess;
		mat->materialHlsl.minDistanceTess = mKlmeshFileStruct->materialsBuffer[i].minDistanceTess;
		mat->maxFactorTess = mKlmeshFileStruct->materialsBuffer[i].maxFactorTess;
		mat->materialHlsl.maxFactorTess = mKlmeshFileStruct->materialsBuffer[i].maxFactorTess;
		mat->minFactorTess = mKlmeshFileStruct->materialsBuffer[i].minFactorTess;
		mat->materialHlsl.minFactorTess = mKlmeshFileStruct->materialsBuffer[i].minFactorTess;
		mat->heightScaleTess = mKlmeshFileStruct->materialsBuffer[i].heightScaleTess;
		mat->materialHlsl.heightScaleTess = mKlmeshFileStruct->materialsBuffer[i].heightScaleTess;

		mat->cubeMapDynamic= mKlmeshFileStruct->materialsBuffer[i].cubeMapDynamic;

		this->mMaterialBuffer->push_back(*mat); //Add material to the materialBuffer
		//mat.alphaMap = texturesBuffer->AddTextureToBuffor(mat.alphaMapName);
	}

	//Create vertices and indices buffers
	CreateBufferVertexAndIndex(mDevice);
	CreateBufferVertexAndIndexCollision(mDevice);

	XMFLOAT3 bBoxMax;
	XMStoreFloat3(&bBoxMax, mBoundingBoxMax);
	XMFLOAT3 bBoxMin;
	XMStoreFloat3(&bBoxMin, mBoundingBoxMin);


	mMesh.CreateBoundingBoxView(bBoxMax, bBoxMin, mMeshBBox);
	//*************
	//BBox 
	//************
	D3D11_BUFFER_DESC vbd2;
	vbd2.Usage = D3D11_USAGE_DYNAMIC;
	vbd2.ByteWidth = sizeof(Vertex)* mMeshBBox.Vertices.size();
	vbd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd2.MiscFlags = 0;
	mDevice->CreateBuffer(&vbd2, 0, &mVertexBufferBBox);
	
	/*D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* mMeshBBox.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &mMeshBBox.Vertices[0];
	device->CreateBuffer(&bd, &InitData, &mVertexBufferBBox);
	*/




}
KLMESH::~KLMESH()
{
	mKlmeshFileStruct = nullptr;
	mVertexBufferObject = nullptr;
	mIndexBufferObject = nullptr;
	mMaterialBuffer->clear();
	mMaterialBuffer = nullptr;
	mObjectsVector_LOD0->clear();
	mObjectsVector_LOD0 = nullptr;
	mObjectsVector_LOD1->clear();
	mObjectsVector_LOD1 = nullptr;
	mObjectsVector_LOD2->clear();
	mObjectsVector_LOD2 = nullptr;
}
//*************************************************************************************************************************************
void KLMESH::CreateBufferVertexAndIndex(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* this->mAllMeshData->Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &this->mAllMeshData->Vertices[0];
	device->CreateBuffer(&bd, &InitData, &mVertexBufferObject);

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* this->mAllMeshData->Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	InitData.pSysMem = &this->mAllMeshData->Indices[0];
	device->CreateBuffer(&bd2, &InitData, &mIndexBufferObject);
}
//*************************************************************************************************************************************
void KLMESH::CreateBufferVertexAndIndexCollision(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* this->mMeshCollision.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &this->mMeshCollision.Vertices[0];
	device->CreateBuffer(&bd, &InitData, &mVertexBufferCollision);

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* this->mMeshCollision.Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	InitData.pSysMem = &this->mMeshCollision.Indices[0];
	device->CreateBuffer(&bd2, &InitData, &mIndexBufferCollision);
}
//*************************************************************************************************************************************
void KLMESH::SetMaterialShaderName(int materialPosition, std::string name)
{
	this->mMaterialBuffer->at(materialPosition).shaderName = name;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).shaderName = name;
}
void KLMESH::SetMaterialDiffuseMap(int materialPosition, ID3D11ShaderResourceView* diffuseMap, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).diffuseMap = diffuseMap;
	this->mMaterialBuffer->at(materialPosition).diffuseMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).diffuseMapName = textureName;
}
void KLMESH::SetMaterialNormalMap(int materialPosition, ID3D11ShaderResourceView* normalMap, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).normalMap = normalMap;
	this->mMaterialBuffer->at(materialPosition).normalMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).normalMapName = textureName;
}
void KLMESH::SetMaterialASHMap(int materialPosition, ID3D11ShaderResourceView* ashMap, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).ashMap = ashMap;
	this->mMaterialBuffer->at(materialPosition).ashMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).ashMapName = textureName;
}
void KLMESH::SetMaterialCubeMap(int materialPosition, ID3D11ShaderResourceView* cubeMap, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).cubeMap = cubeMap;
	this->mMaterialBuffer->at(materialPosition).cubeMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).cubeMapName = textureName;
}
void KLMESH::SetMaterialAmbientColour(int materialPosition, XMFLOAT4 ambientColour)
{
	this->mMaterialBuffer->at(materialPosition).ambientColour = ambientColour;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.Ambient = ambientColour;
	XMFLOAT3 color = XMFLOAT3(ambientColour.x, ambientColour.y, ambientColour.z);
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).ambientColour = color;
}
void KLMESH::SetMaterialDiffuseColour(int materialPosition, XMFLOAT4 diffuseColour)
{
	this->mMaterialBuffer->at(materialPosition).diffuseColour = diffuseColour;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.Diffuse = diffuseColour;
	XMFLOAT3 color = XMFLOAT3(diffuseColour.x, diffuseColour.y, diffuseColour.z);
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).diffuseColour = color;
}
void KLMESH::SetMaterialSpecularColour(int materialPosition, XMFLOAT4 specularColour)
{
	this->mMaterialBuffer->at(materialPosition).specularColour = specularColour;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.Specular = specularColour;
	XMFLOAT3 color = XMFLOAT3(specularColour.x, specularColour.y, specularColour.z);
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).specularColour = color;
}
void KLMESH::SetMaterialReflectColour(int materialPosition, XMFLOAT4 reflectColour)
{
	this->mMaterialBuffer->at(materialPosition).reflectColour = reflectColour;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.Reflect = reflectColour;
	XMFLOAT3 color = XMFLOAT3(reflectColour.x, reflectColour.y, reflectColour.z);
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).reflectColour = color;
}
void KLMESH::SetMaterialAlphaColour(int materialPosition, float alphaColour)
{
	this->mMaterialBuffer->at(materialPosition).alphaColour = alphaColour;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).alphaColour = alphaColour;
}
void KLMESH::SetMaterialRoughness(int materialPosition, float roughness)
{
	this->mMaterialBuffer->at(materialPosition).roughness = roughness;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.roughness = roughness;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).roughness = roughness;
}
void KLMESH::SetMaterialFresnelFactor(int materialPosition, float fresnelFactor)
{
	this->mMaterialBuffer->at(materialPosition).fresnelFactor = fresnelFactor;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.fresnelFactor = fresnelFactor;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).fresnelFactor = fresnelFactor;
}
void KLMESH::SetMaterialSpecularPower(int materialPosition, float specularPower)
{
	this->mMaterialBuffer->at(materialPosition).specularPower = specularPower;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.specularPower = specularPower;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).specularPower = specularPower;
}
void KLMESH::SetMaterialTransparency(int materialPosition, float transparency)
{
	this->mMaterialBuffer->at(materialPosition).transparency = transparency;
//	this->mMaterialBuffer->at(materialPosition).materialHlsl.transparency = transparency;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).transparency = transparency;
}
void KLMESH::SetMaterialHasTessellation(int materialPosition, bool hasTessellation)
{
	this->mMaterialBuffer->at(materialPosition).hasTessellation = hasTessellation;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).hasTessellation = hasTessellation;
}
void KLMESH::SetMaterialHeightScaleTess(int materialPosition, float heightScaleTess)
{
	this->mMaterialBuffer->at(materialPosition).heightScaleTess = heightScaleTess;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.heightScaleTess = heightScaleTess;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).heightScaleTess = (float)heightScaleTess;
}
void KLMESH::SetMaterialMaxDistanceTess(int materialPosition, short maxDistanceTess)
{
	this->mMaterialBuffer->at(materialPosition).maxDistanceTess = maxDistanceTess;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.maxDistanceTess = (float)maxDistanceTess;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).maxDistanceTess = (float)maxDistanceTess;
}
void KLMESH::SetMaterialMinDistanceTess(int materialPosition, short minDistanceTess)
{
	this->mMaterialBuffer->at(materialPosition).minDistanceTess = minDistanceTess;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.minDistanceTess = (float)minDistanceTess;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).minDistanceTess = (float)minDistanceTess;
}
void KLMESH::SetMaterialMaxFactorTess(int materialPosition, short maxFactorTess)
{
	this->mMaterialBuffer->at(materialPosition).maxFactorTess = maxFactorTess;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.maxFactorTess = (float)maxFactorTess;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).maxFactorTess = (float)maxFactorTess;
}
void KLMESH::SetMaterialMinFactorTess(int materialPosition, short minFactorTess)
{
	this->mMaterialBuffer->at(materialPosition).minFactorTess = minFactorTess;
	this->mMaterialBuffer->at(materialPosition).materialHlsl.minFactorTess = (float)minFactorTess;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).minFactorTess = (float)minFactorTess;
}
void KLMESH::SetMaterialDiffuseMapName(int materialPosition, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).diffuseMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).diffuseMapName = textureName;
}
void KLMESH::SetMaterialNormalMapName(int materialPosition, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).normalMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).normalMapName = textureName;
}
void KLMESH::SetMaterialASHMapName(int materialPosition, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).ashMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).ashMapName = textureName;
}
void KLMESH::SetMaterialCubeMapName(int materialPosition, std::string textureName)
{
	this->mMaterialBuffer->at(materialPosition).cubeMapName = textureName;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).cubeMapName = textureName;
}
void KLMESH::SetMaterialCubeMapDynamic(int materialPosition, bool value)
{
	this->mMaterialBuffer->at(materialPosition).cubeMapDynamic = value;
	this->mKlmeshFileStruct->materialsBuffer.at(materialPosition).cubeMapDynamic = value;
}
//*************************************************************************************************************************************
void KLMESH::SetFileName(std::string fileName)
{
	this->mFileName = fileName;
}
void KLMESH::SetLinkToFile(std::string link)
{
	this->mLinkToFile = link;
	this->mKlmeshFileStruct->name = link;
}
void KLMESH::SetDistanceLOD0(UINT distance)
{
	this->mDistanceLOD0 = distance;
	this->mKlmeshFileStruct->mDistanceLOD0 = distance;
}
void KLMESH::SetDistanceLOD1(UINT distance)
{
	this->mDistanceLOD1 = distance;
	this->mKlmeshFileStruct->mDistanceLOD1 = distance;
}
void KLMESH::SetDistanceLOD2(UINT distance)
{
	this->mDistanceLOD2 = distance;
	this->mKlmeshFileStruct->mDistanceLOD2 = distance;
}
void KLMESH::SetDistanceHide(UINT distance)
{
	this->mDistanceHide = distance;
	this->mKlmeshFileStruct->mDistanceHide = distance;
}
void KLMESH::SetLodAmount(short amount)
{
	this->mLodAmount = amount;
}
void KLMESH::SetShadowDistance(float shadowDistance)
{
	this->mDistanceShadow = shadowDistance;
	this->mKlmeshFileStruct->shadowDistance = (short)shadowDistance;
}
void KLMESH::SetBoundingBoxMax(XMVECTOR boundingBoxMax)
{
	this->mBoundingBoxMax = boundingBoxMax;
	XMFLOAT3 vector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMStoreFloat3(&vector, boundingBoxMax);
	this->mKlmeshFileStruct->boundingBoxMax = vector;
	//this->mAxisAlignedBoxOrigin.Extents = vector;


	XMFLOAT3 temp = XMFLOAT3(0.0f, 0.0f, 0.0f);
	temp.x = (mKlmeshFileStruct->boundingBoxMin.x + mKlmeshFileStruct->boundingBoxMax.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMin.y + mKlmeshFileStruct->boundingBoxMax.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMin.z + mKlmeshFileStruct->boundingBoxMax.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Center = temp;

	temp.x = (mKlmeshFileStruct->boundingBoxMax.x - mKlmeshFileStruct->boundingBoxMin.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMax.y - mKlmeshFileStruct->boundingBoxMin.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMax.z - mKlmeshFileStruct->boundingBoxMin.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Extents = temp;


}
void KLMESH::SetBoundingBoxMin(XMVECTOR boundingBoxMin)
{
	this->mBoundingBoxMin = boundingBoxMin;
	XMFLOAT3 vector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMStoreFloat3(&vector, boundingBoxMin);
	this->mKlmeshFileStruct->boundingBoxMin = vector;
	//this->mAxisAlignedBoxOrigin.Center = vector;

	XMFLOAT3 temp = XMFLOAT3(0.0f, 0.0f, 0.0f);
	temp.x = (mKlmeshFileStruct->boundingBoxMin.x + mKlmeshFileStruct->boundingBoxMax.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMin.y + mKlmeshFileStruct->boundingBoxMax.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMin.z + mKlmeshFileStruct->boundingBoxMax.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Center = temp;

	temp.x = (mKlmeshFileStruct->boundingBoxMax.x - mKlmeshFileStruct->boundingBoxMin.x) * 0.5f;
	temp.y = (mKlmeshFileStruct->boundingBoxMax.y - mKlmeshFileStruct->boundingBoxMin.y) * 0.5f;
	temp.z = (mKlmeshFileStruct->boundingBoxMax.z - mKlmeshFileStruct->boundingBoxMin.z) * 0.5f;
	this->mAxisAlignedBoxOrigin.Extents = temp;
}
//*************************************************************************************************************************************
std::string KLMESH::GetFileName()
{
	return this->mFileName;
}
std::string KLMESH::GetLinkToFile()
{
	return this->mLinkToFile;
}
UINT KLMESH::GetDistanceLOD0()
{
	return mDistanceLOD0;
}
UINT KLMESH::GetDistanceLOD1()
{
	return mDistanceLOD1;
}
UINT KLMESH::GetDistanceLOD2()
{
	return mDistanceLOD2;
}
UINT KLMESH::GetDistanceHide()
{
	return mDistanceHide;
}
short KLMESH::GetLodAmount()
{
	return this->mLodAmount;
}
float KLMESH::GetShadowDistance()
{
	return this->mDistanceShadow;
}
collisionMeshStruct* KLMESH::GetCollMeshData()
{
	return &mKlmeshFileStruct->collisionMeshData;
}
AxisAlignedBox* KLMESH::GetAxisAlignedBoxOrigin()
{
	return &mAxisAlignedBoxOrigin;
}
XMVECTOR KLMESH::GetBoundingBoxMax()
{
	return this->mBoundingBoxMax;
}
XMVECTOR KLMESH::GetBoundingBoxMin()
{
	return this->mBoundingBoxMin;
}

//*************************************************************************************************************************************
ID3D11Buffer** KLMESH::GetVertexBuffer()
{
	return &mVertexBufferObject;
}
ID3D11Buffer* KLMESH::GetIndexBuffer()
{
	return mIndexBufferObject;
}
//Collision
ID3D11Buffer** KLMESH::GetVertexBufferCollsion()
{
	return &mVertexBufferCollision;
}
ID3D11Buffer* KLMESH::GetIndexBufferCollsion()
{
	return mIndexBufferCollision;
}
MeshData KLMESH::GetMeshCollision()
{
	return mMeshCollision;
}
//*************************************************************************************************************************************
std::vector<Material>* KLMESH::GetMaterialBuffer()
{
	return mMaterialBuffer;
}
void KLMESH::SetMaterialBuffer(std::vector<Material>* materialBuffer)
{
	this->mMaterialBuffer = materialBuffer;
}
//*************************************************************************************************************************************
std::vector<Chunk*>* KLMESH::GetMeshToDraw(UINT distance)
{
	//return mObjectsVector_LOD0;
	if (this->mLodAmount < 2) //only LOD0
	{
		if (mDistanceHide > 0)
		{
			if (distance >= mDistanceLOD0 && distance < mDistanceHide)
				return mObjectsVector_LOD0;
			else
				return nullptr;
		}
		else
		{
			if (distance >= mDistanceLOD0)
				return mObjectsVector_LOD0;
			else
				return nullptr;
		}
	}
	else
		if (this->mLodAmount < 3) //LOD0 & LOD1
		{
			if (mDistanceHide > 0)
			{
				if (distance >= mDistanceLOD0 && distance < mDistanceLOD1)
					return mObjectsVector_LOD0;
				if (distance >= mDistanceLOD1 && distance < mDistanceHide)
					return mObjectsVector_LOD1;
				else
					return nullptr;
			}
			else
			{
				if (distance >= mDistanceLOD0 && distance < mDistanceLOD1)
					return mObjectsVector_LOD0;
				if (distance >= mDistanceLOD1)
					return mObjectsVector_LOD1;
			}
		}
		else
		{
			if (this->mLodAmount == 3) // LOD0, LOD1 & LOD2
			{
				if (mDistanceHide > 0)
				{
					if (distance >= mDistanceLOD0 && distance < mDistanceLOD1)
						return mObjectsVector_LOD0;
					else
						if (distance >= mDistanceLOD1 && distance < mDistanceLOD2)
							return mObjectsVector_LOD1;
						else
							if (distance >= mDistanceLOD2 && distance < mDistanceHide)
								return mObjectsVector_LOD2;
							else
								return nullptr;
				}
				else
				{
					if (distance >= mDistanceLOD0 && distance < mDistanceLOD1)
						return mObjectsVector_LOD0;
					else
						if (distance >= mDistanceLOD1 && distance < mDistanceLOD2)
							return mObjectsVector_LOD1;
						else
							if (distance >= mDistanceLOD2)
								return mObjectsVector_LOD2;
				}
			}
		}
}
//*************************************************************************************************************************************
MeshData* KLMESH::GetAllMeshData()
{
	return this->mAllMeshData;
}
//*************************************************************************************************************************************
void KLMESH::ConvertMaterialToMaterialStruct()
{
	for (int i = 0; i < (int)mMaterialBuffer->size(); ++i)
	{
		this->mKlmeshFileStruct->materialsBuffer[i].materialName = mMaterialBuffer->at(i).materialName;
		this->mKlmeshFileStruct->materialsBuffer[i].shaderName = mMaterialBuffer->at(i).shaderName;

		XMFLOAT3 color = XMFLOAT3(mMaterialBuffer->at(i).ambientColour.x, mMaterialBuffer->at(i).ambientColour.y, mMaterialBuffer->at(i).ambientColour.z);
		this->mKlmeshFileStruct->materialsBuffer[i].ambientColour = color;

		color = XMFLOAT3(mMaterialBuffer->at(i).diffuseColour.x, mMaterialBuffer->at(i).diffuseColour.y, mMaterialBuffer->at(i).diffuseColour.z);
		this->mKlmeshFileStruct->materialsBuffer[i].diffuseColour = color;

		color = XMFLOAT3(mMaterialBuffer->at(i).specularColour.x, mMaterialBuffer->at(i).specularColour.y, mMaterialBuffer->at(i).specularColour.z);
		this->mKlmeshFileStruct->materialsBuffer[i].specularColour = color;

		color = XMFLOAT3(mMaterialBuffer->at(i).reflectColour.x, mMaterialBuffer->at(i).reflectColour.y, mMaterialBuffer->at(i).reflectColour.z);
		this->mKlmeshFileStruct->materialsBuffer[i].reflectColour = color;
		
		this->mKlmeshFileStruct->materialsBuffer[i].alphaColour = mMaterialBuffer->at(i).alphaColour;
		this->mKlmeshFileStruct->materialsBuffer[i].diffuseMapName = mMaterialBuffer->at(i).diffuseMapName;
		this->mKlmeshFileStruct->materialsBuffer[i].normalMapName = mMaterialBuffer->at(i).normalMapName;
		this->mKlmeshFileStruct->materialsBuffer[i].ashMapName = mMaterialBuffer->at(i).ashMapName;
		this->mKlmeshFileStruct->materialsBuffer[i].cubeMapName = mMaterialBuffer->at(i).cubeMapName;

		this->mKlmeshFileStruct->materialsBuffer[i].hasTessellation = mMaterialBuffer->at(i).hasTessellation;
		this->mKlmeshFileStruct->materialsBuffer[i].heightScaleTess = mMaterialBuffer->at(i).heightScaleTess;
		this->mKlmeshFileStruct->materialsBuffer[i].maxDistanceTess = mMaterialBuffer->at(i).maxDistanceTess;
		this->mKlmeshFileStruct->materialsBuffer[i].minDistanceTess = mMaterialBuffer->at(i).minDistanceTess;
		this->mKlmeshFileStruct->materialsBuffer[i].maxFactorTess = mMaterialBuffer->at(i).maxFactorTess;
		this->mKlmeshFileStruct->materialsBuffer[i].minFactorTess = mMaterialBuffer->at(i).minFactorTess;

		this->mKlmeshFileStruct->materialsBuffer[i].transparency = mMaterialBuffer->at(i).transparency;

		this->mKlmeshFileStruct->materialsBuffer[i].roughness = mMaterialBuffer->at(i).roughness;
		this->mKlmeshFileStruct->materialsBuffer[i].fresnelFactor = mMaterialBuffer->at(i).fresnelFactor;
		this->mKlmeshFileStruct->materialsBuffer[i].specularPower = mMaterialBuffer->at(i).specularPower;
		this->mKlmeshFileStruct->materialsBuffer[i].cubeMapDynamic = mMaterialBuffer->at(i).cubeMapDynamic;
	}
}

ID3D11Buffer** KLMESH::GetBBoxVertexBuffer()
{
	return &mVertexBufferBBox;
}
MeshData KLMESH::GetBBoxMeshData()
{
	return this->mMeshBBox;
}
bool KLMESH::GetIsAllowRender()
{
	return this->mAllowRender;
}
void KLMESH::SetIsAllowRender(bool signal)
{
	this->mAllowRender = signal;
}