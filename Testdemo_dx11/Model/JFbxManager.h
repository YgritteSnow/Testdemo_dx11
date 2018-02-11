#pragma once
#include <fbxsdk.h>

class JFbxManager
{
public:
	JFbxManager() {}
	~JFbxManager() {}
	static bool Init() {
		s_manager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(s_manager, IOSROOT);
		s_manager->SetIOSettings(ios);

		return true;
	}
	static bool UnInit() {
		s_manager->Destroy();
	}

	static bool LoadFbx(const char* filename)
	{
		FbxImporter* importer;

		// Create an importer using the SDK manager.
		importer = FbxImporter::Create(s_manager, "");

		// Use the first argument as the filename for the importer.
		if (!importer->Initialize(filename, -1, importer->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
			return false;
		}

		// Create a new scene so that it can be populated by the imported file.
		FbxScene* lScene = FbxScene::Create(s_manager, "myScene");

		// Import the contents of the file into the scene.
		importer->Import(lScene);

		// The file is imported, so get rid of the importer.
		importer->Destroy();
	}

private:
	static FbxManager* s_manager;
};