#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "asset.h"

class AssetManager
{
public:
    void Create(AAssetManager* managerPtr);

    Asset OpenAsset(const char* filename);

private:
    AAssetManager* mAssetManager;
};

#endif // ASSET_MANAGER_H