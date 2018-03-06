#ifndef KLMESH_BUFFER_H
#define KLMESH_BUFFER_H


#include "Utility.h"
#include "KLMESH.h"

//------------------------------------------------------------------------------------------
// Class KLMESH_Buffer
//------------------------------------------------------------------------------------------
class KLMESH_Buffer
{
public:
	KLMESH_Buffer();
	~KLMESH_Buffer();

	KLMESH* AddMesh(KLMESH* klmesh);//If the object does not exist in the buffer, add the object and return the pointer to the object. If exists, return only the pointer to the object
	void DeleteMesh(KLMESH* klmesh);
	bool CheckMesh(std::string link); //get link to klmesh and return true if object exists in the buffer. 
	KLMESH* GetMeshByLink(std::string link);

private:
	std::vector<KLMESH*>* mKLMeshBuffer;

};
#endif //!KLMESH_BUFFER_H