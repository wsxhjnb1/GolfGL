/*
  This code was mainly copied from https://github.com/JoeyDeVries/LearnOpenGL
*/

#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Render {
    class Model
    {
    public:
        Model(std::string name)
        {            
            loadModel(name);
        }
        void Draw(Shader* shader);        

    protected:
        std::vector<Texture> textures_loaded;
    private:
        // model data        
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            TextureType typeName);
    };
} // Redner