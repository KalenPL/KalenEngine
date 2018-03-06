#include "../Header Files/BinaryFileRead.h"

//**********************************************
// C - tor
//**********************************************
BinaryFileRead::BinaryFileRead(std::string fileName)
{
	ifstream inputFile(fileName, ios::binary);
	this->mInputFile = std::move(inputFile); //Read from file

	if (!mInputFile.is_open())
	{
		wxMessageBox("Cannot open klmesh file", "Error", wxICON_ERROR | wxOK);
	}
	mName = fileName;
}

//**********************************************
// D - tor
//**********************************************
BinaryFileRead::~BinaryFileRead()
{
	if (mInputFile.is_open())
		mInputFile.close();
}

//**********************************************
// Close file
//**********************************************
void BinaryFileRead::closeFile()
{
	mInputFile.close();
}


//*****************************************************************************************************************************************************
// Function to read material from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readMaterialStructure(materialStruct &materialStruct)
{

	//Temporary values
	std::string::size_type stringSize = 0;
	std::string materialName = "";
	std::string shaderName = "";
	XMFLOAT3 ambientColour = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 diffuseColour = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 specularColour = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 reflectColour = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float alphaColour = 0.0f;
	std::string diffuseMapName = "";
	std::string normalMapName = "";
	std::string specularMapName = "";
	std::string alphaMapName = "";
	std::string heightMapName = "";
	std::string cubeMapName = "";

	bool hasTessellation = false;
	float heightScaleTess = 0.0f;
	short maxDistanceTess = 0;
	short minDistanceTess = 0;
	short maxFactorTess = 0;
	short minFactorTess = 0;
	float roughness = 0.0f;
	float fresnelFactor = 0.0f;
	float specularPower = 0.0f;
	float transparency = 1.0f;

	bool cubeMapDynamic = false;



	//Read name of material
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	materialName.resize(stringSize);
	mInputFile.read(&materialName[0], stringSize);

	//Read name of shader
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	shaderName.resize(stringSize);
	mInputFile.read(&shaderName[0], stringSize);

	//Colour information
	mInputFile.read((char*)&ambientColour, sizeof(XMFLOAT3)); //Read "ambientColour" from file as XMFLOAT3
	mInputFile.read((char*)&diffuseColour, sizeof(XMFLOAT3)); //Read "diffuseColour" from file as XMFLOAT3
	mInputFile.read((char*)&specularColour, sizeof(XMFLOAT3)); //Read "specularColour" from file as XMFLOAT3
	mInputFile.read((char*)&reflectColour, sizeof(XMFLOAT3)); //Read "reflectColour" from file as XMFLOAT3
	mInputFile.read((char*)&alphaColour, sizeof(float)); //Read "alphaColour" from file as float

	// Textures information:

	//Read diffuseMapName from file as std::string
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	diffuseMapName.resize(stringSize);
	mInputFile.read(&diffuseMapName[0], stringSize);

	//Read normalMapName from file as std::string
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	normalMapName.resize(stringSize);
	mInputFile.read(&normalMapName[0], stringSize);

	//Read specularMapName from file as std::string
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	specularMapName.resize(stringSize);
	mInputFile.read(&specularMapName[0], stringSize);

	//Read cubeMapName from file as std::string
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	cubeMapName.resize(stringSize);
	mInputFile.read(&cubeMapName[0], stringSize);

	//Read alphaMapName from file as std::string
	/*mInputFile.read((char*)&stringSize, sizeof(stringSize));
	alphaMapName.resize(stringSize);
	mInputFile.read(&alphaMapName[0], stringSize);

	//Read heightMapName from file as std::string
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	heightMapName.resize(stringSize);
	mInputFile.read(&heightMapName[0], stringSize);*/


	//Other values
	mInputFile.read((char*)&hasTessellation, sizeof(bool));
	mInputFile.read((char*)&heightScaleTess, sizeof(float));
	mInputFile.read((char*)&maxDistanceTess, sizeof(short));
	mInputFile.read((char*)&minDistanceTess, sizeof(short));
	mInputFile.read((char*)&maxFactorTess, sizeof(short));
	mInputFile.read((char*)&minFactorTess, sizeof(short));

	mInputFile.read((char*)&roughness, sizeof(float));
	mInputFile.read((char*)&fresnelFactor, sizeof(float));
	mInputFile.read((char*)&specularPower, sizeof(float));
	mInputFile.read((char*)&transparency, sizeof(float));

	mInputFile.read((char*)&cubeMapDynamic, sizeof(bool));

	//Update data
	materialStruct.materialName = materialName;
	materialStruct.shaderName = shaderName;
	materialStruct.ambientColour = ambientColour;
	materialStruct.diffuseColour = diffuseColour;
	materialStruct.specularColour = specularColour;
	materialStruct.reflectColour = reflectColour;
	materialStruct.alphaColour = alphaColour;
	materialStruct.diffuseMapName = diffuseMapName;
	materialStruct.normalMapName = normalMapName;
	materialStruct.ashMapName = specularMapName;
	materialStruct.cubeMapName = cubeMapName;
	//	materialStruct.alphaMapName = alphaMapName;
	//	materialStruct.heightMapName = heightMapName;
	materialStruct.hasTessellation = hasTessellation;
	materialStruct.heightScaleTess = heightScaleTess;
	materialStruct.maxDistanceTess = maxDistanceTess;
	materialStruct.minDistanceTess = minDistanceTess;
	materialStruct.maxFactorTess = maxFactorTess;
	materialStruct.minFactorTess = minFactorTess;
	materialStruct.roughness = roughness;
	materialStruct.fresnelFactor = fresnelFactor;
	materialStruct.specularPower = specularPower;
	materialStruct.transparency = transparency;

	materialStruct.cubeMapDynamic = cubeMapDynamic;




}
//*****************************************************************************************************************************************************
// Function to read collision mesh from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readCollisionMeshStructure(collisionMeshStruct &collisionMeshStruct)
{
	vector<XMFLOAT3>::size_type sizeVectorXMFLOAT3 = 0;
	vector<XMUINT2>::size_type sizeVectorXMUINT2 = 0;
	std::vector<XMFLOAT3> verticesPositionVector;
	std::vector<XMFLOAT3> verticesNormalVector;
	std::vector<XMUINT2> faceDataVector;
	XMFLOAT2 verticesUV;

	//Read 'verticesPositionVector' from binary file as vector XMFLOAT3
	mInputFile.read((char*)&sizeVectorXMFLOAT3, sizeof(sizeVectorXMFLOAT3)); //read size of vector
	verticesPositionVector.resize(sizeVectorXMFLOAT3);//resize vector (set size)
	mInputFile.read((char*)&verticesPositionVector[0], verticesPositionVector.size() * sizeof(XMFLOAT3)); //Read 'verticesPositionVector'

																										  //Read 'verticesNormalVector' from binary file as vector XMFLOAT3
	mInputFile.read((char*)&sizeVectorXMFLOAT3, sizeof(sizeVectorXMFLOAT3)); //read size of vector
	verticesNormalVector.resize(sizeVectorXMFLOAT3);//resize vector (set size)
	mInputFile.read((char*)&verticesNormalVector[0], verticesNormalVector.size() * sizeof(XMFLOAT3));//Read 'verticesNormalVector'

																									 //Read 'verticesUV' from binary file as value XMFLOAT2
	mInputFile.read((char*)&verticesUV, sizeof(XMFLOAT2)); //Read verticesUV as XMFLOAT2 (only one value)

														   //Read 'faceDataVector' from binary file as vector XMUINT2
	mInputFile.read((char*)&sizeVectorXMUINT2, sizeof(sizeVectorXMUINT2)); //read size of vector
	faceDataVector.resize(sizeVectorXMUINT2);//resize vector (set size)
	mInputFile.read((char*)&faceDataVector[0], faceDataVector.size() * sizeof(XMUINT2));//Read 'faceDataVector'

																						//Update data
	collisionMeshStruct.verticesPositionVector = verticesPositionVector;
	collisionMeshStruct.verticesNormalVector = verticesNormalVector;
	collisionMeshStruct.verticesUV = verticesUV;
	collisionMeshStruct.faceDataVector = faceDataVector;
}
//*****************************************************************************************************************************************************
// Function to read chunk from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readChunkStructure(chunkStruct &chunkStruct)
{

	//Local values
	vector<KL_UINT5>::size_type sizeKL_UINT5 = 0;
	std::string::size_type stringSize = 0; //Size of string;
	std::string materialName = "";
	unsigned int startVertex = 0;
	unsigned int startIndex = 0;
	unsigned int numberOfIndicesToDraw = 0;
	std::vector<KL_UINT5> faceDataVector;

	//Read name of material
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	materialName.resize(stringSize);
	mInputFile.read(&materialName[0], stringSize);

	//Read three basic value
	mInputFile.read((char*)&startVertex, sizeof(unsigned int)); //Read "startVertex" from file as "unsigned int"
	mInputFile.read((char*)&startIndex, sizeof(unsigned int)); //Read "startIndex" from file as "unsigned int"
	mInputFile.read((char*)&numberOfIndicesToDraw, sizeof(unsigned int)); //Read "numberOfIndicesToDraw" from file as "unsigned int"

																		  //Read 'faceDataVector' from binary file as vector XMUINT3
	mInputFile.read((char*)&sizeKL_UINT5, sizeof(sizeKL_UINT5)); //read size of vector
	faceDataVector.resize(sizeKL_UINT5);//resize vector (set size)
	mInputFile.read((char*)&faceDataVector[0], faceDataVector.size() * sizeof(KL_UINT5));//Read 'faceDataVector' from binary file as XMUINT4

																						 //Update data
	chunkStruct.materialName = materialName;
	chunkStruct.startVertex = startVertex;
	chunkStruct.startIndex = startIndex;
	chunkStruct.numberOfIndicesToDraw = numberOfIndicesToDraw;
	chunkStruct.faceDataVector = faceDataVector;
}



