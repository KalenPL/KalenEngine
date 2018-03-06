#include "../Header Files/LodInfo.h"


LodInfo::LodInfo()
	:
	mDistance(0),
	mNumberOfTriangles(0),
	mNumberOfVertices(0),
	mNumberOfIndices(0),
	mNumberOfChunks(0),
	mNumberOfMaterials(0)
{

}


LodInfo::~LodInfo()
{

}

unsigned int LodInfo::GetDistance()
{
	return this->mDistance;
}
unsigned int LodInfo::GetNumberOfTriangles()
{
	return this->mNumberOfTriangles;
}
unsigned int LodInfo::GetNumberOfVertices()
{
	return this->mNumberOfVertices;
}
unsigned int LodInfo::GetNumberOfIndices()
{
	return this->mNumberOfIndices;
}
unsigned int LodInfo::GetNumberOfChunks()
{
	return this->mNumberOfChunks;
}
unsigned int LodInfo::GetNumberOfMaterials()
{
	return this->mNumberOfMaterials;
}


void LodInfo::SetDistance(unsigned int value)
{
	this->mDistance = value;
}
void LodInfo::SetNumberOfTriangles(unsigned int value)
{
	this->mNumberOfTriangles = value;
}
void LodInfo::SetNumberOfVertices(unsigned int value)
{
	this->mNumberOfVertices = value;
}
void LodInfo::SetNumberOfIndices(unsigned int value)
{
	this->mNumberOfIndices = value;
}
void LodInfo::SetNumberOfChunks(unsigned int value)
{
	this->mNumberOfChunks = value;
}
void LodInfo::SetNumberOfMaterials(unsigned int value)
{
	this->mNumberOfMaterials = value;
}