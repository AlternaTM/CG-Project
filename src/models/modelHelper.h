#pragma once

#include <glm/glm.hpp>
#include "../modelRenderer/modelRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "mesh.h"
#include "model.h"
class ModelRenderer;

class ModelHelper {
public:
    static float getMaxY(ModelRenderer& renderer);
    static float getMinY(ModelRenderer& renderer);
    static float getMinZ(ModelRenderer& renderer);
    static float getMaxZ(ModelRenderer& renderer);
};