//*****************************************************************************************************************************************************
// Function to read LOD from binary file (klmesh)
//*****************************************************************************************************************************************************
/*void BinaryFileRead::readLODStructure(lodStruct &lodStruct)
{

	//Local values
	vector<XMFLOAT4>::size_type sizeXMFLOAT4 = 0;
	vector<XMFLOAT3>::size_type sizeXMFLOAT3 = 0;
	vector<XMFLOAT2>::size_type sizeXMFLOAT2 = 0;
	vector<chunkStruct>::size_type sizeChunkStruct = 0;
	short chunkAmount = 0;
	std::vector<XMFLOAT3> verticesPositionVector;
	std::vector<XMFLOAT3> verticesNormalVector;
	std::vector<XMFLOAT2> verticesUvVector;
	std::vector<XMFLOAT4> verticesTangentVector;
	std::vector<chunkStruct> chunksVector;
	int sizeChunksVector = 0;

	//Read first value
	mInputFile.read((char*)&chunkAmount, sizeof(short)); //Read "chunkAmount" from file as "short"

														 //Read 'verticesPositionVector' from binary file as vector XMFLOAT3
	mInputFile.read((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //read size of vector
	verticesPositionVector.resize(sizeXMFLOAT3);//resize vector (set size)
	mInputFile.read((char*)&verticesPositionVector[0], verticesPositionVector.size() * sizeof(XMFLOAT3)); //Read 'verticesPositionVector'

																										  //Read 'verticesNormalVector' from binary file as vector XMFLOAT3
	mInputFile.read((char*)&sizeXMFLOAT3, sizeof(sizeXMFLOAT3)); //read size of vector
	verticesNormalVector.resize(sizeXMFLOAT3);//resize vector (set size)
	mInputFile.read((char*)&verticesNormalVector[0], verticesNormalVector.size() * sizeof(XMFLOAT3)); //Read 'verticesNormalVector'

																									  //Read 'verticesUvVector' from binary file as vector XMFLOAT3
	mInputFile.read((char*)&sizeXMFLOAT2, sizeof(sizeXMFLOAT2)); //read size of vector
	verticesUvVector.resize(sizeXMFLOAT2);//resize vector (set size)
	mInputFile.read((char*)&verticesUvVector[0], verticesUvVector.size() * sizeof(XMFLOAT2)); //Read 'verticesUvVector'


																							  //Read 'verticesTangentVector' from binary file as vector XMFLOAT4
	mInputFile.read((char*)&sizeXMFLOAT4, sizeof(sizeXMFLOAT4)); //read size of vector
	verticesTangentVector.resize(sizeXMFLOAT4);//resize vector (set size)
	mInputFile.read((char*)&verticesTangentVector[0], verticesTangentVector.size() * sizeof(XMFLOAT4)); //Read 'verticesTangentVector'

	//-----------------------------------------------------------------------------
	//In this case I write 'chunksVector' to binary file. 
	//First of all I must write size of 'chunksVector' vector to binary file.
	//Next I must use "for" loop and in this loop realise function to write chunkt to binary file.
	//-----------------------------------------------------------------------------

																										//Read size of vector 'chunksVector' to binary file (important to read all elements of vector from binary file)
	mInputFile.read((char*)&sizeChunksVector, sizeof(int)); //Write "chunksVector" from file as "short"

	chunksVector.resize(sizeChunksVector); //Resize temporary vector of chunk

	for (int i = 0; i < sizeChunksVector; ++i)
	{
		readChunkStructure(chunksVector[i]);
	}


	//Update data
	lodStruct.chunkAmount = chunkAmount;
	lodStruct.verticesPositionVector = verticesPositionVector;
	lodStruct.verticesNormalVector = verticesNormalVector;
	lodStruct.verticesUvVector = verticesUvVector;
	lodStruct.verticesTangentVector = verticesTangentVector;
	lodStruct.chunksVector = chunksVector;



}
*/
//*****************************************************************************************************************************************************
// Function to read klmeshFileStruct from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readKLMeshFileStructure(klmeshFileStruct &klmeshFileStruct)
{

	//Temporary value
	std::string::size_type stringSize = 0; //Size of string;
	std::string name = "";
	int lodAmount = 0;
	int sizeReadVector = 0; //Local value to read amount of vector elements (To read the amount of vectors in the vector)
							//std::vector<std::string> texturesNameVector;
	std::vector<materialStruct> materialsBuffer;
	collisionMeshStruct collisionMeshData;
	//std::vector<lodStruct> lodsData; //Not use
	std::vector<Chunk> objectsVector_LOD0;
	std::vector<Chunk> objectsVector_LOD1;
	std::vector<Chunk> objectsVector_LOD2;


	UINT distanceLOD0 = 0;
	UINT distanceLOD1 = 0;
	UINT distanceLOD2 = 0;
	UINT distanceHide = 0;
	XMFLOAT3 boundingBoxMax = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 boundingBoxMin = XMFLOAT3(0.0f, 0.0f, 0.0f);

	bool castShadow = false;
	short shadowDistance = 0;



	//Read name of file (from file)
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	name.resize(stringSize);
	mInputFile.read(&name[0], stringSize);

	//Read first value
	mInputFile.read((char*)&lodAmount, sizeof(short)); //Read "lodAmount" from file as "short"


	//**********************************
	//Read vector of materials from binary file
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int)); //Read "sizeReadVector" from file as "int" (amount of vector elements)
	for (int i = 0; i < sizeReadVector; ++i)
	{
		materialStruct matTmp;
		readMaterialStructure(matTmp);
		materialsBuffer.push_back(matTmp);
	}

	//**********************************
	//Read 'collisionMeshStruct' from binary file
	//**********************************
	readCollisionMeshStructure(collisionMeshData);

	//**********************************
	//Read 'AllMeshData' from binary file
	//**********************************
	readAllMeshData(klmeshFileStruct.AllMeshData);


	//**********************************
	//Read 'Chunks' from binary file (mObjectsVector_LOD0)
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int));
	for (int i = 0; i < sizeReadVector; ++i)
	{
		Chunk chunkTmp;
		readChunk(chunkTmp);
		objectsVector_LOD0.push_back(chunkTmp);
	}


	//**********************************
	//Read 'Chunks' from binary file (mObjectsVector_LOD1)
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int));
	for (int i = 0; i < sizeReadVector; ++i)
	{
		Chunk chunkTmp;
		readChunk(chunkTmp);
		objectsVector_LOD1.push_back(chunkTmp);
	}

	//**********************************
	//Read 'Chunks' from binary file (mObjectsVector_LOD2)
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int));
	for (int i = 0; i < sizeReadVector; ++i)
	{
		Chunk chunkTmp;
		readChunk(chunkTmp);
		objectsVector_LOD2.push_back(chunkTmp);
	}



	//**********************************
	//Read other values from binary file
	//**********************************
	mInputFile.read((char*)&distanceLOD0, sizeof(UINT));
	mInputFile.read((char*)&distanceLOD1, sizeof(UINT));
	mInputFile.read((char*)&distanceLOD2, sizeof(UINT));
	mInputFile.read((char*)&distanceHide, sizeof(UINT));

	mInputFile.read((char*)&boundingBoxMax, sizeof(XMFLOAT3));
	mInputFile.read((char*)&boundingBoxMin, sizeof(XMFLOAT3));
	/*mInputFile.read((char*)&hasTessellation, sizeof(bool));
	mInputFile.read((char*)&heightScaleTess, sizeof(float));
	mInputFile.read((char*)&maxDistanceTess, sizeof(short));
	mInputFile.read((char*)&minDistanceTess, sizeof(short));
	mInputFile.read((char*)&maxFactorTess, sizeof(short));
	mInputFile.read((char*)&minFactorTess, sizeof(short));*/
	mInputFile.read((char*)&castShadow, sizeof(bool));
	mInputFile.read((char*)&shadowDistance, sizeof(short));


	//Update data
	klmeshFileStruct.mDistanceLOD0 = distanceLOD0;
	klmeshFileStruct.mDistanceLOD1 = distanceLOD1;
	klmeshFileStruct.mDistanceLOD2 = distanceLOD2;
	klmeshFileStruct.mDistanceHide = distanceHide;
	klmeshFileStruct.boundingBoxMax = boundingBoxMax;
	klmeshFileStruct.boundingBoxMin = boundingBoxMin;
	klmeshFileStruct.castShadow = castShadow;
	klmeshFileStruct.collisionMeshData = collisionMeshData;
	//klmeshFileStruct.hasTessellation = hasTessellation;
	//klmeshFileStruct.heightScaleTess = heightScaleTess;
	klmeshFileStruct.lodAmount = lodAmount;
	//klmeshFileStruct.lodsData = lodsData;
	klmeshFileStruct.materialsBuffer = materialsBuffer;
	//klmeshFileStruct.maxDistanceTess = maxDistanceTess;
	//klmeshFileStruct.maxFactorTess = maxFactorTess;
	//klmeshFileStruct.minDistanceTess = minDistanceTess;
	//klmeshFileStruct.minFactorTess = minFactorTess;
	klmeshFileStruct.name = mName;
	klmeshFileStruct.shadowDistance = shadowDistance;

	klmeshFileStruct.mObjectsVector_LOD0 = objectsVector_LOD0;
	klmeshFileStruct.mObjectsVector_LOD1 = objectsVector_LOD1;
	klmeshFileStruct.mObjectsVector_LOD2 = objectsVector_LOD2;
}




