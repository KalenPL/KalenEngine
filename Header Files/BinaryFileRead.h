#ifndef BINARY_FILE_READ_H
#define BINARY_FILE_READ_H

#include "Utility.h"
#include "File.h"




class BinaryFileRead
{
public:
	BinaryFileRead(std::string fileName);
	~BinaryFileRead();

	void closeFile();

	//Read from binary file (klmesh)
	//KLMESH structure
	void readKLMeshFileStructure(klmeshFileStruct &klmeshFileStruct);

	//Layer
	void readKLLFileStructure(kllFileStruct &kllFileStruct);

	//World
	void readKLWFileStructure(klwFileStruct &klwFileStruct);


private:
	ifstream mInputFile; //Read from file
	std::string mName;
	//Read and write to binary file (klmesh)
	//Material structure
	void readMaterialStructure(materialStruct &materialStruct);
	//Collision mesh structure
	void readCollisionMeshStructure(collisionMeshStruct &collisionMeshStruct);
	//Chunk structure
	void readChunkStructure(chunkStruct &chunkStruct);
	//Lod structure
	//void readLODStructure(lodStruct &lodStruct);


	//Read vertex from binary file
	void readVertex(Vertex &vertex);

	//Read allMeshData from binary file
	void readAllMeshData(MeshData &allMeshData);

	//Read chunk v binary file
	void readChunk(Chunk &chunk);


	void readObject(objectStruct &objectStructure);

};
#endif //BINARY_FILE_READ_H