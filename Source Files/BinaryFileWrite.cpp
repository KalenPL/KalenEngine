#include "../Header Files/BinaryFileWrite.h"

//**********************************************
// C - tor
//**********************************************
BinaryFileWrite::BinaryFileWrite(std::string fileName)
{
	ofstream outputFile(fileName, ios::binary);
	this->mOutputFile = std::move(outputFile); //Write to file
}

//**********************************************
// D - tor
//**********************************************
BinaryFileWrite::~BinaryFileWrite()
{
	if (mOutputFile.is_open())
		mOutputFile.close();
}

void BinaryFileWrite::closeFile()
{
	mOutputFile.close();
}

//*****************************************************************************************************************************************************
// Function to write material to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeMaterialStructure(materialStruct &materialStruct)
{
	size_t stringSize; //Size of string;
					   //Name
	stringSize = materialStruct.materialName.size(); //Firts of all I write length of string to binary file(important to read string from binary file)
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&materialStruct.materialName[0], stringSize); //Write nameFile to file as std::string

																	//ShaderName
	stringSize = materialStruct.shaderName.size(); //Firts of all I write length of string to binary file(important to read string from binary file)
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&materialStruct.shaderName[0], stringSize); //Write nameFile to file as std::string

																  //Colour information
	mOutputFile.write((char*)&materialStruct.ambientColour, sizeof(XMFLOAT3)); //Write ambientColour to file as XMFLOAT3
	mOutputFile.write((char*)&materialStruct.diffuseColour, sizeof(XMFLOAT3)); //Write diffuseColour to file as XMFLOAT3

	mOutputFile.write((char*)&materialStruct.specularColour, sizeof(XMFLOAT3)); //Write specularColour to file as XMFLOAT3
	mOutputFile.write((char*)&materialStruct.reflectColour, sizeof(XMFLOAT3)); //Write reflectColour to file as XMFLOAT3
	mOutputFile.write((char*)&materialStruct.alphaColour, sizeof(float)); //Write alphaColour to file as float

																		  // Textures information:
	stringSize = materialStruct.diffuseMapName.size(); //Length of diffuseMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of diffuseMapName to binary file
	mOutputFile.write(&materialStruct.diffuseMapName[0], stringSize); //Write diffuseMapName to file as std::string

	stringSize = materialStruct.normalMapName.size(); //Length of normalMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of normalMapName to binary file
	mOutputFile.write(&materialStruct.normalMapName[0], stringSize); //Write normalMapName to file as std::string

	stringSize = materialStruct.ashMapName.size(); //Length of specularMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of specularMapName to binary file
	mOutputFile.write(&materialStruct.ashMapName[0], stringSize); //Write specularMapName to file as std::string

	stringSize = materialStruct.cubeMapName.size(); //Length of cubeMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of cubeMapName to binary file
	mOutputFile.write(&materialStruct.cubeMapName[0], stringSize); //Write cubeMapName to file as std::string

	/*	stringSize = materialStruct.alphaMapName.size(); //Length of alphaMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of alphaMapName to binary file
	mOutputFile.write(&materialStruct.alphaMapName[0], stringSize); //Write alphaMapName to file as std::string

	stringSize = materialStruct.heightMapName.size(); //Length of heightMapName (string)
	mOutputFile.write((char*)&stringSize, sizeof(size_t)); //Write lenght of heightMapName to binary file
	mOutputFile.write(&materialStruct.heightMapName[0], stringSize); //Write heightMapName to file as std::string*/

	mOutputFile.write((char*)&materialStruct.hasTessellation, sizeof(bool)); //Write 'hasTessellation' to file as "bool"
	mOutputFile.write((char*)&materialStruct.heightScaleTess, sizeof(float)); //Write 'heightScaleTess' to file as "float"
	mOutputFile.write((char*)&materialStruct.maxDistanceTess, sizeof(short)); //Write 'maxDistanceTess' to file as "short"
	mOutputFile.write((char*)&materialStruct.minDistanceTess, sizeof(short)); //Write 'minDistanceTess' to file as "short"
	mOutputFile.write((char*)&materialStruct.maxFactorTess, sizeof(short)); //Write 'maxFactorTess' to file as "short"
	mOutputFile.write((char*)&materialStruct.minFactorTess, sizeof(short)); //Write 'minFactorTess' to file as "short"

	mOutputFile.write((char*)&materialStruct.roughness, sizeof(float)); //Write roughness to file as float
	mOutputFile.write((char*)&materialStruct.fresnelFactor, sizeof(float)); //Write fresnelFactor to file as float
	mOutputFile.write((char*)&materialStruct.specularPower, sizeof(float)); //Write specularPower to file as float
	mOutputFile.write((char*)&materialStruct.transparency, sizeof(float)); //Write transparency to file as float

	mOutputFile.write((char*)&materialStruct.cubeMapDynamic, sizeof(bool)); //Write 'cubeMapDynamic' to file as "bool"
	
}

