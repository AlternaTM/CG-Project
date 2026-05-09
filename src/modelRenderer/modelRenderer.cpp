#include "modelRenderer.h"
#include "../models/model.h"
#include "../shader/shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


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

void ModelRenderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::mat4* parentMatrix) {

    glm::mat4 modelMat = getModelMatrix(); //glm::mat4(1.0f);
    //modelMat = glm::translate(modelMat, position);
    //modelMat = glm::scale(modelMat, scale);



    if (parentMatrix != nullptr) {
        modelMat = *parentMatrix * modelMat;
    }

    //per la rotazione con supporto per il pivot
    /*
    modelMat = glm::translate(modelMat, pivot);

    glm::quat qX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));

    glm::quat finalRot = qZ * qY * qX;
    modelMat = modelMat * glm::mat4_cast(finalRot);
    modelMat = glm::translate(modelMat, -pivot);
    */

    shader->setMat4("model", modelMat);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    model->Draw(*shader);
}

glm::mat4 ModelRenderer::getModelMatrix() const {
    glm::mat4 m = glm::mat4(1.0f);
    m = glm::translate(m, position);
    m = glm::scale(m, scale);

    m = glm::translate(m, pivot);
    glm::quat qX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    glm::quat qY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    glm::quat qZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    m = m * glm::mat4_cast(qZ * qY * qX);
    m = glm::translate(m, -pivot);

    return m;
}


//METODI STATICI DI SUPPORTO
void ModelRenderer::render_chest(Camera3D& camera3D, glm::mat4& projection3D, ModelRenderer& chest, ModelRenderer& chest_lid, float dt) {
    static float angle = 0.0f;
    //angle += 20.0f * dt;

    chest.setRotation(glm::vec3(0, 20.0f, 0));

    chest_lid.setPosition(glm::vec3(0, 1.0f, -1.0f));
    chest_lid.setRotation(glm::vec3(-90.0f, 0.0f, 0));
    
    glm::mat4 view = camera3D.getViewMatrix();

    //glm::mat4 view = glm::lookAt(camera3D.position, camera3D.target, camera3D.up);

    Shader& shader = *chest.shader;
    shader.use();
    shader.setVec3("viewPos", camera3D.position);

    // Luce principale
    shader.setVec3("lightPos1", glm::vec3(2, 2, 2));
    shader.setVec3("lightColor1", glm::vec3(1, 1, 1));

    // Luce sopra-destra
    shader.setVec3("lightPos2", glm::vec3(5, 8, 5));
    shader.setVec3("lightColor2", glm::vec3(0.6f, 0.6f, 0.7f));


    glm::mat4 chestWorld = chest.getModelMatrix();

    chest.render(camera3D.getViewMatrix(), projection3D);
    chest_lid.render(camera3D.getViewMatrix(), projection3D, &chestWorld);
}