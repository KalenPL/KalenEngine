#ifndef FILE_H
#define FILE_H

#include "Utility.h"
#include "Material.h"
#include "TexturesResource.h"
#include "Mesh.h"
#include "LocalCoordinateSystem.h"
#include <process.h>


//Structure to save 5 values of uint
struct KL_UINT5
{
	//C - tor
	KL_UINT5(UINT index, XMUINT4 face)
	{
		Index = index;
		Face = face;
	}
	KL_UINT5()
	{
	}
	UINT Index;
	XMUINT4 Face;
};




//*********************************************************************
// MATERIAL STRUCTURE
//*********************************************************************
struct materialStruct
{
	// C-tor set default data
	materialStruct()
	{
		this->shaderName = "";
		this->materialName = "";
		XMFLOAT3 empty = XMFLOAT3(1.0, 1.0, 1.0); //default set on white
		this->diffuseColour = empty;
		this->ambientColour = empty;
		this->specularColour = empty;
		this->reflectColour = empty;
		this->alphaColour = 1.0;
		//this->alphaMapName = "NULL";
		this->diffuseMapName = "NULL";
		//this->heightMapName = "NULL";
		this->normalMapName = "NULL";
		this->ashMapName = "NULL";
		this->cubeMapName = "NULL";

		this->hasTessellation = false;
		this->heightScaleTess = 1.5;
		this->maxDistanceTess = 150.0;
		this->minDistanceTess = 300.0;
		this->maxFactorTess = 1.0;
		this->minFactorTess = 8.0;

		this->roughness = 0.5f;
		this->fresnelFactor = 1.0f;
		this->specularPower = 64.0f;
		this->transparency = 1.0f;

		this->cubeMapDynamic = false;
	}
	string shaderName;
	string materialName;

	// Colour information
	XMFLOAT3	ambientColour;
	XMFLOAT3	diffuseColour;
	XMFLOAT3	specularColour;
	XMFLOAT3	reflectColour;
	float		alphaColour;
	float		transparency;

	float roughness;
	float fresnelFactor;
	float specularPower;

	// Textures information:
	string  diffuseMapName;
	string	normalMapName;
	string	ashMapName;
	string	cubeMapName;
	//string	alphaMapName;
	//string	heightMapName;

	//Tessellation
	bool hasTessellation;
	float heightScaleTess;
	short maxDistanceTess;
	short minDistanceTess;
	short maxFactorTess;
	short minFactorTess;
	bool cubeMapDynamic;

};



//*********************************************************************
// COLLISOIN MESH STRUCTURE
//*********************************************************************
struct collisionMeshStruct
{
	// C-tor set default data
	collisionMeshStruct()
	{
		this->verticesUV = XMFLOAT2(0.0, 0.0);
	}
	// Buffer collision data information - important to detect collision mesh!
	std::vector<XMFLOAT3> verticesPositionVector;
	std::vector<XMFLOAT3>  verticesNormalVector;
	XMFLOAT2  verticesUV;     //Only ONE UV Vertex, set on 0.0, 0.0 because this information is not important

							  //Face data - only for load file - This data informs computer how to create triangle!
	std::vector<XMUINT2> faceDataVector; //V, Vn  (not for UV!!!)

};



//*********************************************************************
// CHUNK STRUCTURE
//*********************************************************************
struct chunkStruct
{
	//C-tor set default data
	chunkStruct()
	{
		this->startIndex = 0;
		this->startVertex = 0;
		this->numberOfIndicesToDraw = 0;
		this->materialName = "";
	}
	//Material information - structure
	//materialStruct material;    //_____________________________________________ NOT USE

	std::string materialName;

	// Buffor data information - important to draw mesh!
	unsigned int  startVertex;
	unsigned int  startIndex;
	unsigned int  numberOfIndicesToDraw;

	//Face data - only for load file - This data informs computer how to create triangle!
	//std::vector<XMUINT4> faceDataVector;


	std::vector<KL_UINT5> faceDataVector;
	//Indices 
	//std::vector<short> indicesOfChunkVector;

};

