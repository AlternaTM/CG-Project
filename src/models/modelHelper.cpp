#include "modelHelper.h"


float ModelHelper::getMaxY(ModelRenderer& renderer) {
    float maxY = -FLT_MAX;

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, renderer.getPosition());
    transform = glm::scale(transform, renderer.getScale());

    

    transform = glm::translate(transform, renderer.getPivot());
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().z), glm::vec3(0, 0, 1));
    transform = glm::translate(transform, -renderer.getPivot());

    
    std::vector<Mesh>& meshes = renderer.model->meshes;
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];
        for (auto& v : mesh.vertices) {
            glm::vec4 transformedPos = transform * glm::vec4(v.Position, 1.0f);
            maxY = std::max(maxY, transformedPos.y);
        }
    }

    return maxY;
}

float ModelHelper::getMinY(ModelRenderer& renderer) {
    float minY = FLT_MAX;


    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, renderer.getPosition());
    transform = glm::scale(transform, renderer.getScale());


    transform = glm::translate(transform, renderer.getPivot());
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().z), glm::vec3(0, 0, 1));
    transform = glm::translate(transform, -renderer.getPivot());


    std::vector<Mesh>& meshes = renderer.model->meshes;
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];
        for (auto& v : mesh.vertices) {
            glm::vec4 transformedPos = transform * glm::vec4(v.Position, 1.0f);
            minY = std::min(minY, transformedPos.y);
        }
    }

    return minY;
}

float ModelHelper::getMinZ(ModelRenderer& renderer) {
    float minZ = FLT_MAX;


    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, renderer.getPosition());
    transform = glm::scale(transform, renderer.getScale());

    
    transform = glm::translate(transform, renderer.getPivot());
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().z), glm::vec3(0, 0, 1));
    transform = glm::translate(transform, -renderer.getPivot());

    std::vector<Mesh>& meshes = renderer.model->meshes;
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];
        for (auto& v : mesh.vertices) {
            glm::vec4 transformedPos = transform * glm::vec4(v.Position, 1.0f);
            minZ = std::min(minZ, transformedPos.z);
        }
    }

    return minZ;
}

float ModelHelper::getMaxZ(ModelRenderer& renderer) {
    float maxZ = -FLT_MAX;


    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, renderer.getPosition());
    transform = glm::scale(transform, renderer.getScale());


    transform = glm::translate(transform, renderer.getPivot());
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(renderer.getRotation().z), glm::vec3(0, 0, 1));
    transform = glm::translate(transform, -renderer.getPivot());

    std::vector<Mesh>& meshes = renderer.model->meshes;
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];
        for (auto& v : mesh.vertices) {
            glm::vec4 transformedPos = transform * glm::vec4(v.Position, 1.0f);
            maxZ = std::max(maxZ, transformedPos.z);
        }
    }

    return maxZ;
}