//*****************************************************************************************************************************************************
// Function to write collision mesh to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeCollisionMeshStructure(collisionMeshStruct &collisionMeshStruct)
{

	//Local values
	vector<XMFLOAT3>::size_type sizeXMFLOAT3 = 0;
	vector<XMUINT2>::size_type sizeXMUINT2 = 0;

	//Write 'verticesPositionVector' to binary file
	sizeXMFLOAT3 = collisionMeshStruct.verticesPositionVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //Size of verticesPositionVector vector data
	mOutputFile.write((char*)&collisionMeshStruct.verticesPositionVector[0], collisionMeshStruct.verticesPositionVector.size() * sizeof(XMFLOAT3));

	//Write 'verticesNormalVector' to binary file
	sizeXMFLOAT3 = collisionMeshStruct.verticesNormalVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //Size of verticesPositionVector vector data
	mOutputFile.write((char*)&collisionMeshStruct.verticesNormalVector[0], collisionMeshStruct.verticesNormalVector.size() * sizeof(XMFLOAT3));

	//Write 'verticesUV' to binary file
	mOutputFile.write((char*)&collisionMeshStruct.verticesUV, sizeof(XMFLOAT2));

	//Write 'faceDataVector' to binary file
	sizeXMUINT2 = collisionMeshStruct.faceDataVector.size();
	mOutputFile.write((char*)&sizeXMUINT2, sizeof(sizeXMUINT2)); //Size of verticesPositionVector vector data
	mOutputFile.write((char*)&collisionMeshStruct.faceDataVector[0], collisionMeshStruct.faceDataVector.size() * sizeof(XMUINT2));

}
//*****************************************************************************************************************************************************
// Function to write chunk to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeChunkStructure(chunkStruct &chunkStruct)
{
	//Local values
	vector<KL_UINT5>::size_type sizeKL_UINT5 = 0;
	size_t stringSize; //Size of string;

	stringSize = chunkStruct.materialName.size(); //Firts of all I write length of string to binary file(important to read string from binary file)
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&chunkStruct.materialName[0], stringSize); //Write nameFile to file as std::string

	mOutputFile.write((char*)&chunkStruct.startVertex, sizeof(unsigned int)); //Write 'startVertex' to binary file as "unsigned int"
	mOutputFile.write((char*)&chunkStruct.startIndex, sizeof(unsigned int)); //Write 'startIndex' to binary file as "unsigned int"
	mOutputFile.write((char*)&chunkStruct.numberOfIndicesToDraw, sizeof(unsigned int)); //Write 'numberOfIndicesToDraw' to binary file as "unsigned int"

																						//Write 'faceDataVector' to binary file
	sizeKL_UINT5 = chunkStruct.faceDataVector.size();
	mOutputFile.write((char*)&sizeKL_UINT5, sizeof(sizeKL_UINT5)); //Size of faceDataVector vector data as XMUINT3
	mOutputFile.write((char*)& chunkStruct.faceDataVector[0], chunkStruct.faceDataVector.size() * sizeof(KL_UINT5));
}

