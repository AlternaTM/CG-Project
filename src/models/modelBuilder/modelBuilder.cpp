#include "modelBuilder.h"

#include "models/mesh.h"

# define M_PI           3.14159265358979323846  /* pi */


ModelRenderer ModelBuilder::buildPlane(glm::vec2 size, const std::string& vertexShader, const std::string& fragmentShader) {
	vector<Vertex> verts;
	vector<unsigned int> indices;

	int W = 40, H = 40;

    for (int j = 0; j <= H; j++) {
        for (int i = 0; i <= W; i++) {
            float x = (float)i / W * 2.0f - 1.0f;
            float z = (float)j / H * 2.0f - 1.0f;
            float y = 0.3f * sin(x * M_PI * 2) * cos(z * M_PI * 2);

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

    Model* sup = new Model(verts, indices, "assets/texture.png");

    return ModelRenderer(sup, vertexShader, fragmentShader);
}