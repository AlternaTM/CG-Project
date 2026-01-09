#include "modelRenderer.h"
#include "../models/model.h"
#include "../shader/shader.h"
#include <glm/gtc/matrix_transform.hpp>

ModelRenderer::ModelRenderer(const std::string& modelPath, const std::string& vertexShader,
    const std::string& fragmentShader)
    : position(0.0f), scale(1.0f), rotation(0.0f),pivot(0.0f)
{
    model = new Model(modelPath);
    shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
}

ModelRenderer::~ModelRenderer() {
    delete model;   
    delete shader;
}

void ModelRenderer::setPosition(glm::vec3 pos) { position = pos; }
void ModelRenderer::setScale(glm::vec3 s) { scale = s; }
void ModelRenderer::setRotation(glm::vec3 rot) { rotation = rot; }
void ModelRenderer::setPivot(glm::vec3 p) { pivot = p; }

const glm::vec3& ModelRenderer::getPosition() const { return position; }
const glm::vec3& ModelRenderer::getScale() const { return scale; }
const glm::vec3& ModelRenderer::getRotation() const { return rotation; }
const glm::vec3& ModelRenderer::getPivot() const { return pivot; }

void ModelRenderer::render(const glm::mat4& view, const glm::mat4& projection) {


    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);
    modelMat = glm::scale(modelMat, scale);


    //per la rotazione con supporto per il pivot

    modelMat = glm::translate(modelMat, pivot);
    modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1, 0, 0)); // X 
    modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0, 1, 0)); // Y 
    modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0, 0, 1)); // Z 
    modelMat = glm::translate(modelMat, -pivot);


 

    shader->setMat4("model", modelMat);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    model->Draw(*shader);
}