//*****************************************************************************************************************************************************
// Function to read vertex from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readVertex(Vertex &vertex)
{

	mInputFile.read((char*)&vertex.Pos, sizeof(XMFLOAT3)); //Read 'position' of vertex from file as "XMFLOAT3"
	mInputFile.read((char*)&vertex.Normal, sizeof(XMFLOAT3)); //Read 'Normal' of vertex from file as "XMFLOAT3"
	mInputFile.read((char*)&vertex.TexC, sizeof(XMFLOAT2)); //Read 'TexC' of vertex from file as "XMFLOAT2"
	mInputFile.read((char*)&vertex.Tangent, sizeof(XMFLOAT4)); //Read 'TangentU' of vertex from file as "XMFLOAT4"
}

//*****************************************************************************************************************************************************
// Function to read AllMeshData to binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readAllMeshData(MeshData &allMeshData)
{

	int sizeReadVector = 0; //Local value to read amount of vector elements (To read the amount of vectors in the vector)
	MeshData AllMeshData;

	//**********************************
	//Read the vector of 'lodStruct' from binary file
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int)); //Read "sizeReadVector" from file as "int" (amount of vector(vertices) elements)
	for (int i = 0; i < sizeReadVector; ++i)
	{
		Vertex vertex;
		readVertex(vertex);
		AllMeshData.Vertices.push_back(vertex); //add read vertex to temporary vector 
	}

	//**********************************
	//Read the vector of 'indices' from binary file
	//**********************************
	mInputFile.read((char*)&sizeReadVector, sizeof(int)); //Read "sizeReadVector" from file as "int" (amount of vector(indices) elements)
	for (int i = 0; i < sizeReadVector; ++i)
	{
		WORD index = -1;
		mInputFile.read((char*)&index, sizeof(WORD)); //Read 'index' of vertex from file as "WORD"
		AllMeshData.Indices.push_back(index);
	}


	allMeshData = AllMeshData;
}

