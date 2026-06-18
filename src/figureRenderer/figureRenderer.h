#pragma once
#include <string>
#include "shader/shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <GLFW/glfw3.h>

struct Vertex2D {
	glm::vec2 pos;
	glm::vec2 uv;
};


class Mesh2D {
public: 
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	uint32_t indexCount = 0;

	void create(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices);


	void destroy();
};


class FigRenderer {
public:

	FigRenderer(const char* vertexPath, const char* fragmentPath);
	~FigRenderer();

	FigRenderer(FigRenderer&&) = default;
	FigRenderer& operator=(FigRenderer&&) = default;
	
	void drawRect(
		const glm::vec2& pos, 
		const glm::vec2& size,
		float angle,           // radianti
		const glm::mat4& view,
		const glm::vec4& color = glm::vec4(1, 0, 0, 1)
	);
	bool init(const glm::mat4& projection);

	void draw(
		const Mesh2D& mesh,
		const glm::mat4& transform,
		const glm::mat4& view,
		const glm::vec4& color = glm::vec4(1, 0, 0, 1),
		const float uTime = (float)glfwGetTime()
	);
private:
	Shader shader;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
};