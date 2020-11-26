#include "asset.h"

Asset::Asset(AAsset* assetPtr)
{
    mAsset = assetPtr;
    bIsOpen = mAsset != nullptr;
}

void Asset::Read(char* buffer, const uint32_t size)
{
    AAsset_read(mAsset, buffer, size);
}

void Asset::Seek(const uint32_t offset, const AssetSeekDir& direction)
{
    AAsset_seek(mAsset, offset, (int32_t)direction);
}

void Asset::Close()
{
    AAsset_close(mAsset);
}

uint32_t Asset::GetCursorOffset() const
{
    return AAsset_seek(mAsset, 0, (int32_t)AssetSeekDir::Current);
}

bool Asset::IsOpen() const
{
    return bIsOpen;
}