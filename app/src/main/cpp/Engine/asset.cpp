#include "asset.h"

Asset::Asset(AAsset* assetPtr)
{
    mAsset = assetPtr;
    bIsOpen = (mAsset != nullptr);
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
    if (bIsOpen) {
        AAsset_close(mAsset);
        bIsOpen = false;
    }
}

uint32_t Asset::GetCursorOffset() const
{
    return AAsset_seek(mAsset, 0, (int32_t)AssetSeekDir::Current);
}

uint32_t Asset::GetLength() const
{
    return AAsset_getLength(mAsset);
}

bool Asset::IsOpen() const
{
    return bIsOpen;
}