//*****************************************************************************************************************************************************
// Function to write chunk to binary file (klmesh)
//*****************************************************************************************************************************************************
/*void BinaryFileWrite::writeLODStructure(lodStruct &lodStruct)
{
	//Local values
	vector<XMFLOAT4>::size_type sizeXMFLOAT4 = 0;
	vector<XMFLOAT3>::size_type sizeXMFLOAT3 = 0;
	vector<XMFLOAT2>::size_type sizeXMFLOAT2 = 0;
	vector<chunkStruct>::size_type sizeChunkStruct = 0;
	int sizeChunksVector = lodStruct.chunksVector.size();


	//Write "chunkAmount" to binary file
	mOutputFile.write((char*)&lodStruct.chunkAmount, sizeof(short)); //Write chunkAmount to file as "short"

																	 //Write 'verticesPositionVector' to binary file as "vector XMFLOAT3"
	sizeXMFLOAT3 = lodStruct.verticesPositionVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //Size of "verticesPositionVector" vector data
	mOutputFile.write((char*)&lodStruct.verticesPositionVector[0], lodStruct.verticesPositionVector.size() * sizeof(XMFLOAT3));

	//Write 'verticesNormalVector' to binary file as "vector XMFLOAT3"
	sizeXMFLOAT3 = lodStruct.verticesNormalVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //Size of "verticesPositionVector" vector data
	mOutputFile.write((char*)&lodStruct.verticesNormalVector[0], lodStruct.verticesNormalVector.size() * sizeof(XMFLOAT3));

	//Write 'verticesUvVector' to binary file as "vector XMFLOAT3"
	sizeXMFLOAT2 = lodStruct.verticesUvVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT2, sizeof(sizeXMFLOAT2)); //Size of "verticesPositionVector" vector data
	mOutputFile.write((char*)&lodStruct.verticesUvVector[0], lodStruct.verticesUvVector.size() * sizeof(XMFLOAT2));

	//Write 'verticesTangentVector' to binary file as "vector XMFLOAT4"
	sizeXMFLOAT4 = lodStruct.verticesTangentVector.size();
	mOutputFile.write((char*)&sizeXMFLOAT4, sizeof(sizeXMFLOAT4)); //Size of "verticesTangentVector" vector data
	mOutputFile.write((char*)&lodStruct.verticesTangentVector[0], lodStruct.verticesTangentVector.size() * sizeof(XMFLOAT4));

	//Write 'verticesUvVector' to binary file as "vector XMFLOAT3"
	//sizeXMFLOAT3 = lodStruct.verticesTangentVector.size();
	//mOutputFile.write((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //Size of "verticesPositionVector" vector data
	//mOutputFile.write((char*)&lodStruct.verticesTangentVector[0], lodStruct.verticesTangentVector.size() * sizeof(XMFLOAT3));


	//Write "sizeChunksVector" to binary file
	sizeChunksVector = lodStruct.chunksVector.size();
	mOutputFile.write((char*)&sizeChunksVector, sizeof(int)); //Write sizeChunksVector to file as "int" (important to read from binary file)

															  //Loop "for" to write vector of "chunk" data
	for (int i = 0; i < sizeChunksVector; ++i)
	{
		//Write 'chunksVector' to binary file as "vector chunkStruct" - Realise function to write chunk to binary file
		writeChunkStructure(lodStruct.chunksVector[i]);
	}



}
*/

