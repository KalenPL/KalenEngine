#ifndef BINARY_FILE_WRITE_H
#define BINARY_FILE_WRITE_H

#include "File.h"

class BinaryFileWrite
{
public:
	//BinaryFileIO(ofstream* output_file, ifstream* input_file); std::string fileName
	BinaryFileWrite(std::string fileName);
	~BinaryFileWrite();
	void closeFile();

	//Write to binary file (klmesh)
	//KLMESH structure
	void writeKLMeshFileStructure(klmeshFileStruct &klmeshFileStruct);

	//KLL - layer
	void writeKLLFileStructure(kllFileStruct &kllFileStruct);

	//KLW - world
	void writeKLWFileStructure(klwFileStruct &klwFileStruct);



private:
	ofstream mOutputFile; //Write to file

						  //Write to binary file (klmesh)
						  //Material structure
	void writeMaterialStructure(materialStruct &materialStruct);
	//Collision mesh structure
	void writeCollisionMeshStructure(collisionMeshStruct &collisionMeshStruct);
	//Chunk structure
	void writeChunkStructure(chunkStruct &chunkStruct);
	//Lod structure
	//void writeLODStructure(lodStruct &lodStruct);

	//Write vertex to binary file
	void writeVertex(Vertex &vertex);

	//Write allMeshData to binary file
	void writeAllMeshData(MeshData &allMeshData);

	//Write chunk to binary file
	void writeChunk(Chunk &chunk);

	void writeObject(objectStruct &object);
};
#endif //BINARY_FILE_WRITE_H