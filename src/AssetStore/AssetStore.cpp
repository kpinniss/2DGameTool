#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore()
{
    Logger::Log("Asset Store constructor called");
}

AssetStore::~AssetStore()
{
    ClearAssets();
    Logger::Log("Asset Store destructor called");
}

void AssetStore::ClearAssets()
{
    //destory all textures
    for(auto text: textures){
        SDL_DestroyTexture(text.second);
    }
    // then clear
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* rndr,const std::string& assetId, const std::string& filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rndr, surface);
    SDL_FreeSurface(surface);
    //add text to the map
    textures.emplace(assetId, texture);
    Logger::Log("Texture added to asset store: " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assestId)
{
    return textures[assestId];
}

