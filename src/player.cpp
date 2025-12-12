#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <player/player.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player()
	: position(0.0f), speed(15.0f), mousePosition(0.0f)
{
    

    setupQuad();
	setupLine();
}

Player::~Player() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);
}

void Player::setupLine() {
    // Implementation for line setup (if needed in the future)
    
    float vertices[] = {
        0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
    };
    

    glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);

	glBindVertexArray(lineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 6, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

void Player::setupQuad() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Player::draw(const Shader& shader) {

    // --- CAMERA LOGIC ---
    float screenHeight = 9.0f;
    float screenWidth = 16.0f;

    float halfW = screenWidth / 2.0f;
    float halfH = screenHeight / 2.0f;
    float camX = position.x;
    float camY = position.y;

    float worldWidth = screenWidth * 2.0f;
    float worldHeight = screenHeight * 2.0f;

    camX = glm::clamp(camX, -worldWidth * 0.5f + halfW, worldWidth * 0.5f - halfW);
    camY = glm::clamp(camY, -worldHeight * 0.5f + halfH, worldHeight * 0.5f - halfH);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camX, -camY, 0.0f));

    shader.setMat4("uView", view);


    shader.setMat4("uModel", glm::translate(glm::mat4(1.0f), position));
    shader.setVec3("uColor", glm::vec3(0.5f, 1.0f, 0.2f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Draw line from player to mouse position

	glm::vec3 cam = glm::vec3(camX, camY, 0.0f);
    glm::vec3 pos = position - cam;
    pos.x /= 8.0f;
	pos.y /= 4.5f;

	/*std::cout << "Player Pos: (" << pos.x << ", " << pos.y << ")\n";
	std::cout << "Mouse Pos: (" << mousePosition.x << ", " << mousePosition.y << ")\n";*/

	glm::vec3 d = mousePosition - pos;
	float len = glm::length(d);
    if(len < 1e-5f) {
		std::cout << "Length too small, skipping line draw.\n";
        return;
	}
	glm::vec3 dir = d / len;

	float angle = atan2(dir.y, dir.x);
    
    glm::mat4 M = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	
    shader.setMat4("uModel", M);
    //shader.setMat4("uModel", glm::translate(glm::mat4(1.0f), position));
    shader.setVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    
}