//*****************************************************************************************************************************************************
// Function to read chunk from binary file (klmesh)
//*****************************************************************************************************************************************************
void BinaryFileRead::readChunk(Chunk &chunk)
{
	mInputFile.read((char*)&chunk.materialPosition, sizeof(short)); //Read 'materialPosition' from file as "short"
	mInputFile.read((char*)&chunk.numberOfIndicesToDraw, sizeof(UINT)); //Read 'numberOfIndicesToDraw' from file as "UINT"
	mInputFile.read((char*)&chunk.startIndex, sizeof(UINT)); //Read 'startIndex' from file as "UINT"
	mInputFile.read((char*)&chunk.startVertex, sizeof(UINT)); //Read 'startVertex' from file as "UINT"
}


//*****************************************************************************************************************************************************
// Function to read object from binary file (kll)
//*****************************************************************************************************************************************************
void BinaryFileRead::readObject(objectStruct &objectStructure)
{
	std::string::size_type stringSize = 0; //Size of string;
	std::string name = "";

	//Name
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	name.resize(stringSize);
	mInputFile.read(&name[0], stringSize);
	objectStructure.name = name;

	//TAG
	stringSize = 0;
	name = "";
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	name.resize(stringSize);
	mInputFile.read(&name[0], stringSize);
	objectStructure.tag = name;

	//Link to file
	stringSize = 0;
	name = "";
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	name.resize(stringSize);
	mInputFile.read(&name[0], stringSize);
	objectStructure.linkToFile = name;

	mInputFile.read((char*)&objectStructure.mLocalCoordinatePosition, sizeof(XMFLOAT3)); //Read mLocalCoordinatePosition from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.mLocalAxisX, sizeof(XMFLOAT3)); //Read mLocalAxisX from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.mLocalAxisY, sizeof(XMFLOAT3)); //Read mLocalAxisY from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.mLocalAxisZ, sizeof(XMFLOAT3)); //Read mLocalAxisZ from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.mScaleVector, sizeof(XMFLOAT3)); //Read mScaleVector from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.mPitch, sizeof(float)); //Read mPitch from file as "float"
	mInputFile.read((char*)&objectStructure.mYaw, sizeof(float)); //Read mYaw from file as "float"
	mInputFile.read((char*)&objectStructure.mRoll, sizeof(float)); //Read mRoll from file as "float"
	mInputFile.read((char*)&objectStructure.mIsCastShadow, sizeof(bool)); //Read mIsCastShadow from file as "bool"
	mInputFile.read((char*)&objectStructure.mShadowDistance, sizeof(float)); //Read mShadowDistance from file as "float"
	mInputFile.read((char*)&objectStructure.mIsVisible, sizeof(bool)); //Read mIsVisible from file as "bool"
	mInputFile.read((char*)&objectStructure.mType, sizeof(int)); //Read mType from file as "int"
	mInputFile.read((char*)&objectStructure.Ambient, sizeof(XMFLOAT4)); //Read Ambient from file as "XMFLOAT4"
	mInputFile.read((char*)&objectStructure.Diffuse, sizeof(XMFLOAT4)); //Read Diffuse from file as "XMFLOAT4"
	mInputFile.read((char*)&objectStructure.Specular, sizeof(XMFLOAT4)); //Read Specular from file as "XMFLOAT4"
	mInputFile.read((char*)&objectStructure.Position, sizeof(XMFLOAT3)); //Read Position from file as "XMFLOAT3"
	mInputFile.read((char*)&objectStructure.Range, sizeof(float)); //Read Range from file as "float"
	mInputFile.read((char*)&objectStructure.Att, sizeof(XMFLOAT3)); //Read Att from file as "XMFLOAT3"
}


//*****************************************************************************************************************************************************
// Function to read kllFileStruct from binary file (KL LAYER)
//*****************************************************************************************************************************************************
void BinaryFileRead::readKLLFileStructure(kllFileStruct &kllFileStruct)
{
	int sizeReadVector = 0; //Local value to read amount of vector elements 

	mInputFile.read((char*)&sizeReadVector, sizeof(int));
	for (int i = 0; i < sizeReadVector; ++i)
	{
		objectStruct objectStructTemp;
		readObject(objectStructTemp);
		kllFileStruct.objectBuffer.push_back(objectStructTemp);
	}
}

//*****************************************************************************************************************************************************
// Function to read klwFileStruct from binary file (KL WORLD)
//*****************************************************************************************************************************************************
void BinaryFileRead::readKLWFileStructure(klwFileStruct &klwFileStruct)
{
	std::string::size_type stringSize = 0; //Size of string;
	std::string name = "";

	//Name
	mInputFile.read((char*)&stringSize, sizeof(stringSize));
	name.resize(stringSize);
	mInputFile.read(&name[0], stringSize);
	klwFileStruct.mName = name;
}