//*********************************************************************
// LOD STRUCTURE
//*********************************************************************
/*struct lodStruct
{
	lodStruct()
	{
		this->chunkAmount = 0;
	}
	short chunkAmount;

	//Vertices information - BUFFER DATA
	std::vector<XMFLOAT3> verticesPositionVector;
	std::vector<XMFLOAT3> verticesNormalVector;
	std::vector<XMFLOAT2> verticesUvVector;
	std::vector<XMFLOAT4> verticesTangentVector;// --------------------------------------------- TODO - Count while reading obj file 

	std::vector<chunkStruct> chunksVector;

};
*/

//*********************************************************************
// BASIC INFORMATION STRUCTURE
//*********************************************************************
struct klmeshFileStruct
{
	//C-tor
	klmeshFileStruct()
	{
		this->name = "NULL";
		this->lodAmount = -1;
		this->boundingBoxMax = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->boundingBoxMin = XMFLOAT3(0.0f, 0.0f, 0.0f);

		this->mDistanceLOD0 = 0;
		this->mDistanceLOD1 = 800; //80m
		this->mDistanceLOD2 = 1000; //100m
		this->mDistanceHide = 0; //No hide

		this->castShadow = false;
		this->shadowDistance = 600; //60m
	}
	string name;
	short lodAmount;
	std::vector<materialStruct> materialsBuffer;

	//Collision Mesh
	collisionMeshStruct collisionMeshData;

	MeshData AllMeshData; //All vertices and all indices (include all lods and all chunk) whole mesh
	std::vector<Chunk> mObjectsVector_LOD0;
	std::vector<Chunk> mObjectsVector_LOD1;
	std::vector<Chunk> mObjectsVector_LOD2;

	UINT mDistanceLOD0;
	UINT mDistanceLOD1;
	UINT mDistanceLOD2;
	UINT mDistanceHide;

	//BoundingBox BBox
	XMFLOAT3 boundingBoxMax;
	XMFLOAT3 boundingBoxMin;

	//Shadow information
	bool castShadow;
	short shadowDistance;
};

//*********************************************************************
// KLL - Layer file structure
//*********************************************************************
struct objectStruct
{
	objectStruct()
	{
		this->name = "";
		this->tag = "";
		this->linkToFile = "";
		this->mLocalCoordinatePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->mLocalAxisX = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->mLocalAxisY = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->mLocalAxisZ = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->mScaleVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->mPitch = 0.0f;
		this->mYaw = 0.0f;
		this->mRoll = 0.0f;
		this->mIsCastShadow = false;
		this->mShadowDistance = 0.0f;
		this->mIsVisible = true;
		this->mType;
		//Light
		this->Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		this->Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		this->Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		this->Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->Range = 0.0f;
		this->Att = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	std::string name;
	std::string tag;
	std::string linkToFile;
	XMFLOAT3 mLocalCoordinatePosition;
	XMFLOAT3 mLocalAxisX;
	XMFLOAT3 mLocalAxisY;
	XMFLOAT3 mLocalAxisZ;
	XMFLOAT3 mScaleVector;
	float mPitch;
	float mYaw;
	float mRoll;
	bool mIsCastShadow;
	float mShadowDistance;
	bool mIsVisible;
	int mType;
	//Light
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	float Range;
	XMFLOAT3 Att;
};

struct kllFileStruct
{
	kllFileStruct()
	{
	}
	std::vector<objectStruct> objectBuffer;
};

struct klwFileStruct
{
	klwFileStruct()
	{
		mName = "";
	}
	std::string mName;
};






















//*******************************************************************************************************

class File
{
public:
	File();
	File(ID3D11Device* pDevice);
	~File();
	void SetDivice(ID3D11Device* pDevice);
	//Object LoadObjectFile(std::string fileName, XMFLOAT3 &centerBoxSelect, XMFLOAT3 &extentsBoxSelect);
	//bool LoadSceneFile(std::string fileName, std::vector<Object> &mObjectsVector);
	//bool SaveSceneFile(std::string fileName, std::vector<Object> &mObjectsVector);



private:
	ID3D11Device* m_pDevice;
	TexturesResource mTexturesResource;


};
#endif //FILE_H

