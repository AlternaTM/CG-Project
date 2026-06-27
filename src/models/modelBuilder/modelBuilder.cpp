#include "modelBuilder.h"

#include "models/mesh.h"

# define M_PI           3.14159265358979323846  /* pi */


ModelRenderer* ModelBuilder::buildPlane(
    glm::vec2 size,
    const std::string& texturePath,
    Shader* shader
) {
	vector<Vertex> verts;
	vector<unsigned int> indices;


    float maxHeight = 5.0f;

	int W = 40, H = 40;
    float center = 0.75f;
    float steepness = 20.0f;

    for (int j = 0; j <= H; j++) {
        for (int i = 0; i <= W; i++) {
            float x = ((float)i / W - 0.5f) * size.x;
            float z = ((float)j / H - 0.5f) * size.y;

            float t = 1 - ((float)j / H);
            float y = maxHeight * (1.0f / (1.0f + exp(-steepness * (t - center))));

            Vertex v;
            v.Position = glm::vec3(x, y, z);
            v.Normal = glm::vec3(0, 1, 0);
            v.TexCoords = glm::vec2((float)i / W, (float)j / H);
            verts.push_back(v);
        }
    }

    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; i++) {
            unsigned int tl = j * (W + 1) + i;
            unsigned int tr = tl + 1;
            unsigned int bl = tl + (W + 1);
            unsigned int br = bl + 1;
            indices.push_back(tl); indices.push_back(bl); indices.push_back(tr);
            indices.push_back(tr); indices.push_back(bl); indices.push_back(br);
        }
    }

    Model* sup = new Model(verts, indices, texturePath);

    return new ModelRenderer(sup, shader);
}