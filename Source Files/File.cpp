#include "../Header Files/File.h"


//**************************************************************************************************************
// Constructor 
//**************************************************************************************************************
File::File()
{
}

File::File(ID3D11Device* pDevice)
{
	this->m_pDevice = pDevice;
	TexturesResource texturesResource(pDevice);
	mTexturesResource = texturesResource;
}
//**************************************************************************************************************
// Destructor
//**************************************************************************************************************
File::~File()
{

}

//**************************************************************************************************************
// Ste Device
//**************************************************************************************************************
void File::SetDivice(ID3D11Device* pDevice)
{
	this->m_pDevice = pDevice;
}






//**************************************************************************************************************
// Load Object File
//**************************************************************************************************************
/*Object File::LoadObjectFile(std::string fileName, XMFLOAT3 &centerBoxSelect, XMFLOAT3 &extentsBoxSelect)
{
	//!!!!!!!!!!!!!!!!!!!!!!!
	//Ta funkcja nie jest obecnie uzywana --> Zostala przeniesiona do watku (KalenEngine.cpp)
	//!!!!!!!!!!!!!!!!!!!!!!!

	Object object;
	
return object;
}*/

//**************************************************************************************************************
// Load Objects File
//**************************************************************************************************************
/*bool File::LoadSceneFile(std::string fileName, std::vector<Object> &mObjectsVector)
{
	std::ifstream file(fileName.c_str());
	if (!file)
	{
		INFO_MESSAGE(L"Plik sceny nie istnieje!");
		return false;
	}


	std::string ignore;
	UINT IndependentObj = 0;
	UINT actualLod = 0;
	UINT chunkCount = 0;

	file >> ignore;
	file >> ignore >> ignore >> ignore;
	file >> ignore;
	file >> ignore;
	file >> ignore >> IndependentObj;


	for (int i = 0; i < IndependentObj; ++i)//petla
	{

		//Wczytywanie informacji o obiekcie z pliku
		file >> ignore;
		file >> ignore >> ignore;
		file >> ignore;
		UINT CopyObj = 0;
		file >> CopyObj;
		file >> ignore;
		std::string objectName;
		file >> objectName;


		//std::wstring stemp = std::wstring(objectName.begin(), objectName.end());
		//std::wstring stemp = std::wstring(objectName.begin(), objectName.end());
		//LPCWSTR name = stemp.c_str();
		//INFO_MESSAGE(name);


		Object object;

		object.setFileName(objectName); // To dodalem _____________________________________
		//XMFLOAT3 centerBoxSelect;
		//XMFLOAT3 extentsBoxSelect;
		//object = LoadObjectFile(objectName, centerBoxSelect, extentsBoxSelect); //Load object file //To tez teraz nie jest potrzebne
		//TODO Tutaj dodac watki

		//XMFLOAT3 save_centerBoxSelect = centerBoxSelect;
		for (int k = 0; k < CopyObj; ++k)	//petla po wspolrzednych
		{
			file >> ignore;
			LocalCoordinateSystem localspace;
			XMFLOAT3 vector;

			//XMFLOAT3 centerBoxSelectTmp = centerBoxSelect;
			XMFLOAT3 centerBoxSelectTmp = XMFLOAT3(0.0f, 0.0f, 0.0f);

			file >> vector.x >> vector.y >> vector.z;
			localspace.setLocalAxisX(vector.x, vector.y, vector.z);

			file >> vector.x >> vector.y >> vector.z;
			localspace.setLocalAxisY(vector.x, vector.y, vector.z);

			file >> vector.x >> vector.y >> vector.z;
			localspace.setLocalAxisZ(vector.x, vector.y, vector.z);

			file >> vector.x >> vector.y >> vector.z;
			localspace.SetLocalCoordinatePosition(vector.x, vector.y, vector.z);

			meshPosition meshPos;
			meshPos.coordinate = localspace;
			
			//centerBoxSelectTmp.x += vector.x;
			//centerBoxSelectTmp.y += vector.y;
			//centerBoxSelectTmp.z += vector.z;

			centerBoxSelectTmp.x = vector.x;
			centerBoxSelectTmp.y = vector.y;
			centerBoxSelectTmp.z = vector.z;

			meshPos.meshBox.Center = centerBoxSelectTmp;//To tez teraz nie jest potrzebne
			//meshPos.meshBox.Extents = extentsBoxSelect;//To tez teraz nie jest potrzebne
			meshPos.meshBox.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

			meshPos.mIsSelected = false;
			meshPos.mIsCastShadow = true;
			meshPos.mIsVisible = false;//Zmieniam na false, poniewaz true bedzie dopiero po pelnym wczytaniu obiektu.
			object.manyObjectsVector.push_back(meshPos);
		}

		for (int i = 0; i < object.manyObjectsVector.size(); ++i)
			object.manyObjectsVector[i].coordinate.UpdateCoordinateMatrix();
		//object.createBufferVertexAndIndex(m_pDevice); //To wywalic do osobnej funkcji
		mObjectsVector.push_back(object);
	}
	file.close();
	
	return true;
}

*/



//**************************************************************************************************************
// Save Scene File
//**************************************************************************************************************
/*bool File::SaveSceneFile(std::string fileName, std::vector<Object> &mObjectsVector)
{
	std::fstream file(fileName.c_str(), std::ios::out);
	if (!file.is_open())
		return false;


	file << "###############################################################" << std::endl;
	file << "#KALEN_ENGINE - SAVED_SCENE" << std::endl;
	file << "###############################################################" << std::endl;
	file << "######################" << std::endl;
	file << "#Independent_objects: " << mObjectsVector.size() << std::endl;
	//file << "######################" << std::endl;
	for (int i = 0; i < mObjectsVector.size(); i++) // dla wszystkich niezale¿nych obiektów 
	{
		file << "######################" << std::endl;
		file << "#Object: " << i + 1 << std::endl;
		file << "#CopiesOfThisObject: " << mObjectsVector[i].manyObjectsVector.size() << std::endl;
		file << "######################" << std::endl;


		file << mObjectsVector[i].getFileName() << std::endl;
		//TODO - dopisac nazwe pliku!!!

		for (int k = 0; k < mObjectsVector[i].manyObjectsVector.size(); k++) // dla wielu takich samych obiektów
		{
			file << "####" << std::endl;
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisX().x << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisX().y << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisX().z << std::endl;
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisY().x << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisY().y << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisY().z << std::endl;
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisZ().x << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisZ().y << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalAxisZ().z << std::endl;
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalCoordinatePosition().x << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalCoordinatePosition().y << " ";
			file << mObjectsVector[i].manyObjectsVector[k].coordinate.GetLocalCoordinatePosition().z << std::endl;

		}
	}

	file.close();
}
*/