//*****************************************************************************************************************************************************
// Function to write klmeshFileStruct to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeKLMeshFileStructure(klmeshFileStruct &klmeshFileStruct)
{
	size_t stringSize = 0; //Size of string;
	int sizeVector = 0;

	//Name
	stringSize = klmeshFileStruct.name.size(); //Firts of all I write length of string to binary file(important to read string from binary file)
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&klmeshFileStruct.name[0], stringSize); //Write name to file as std::string

															  //Write "lodAmount" to binary file
	mOutputFile.write((char*)&klmeshFileStruct.lodAmount, sizeof(short)); //Write lodAmount to file as "short"


	//**********************************
	//Write vector of materials to binary file
	//**********************************
	sizeVector = klmeshFileStruct.materialsBuffer.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of materialsBuffer vector
	for (int i = 0; i < sizeVector; ++i)
	{
		writeMaterialStructure(klmeshFileStruct.materialsBuffer[i]);
	}

	//**********************************
	//Write 'collisionMeshStruct' to binary file
	//**********************************
	writeCollisionMeshStructure(klmeshFileStruct.collisionMeshData);


	//**********************************
	//Write 'AllMeshData' to binary file
	//**********************************
	writeAllMeshData(klmeshFileStruct.AllMeshData);


	//**********************************
	//Write 'Chunks' to binary file (mObjectsVector_LOD0)
	//**********************************
	sizeVector = klmeshFileStruct.mObjectsVector_LOD0.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of mObjectsVector_LOD0 vector
	for (int i = 0; i < sizeVector; ++i)
	{
		writeChunk(klmeshFileStruct.mObjectsVector_LOD0[i]);
	}

	//**********************************
	//Write 'Chunks' to binary file (mObjectsVector_LOD1)
	//**********************************
	sizeVector = klmeshFileStruct.mObjectsVector_LOD1.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of mObjectsVector_LOD0 vector
	for (int i = 0; i < sizeVector; ++i)
	{
		writeChunk(klmeshFileStruct.mObjectsVector_LOD1[i]);
	}

	//**********************************
	//Write 'Chunks' to binary file (mObjectsVector_LOD2)
	//**********************************
	sizeVector = klmeshFileStruct.mObjectsVector_LOD2.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of mObjectsVector_LOD0 vector
	for (int i = 0; i < sizeVector; ++i)
	{
		writeChunk(klmeshFileStruct.mObjectsVector_LOD2[i]);
	}


	//**********************************
	//Write other data to binary file
	//**********************************
	mOutputFile.write((char*)&klmeshFileStruct.mDistanceLOD0, sizeof(UINT)); //Write 'mDistanceLOD0' to file as "UINT"
	mOutputFile.write((char*)&klmeshFileStruct.mDistanceLOD1, sizeof(UINT)); //Write 'mDistanceLOD1' to file as "UINT"
	mOutputFile.write((char*)&klmeshFileStruct.mDistanceLOD2, sizeof(UINT)); //Write 'mDistanceLOD2' to file as "UINT"
	mOutputFile.write((char*)&klmeshFileStruct.mDistanceHide, sizeof(UINT)); //Write 'mDistanceLOD3' to file as "UINT"



	mOutputFile.write((char*)&klmeshFileStruct.boundingBoxMax, sizeof(XMFLOAT3)); //Write 'boundingBoxMax' to file as "XMFLOAT3"
	mOutputFile.write((char*)&klmeshFileStruct.boundingBoxMin, sizeof(XMFLOAT3)); //Write 'boundingBoxMin' to file as "XMFLOAT3"
														
	//Shadow information
	mOutputFile.write((char*)&klmeshFileStruct.castShadow, sizeof(bool)); //Write 'castShadow' to file as "bool"
	mOutputFile.write((char*)&klmeshFileStruct.shadowDistance, sizeof(short)); //Write 'shadowDistance' to file as "short"
}


//*****************************************************************************************************************************************************
// Function to write vertex to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeVertex(Vertex &vertex)
{
	mOutputFile.write((char*)&vertex.Pos, sizeof(XMFLOAT3)); //Write 'position' of vertex to file as "XMFLOAT3"
	mOutputFile.write((char*)&vertex.Normal, sizeof(XMFLOAT3)); //Write 'Normal' of vertex to file as "XMFLOAT3"
	mOutputFile.write((char*)&vertex.TexC, sizeof(XMFLOAT2)); //Write 'TexC' of vertex to file as "XMFLOAT2"
	mOutputFile.write((char*)&vertex.Tangent, sizeof(XMFLOAT4)); //Write 'TangentU' of vertex to file as "XMFLOAT4"
}

//*****************************************************************************************************************************************************
// Function to write AllMeshData to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeAllMeshData(MeshData &allMeshData)
{
	//**********************************
	//Write vector of vetrices to binary file
	//**********************************
	int sizeVector = 0;
	sizeVector = allMeshData.Vertices.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of allMeshData vector as int
	for (int i = 0; i < sizeVector; ++i)
	{
		writeVertex(allMeshData.Vertices[i]);
	}

	//**********************************
	//Write 'indices' to binary file as "vector WORD"
	//**********************************
	sizeVector = allMeshData.Indices.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Size of "verticesPositionVector" vector data
	mOutputFile.write((char*)&allMeshData.Indices[0], allMeshData.Indices.size() * sizeof(WORD));
}

//*****************************************************************************************************************************************************
// Function to write chunk to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeChunk(Chunk &chunk)
{
	mOutputFile.write((char*)&chunk.materialPosition, sizeof(short)); //Write 'materialPosition' of chunk to file as "short"
	mOutputFile.write((char*)&chunk.numberOfIndicesToDraw, sizeof(UINT)); //Write 'numberOfIndicesToDraw' of chunk to file as "UINT"
	mOutputFile.write((char*)&chunk.startIndex, sizeof(UINT)); //Write 'startIndex' of chunk to file as "UINT"
	mOutputFile.write((char*)&chunk.startVertex, sizeof(UINT)); //Write 'startVertex' of chunk to file as "UINT"
}

