/*
  This code was mainly copied from https://github.com/JoeyDeVries/LearnOpenGL
*/

#pragma once

#include "Mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Render
{    
    using Textures = std::vector<Texture>;

    class Model
    {
    public:
        Model(const std::string &name)
        {
            loadModel(name);
        }

        void Draw(Shader *shader);

        const Textures& GetTextures() const;

    private:
        // model data
        Textures textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        Textures loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName);
    };
} // namespace Render
