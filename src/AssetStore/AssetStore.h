#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetStore
{
    private:
        std::map<std::string, SDL_Texture*> textures;
        //TODO:
        //create a mpa for fonts and audio 
    public:
        AssetStore();
        ~AssetStore();

        void ClearAssets();
        void AddTexture(SDL_Renderer* rndr,const std::string& assetId, const std::string& filePath);
        SDL_Texture* GetTexture(const std::string& assestId);

};
#endif