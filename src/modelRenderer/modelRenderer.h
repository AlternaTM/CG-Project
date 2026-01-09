#pragma once
#include <glm/glm.hpp>
#include <string>

// Forward declarations
class Model;
class Shader;

class ModelRenderer {
private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 pivot;


public:
    Model* model;
    Shader* shader;
    ModelRenderer(const std::string& modelPath, const std::string& vertexShader,
        const std::string& fragmentShader);
    ~ModelRenderer();

    void setPosition(glm::vec3 pos);
    void setScale(glm::vec3 s);
    void setRotation(glm::vec3 rotation);
    void setPivot(glm::vec3 p);

    const glm::vec3& getPosition() const;
    const glm::vec3& getScale() const;
    const glm::vec3& getRotation() const;
    const glm::vec3& getPivot() const;
    void render(const glm::mat4& view, const glm::mat4& projection);
};