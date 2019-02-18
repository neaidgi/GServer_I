#ifndef _NETWORKDATAMANAGER_H_
#define _NETWORKDATAMANAGER_H_

#include "FileSystem.h"

class NetworkDataManager {
private:
	static NetworkDataManager* Instance;

	NetworkDataManager();
	~NetworkDataManager();
public:
	static void CreateInstance();
	static NetworkDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();
};

#endif