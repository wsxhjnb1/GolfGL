#pragma once

#include <vector>
#include <string>
#include <Render/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Render {
    class Model
    {
    public:
        Model(std::string name)
        {
            // name.insert(0, objectDir);
            loadModel(name);
        }
        void Draw(Shader* shader);

        virtual void Update(float delta) = 0;

        // inline static const char* objectDir = "Resources\\Objects\\";
    private:
        // model data
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            std::string typeName);
    };
} // Redner