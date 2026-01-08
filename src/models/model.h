#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

using namespace std;

class Model {
public:
    Model(const string& path);
    void Draw(Shader& shader);

private:
    vector<Mesh> meshes;
    string directory;

    void loadModel(const string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat,
        aiTextureType type,
        string typeName);
};

#endif
