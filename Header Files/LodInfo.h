#ifndef __LOD_INFO__
#define  __LOD_INFO__

#include "Utility.h"

class LodInfo
{
public:
	LodInfo();
	~LodInfo();
	unsigned int GetDistance();
	unsigned int GetNumberOfTriangles();
	unsigned int GetNumberOfVertices();
	unsigned int GetNumberOfIndices();
	unsigned int GetNumberOfChunks();
	unsigned int GetNumberOfMaterials();

	void SetDistance(unsigned int value);
	void SetNumberOfTriangles(unsigned int value);
	void SetNumberOfVertices(unsigned int value);
	void SetNumberOfIndices(unsigned int value);
	void SetNumberOfChunks(unsigned int value);
	void SetNumberOfMaterials(unsigned int value);


private:
	unsigned int mDistance;
	unsigned int mNumberOfTriangles;
	unsigned int mNumberOfVertices;
	unsigned int mNumberOfIndices;
	unsigned int mNumberOfChunks;
	unsigned int mNumberOfMaterials;
};



#endif // !__LOD_INFO__