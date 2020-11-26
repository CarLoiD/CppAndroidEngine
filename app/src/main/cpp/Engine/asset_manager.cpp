#include "asset_manager.h"

void AssetManager::Create(AAssetManager* managerPtr)
{
    mAssetManager = managerPtr;
}

Asset AssetManager::OpenAsset(const char* filename)
{
    return { AAssetManager_open(mAssetManager, filename, AASSET_MODE_UNKNOWN) };
}