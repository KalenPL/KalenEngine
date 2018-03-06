#include "../Header Files/KLMESH_Buffer.h"


KLMESH_Buffer::KLMESH_Buffer()
{
	mKLMeshBuffer = new std::vector<KLMESH*>();
	mKLMeshBuffer->clear();
}
//*************************************************************************************************************************************
KLMESH_Buffer::~KLMESH_Buffer()
{
	for (int i = 0; i < (int)mKLMeshBuffer->size(); ++i)
		delete mKLMeshBuffer->at(i); //Delete all KLMESHes from buffer
	mKLMeshBuffer->clear(); //clear
	delete mKLMeshBuffer; //delete
}
//*************************************************************************************************************************************
KLMESH* KLMESH_Buffer::AddMesh(KLMESH* klmesh)
{
	bool isExist = false;
	int position = -1;
	for (int i = 0; i < (int)mKLMeshBuffer->size(); ++i)
		if (mKLMeshBuffer->at(i)->GetLinkToFile() == klmesh->GetLinkToFile())
		{
			isExist = true;
			position = i;
		}
	if (isExist == false)
	{
		//Add KLMESH to buffer
		mKLMeshBuffer->push_back(klmesh);
		return mKLMeshBuffer->at(mKLMeshBuffer->size() -1); //return pointer to KLMESH
	}
	else
		return mKLMeshBuffer->at(position);  //return pointer to KLMESH
}
//*************************************************************************************************************************************
bool KLMESH_Buffer::CheckMesh(std::string link)
{
	bool isExist = false;
	for (int i = 0; i < (int)mKLMeshBuffer->size(); ++i)
		if (mKLMeshBuffer->at(i)->GetLinkToFile() == link)
			isExist = true;

	return isExist;
}
//*************************************************************************************************************************************
void KLMESH_Buffer::DeleteMesh(KLMESH* klmesh)
{
	int position = -1;
	for (int i = 0; i < (int)mKLMeshBuffer->size(); ++i)
		if (mKLMeshBuffer->at(i)->GetLinkToFile() == klmesh->GetLinkToFile())
			position = i;

	mKLMeshBuffer->erase(mKLMeshBuffer->begin() + position); //delete klmesh from buffer.
}
KLMESH* KLMESH_Buffer::GetMeshByLink(std::string link)
{
	bool isExist = false;
	int pos = -1;
	for (int i = 0; i < (int)mKLMeshBuffer->size(); ++i)
		if (mKLMeshBuffer->at(i)->GetLinkToFile() == link)
			pos = i;
	if (pos == -1)
		return nullptr;
	else
		return mKLMeshBuffer->at(pos);
}
