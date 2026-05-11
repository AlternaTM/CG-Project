#pragma once
#include <string>
#include "shader/shader.h"
#include <glm/glm.hpp>

class FigRenderer {
public:
	FigRenderer(Shader& shader);
	~FigRenderer();

	bool init(const std::string& vertPath, const std::string& fragPath);

	void drawRect(
		const glm::vec2& pos,
		const glm::vec2& size,
		const glm::mat4& view,
		const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
	);
	void drawRect(
		const glm::vec2& pos, 
		const glm::vec2& size,
		float angle,           // radianti
		const glm::mat4& view,
		const glm::vec4& color = glm::vec4(1, 0, 0, 1)
	);
	bool init();
private:
	Shader& shader;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
};