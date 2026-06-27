#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "models/model.h"
#include "../../modelRenderer/modelRenderer.h"

class ModelBuilder {
public:
	static ModelRenderer* buildPlane(
		glm::vec2 size,
		const std::string& texturePath,
		Shader* shader
	);
};