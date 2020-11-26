#ifndef ASSET_H
#define ASSET_H

#include <android/asset_manager.h>

#include <cstdint>
#include <cstdio>

class Asset
{
private:
    typedef enum class ASSET_SEEK_DIR : int32_t {
        Begin   = SEEK_SET,
        Current = SEEK_CUR,
        End     = SEEK_END
    } AssetSeekDir;

public:
    Asset(AAsset* assetPtr);

    void Read(char* buffer, const uint32_t size);
    void Seek(const uint32_t offset, const AssetSeekDir& direction);
    void Close();
    uint32_t GetCursorOffset() const;

    bool IsOpen() const;

private:
    AAsset* mAsset;
    bool bIsOpen;
};

#endif // ASSET_H