//*****************************************************************************************************************************************************
// Function to write object to binary file (KLL)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeObject(objectStruct &object)
{
	size_t stringSize = 0; //Size of string;
	int sizeVector = 0;

	//Name
	stringSize = object.name.size(); //Firts of all I write length of string to binary file(important to read string from binary file)
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&object.name[0], stringSize); //Write name to file as std::string

	//TAG
	stringSize = 0;
	sizeVector = 0;
	stringSize = object.tag.size();
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&object.tag[0], stringSize);

	//Link to file
	stringSize = 0;
	sizeVector = 0;
	stringSize = object.linkToFile.size();
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&object.linkToFile[0], stringSize);

	mOutputFile.write((char*)&object.mLocalCoordinatePosition, sizeof(XMFLOAT3)); //Write mLocalCoordinatePosition to file as "XMFLOAT3"
	mOutputFile.write((char*)&object.mLocalAxisX, sizeof(XMFLOAT3)); //Write mLocalAxisX to file as "XMFLOAT3"
	mOutputFile.write((char*)&object.mLocalAxisY, sizeof(XMFLOAT3)); //Write mLocalAxisY to file as "XMFLOAT3"
	mOutputFile.write((char*)&object.mLocalAxisZ, sizeof(XMFLOAT3)); //Write mLocalAxisZ to file as "XMFLOAT3"
	mOutputFile.write((char*)&object.mScaleVector, sizeof(XMFLOAT3)); //Write mScaleVector to file as "XMFLOAT3"

	mOutputFile.write((char*)&object.mPitch, sizeof(float)); //Write mPitch to file as "float"
	mOutputFile.write((char*)&object.mYaw, sizeof(float)); //Write mYaw to file as "float"
	mOutputFile.write((char*)&object.mRoll, sizeof(float)); //Write mRoll to file as "float"

	mOutputFile.write((char*)&object.mIsCastShadow, sizeof(bool)); //Write mIsCastShadow to file as "bool"
	mOutputFile.write((char*)&object.mShadowDistance, sizeof(float)); //Write mShadowDistance to file as "float"

	mOutputFile.write((char*)&object.mIsVisible, sizeof(bool)); //Write mIsVisible to file as "bool"

	mOutputFile.write((char*)&object.mType, sizeof(int)); //Write mType to file as "int"
	mOutputFile.write((char*)&object.Ambient, sizeof(XMFLOAT4)); //Write Ambient to file as "XMFLOAT4"
	mOutputFile.write((char*)&object.Diffuse, sizeof(XMFLOAT4)); //Write Diffuse to file as "XMFLOAT4"
	mOutputFile.write((char*)&object.Specular, sizeof(XMFLOAT4)); //Write Specular to file as "XMFLOAT4"
	mOutputFile.write((char*)&object.Position, sizeof(XMFLOAT3)); //Write Position to file as "XMFLOAT3"
	mOutputFile.write((char*)&object.Range, sizeof(float)); //Write Range to file as "float"
	mOutputFile.write((char*)&object.Att, sizeof(XMFLOAT3)); //Write Att to file as "XMFLOAT3"
}

//*****************************************************************************************************************************************************
// Function to write kllFileStruct to binary file (KLL)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeKLLFileStructure(kllFileStruct &kllFileStruct)
{
	int sizeVector = 0;
	sizeVector = kllFileStruct.objectBuffer.size();
	mOutputFile.write((char*)&sizeVector, sizeof(int)); //Write size of objectBuffer
	for (int i = 0; i < sizeVector; ++i)
	{
		writeObject(kllFileStruct.objectBuffer[i]);
	}
}

//*****************************************************************************************************************************************************
// Function to write klwFileStruct to binary file (KLW)
//*****************************************************************************************************************************************************
void BinaryFileWrite::writeKLWFileStructure(klwFileStruct &klwFileStruct)
{
	size_t stringSize = 0; //Size of string;
	int sizeVector = 0;

	//Name
	stringSize = klwFileStruct.mName.size();
	mOutputFile.write((char*)&stringSize, sizeof(size_t));
	mOutputFile.write(&klwFileStruct.mName[0], stringSize); //Write name to file as std::string
}