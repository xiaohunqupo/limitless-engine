#include <limitless/instances/model_instance.hpp>

#include <limitless/models/model.hpp>
#include <limitless/models/elementary_model.hpp>
#include <limitless/core/shader/shader_program.hpp>
#include <stdexcept>
#include <utility>

using namespace Limitless;

ModelInstance::ModelInstance(InstanceType shader, decltype(model) _model, const glm::vec3& position)
    : Instance(shader, position)
    , model {std::move(_model)} {
    try {
        auto& simple_model = dynamic_cast<Model&>(*model);
        auto& model_meshes = simple_model.getMeshes();
        auto& model_mats = simple_model.getMaterials();

        for (uint32_t i = 0; i < model_meshes.size(); ++i) {
            meshes.emplace(model_meshes[i]->getName(), MeshInstance{model_meshes[i], model_mats[i]});
        }
    } catch (...) {
        throw std::logic_error{"Wrong model for ModelInstance"};
    }
}

ModelInstance::ModelInstance(decltype(model) _model, const glm::vec3& _position)
    : ModelInstance {InstanceType::Model, std::move(_model), _position} {
}

ModelInstance::ModelInstance(decltype(model) _model, std::shared_ptr<ms::Material> material, const glm::vec3& position)
    : Instance(InstanceType::Model, position)
    , model {std::move(_model)} {
    try {
        auto& elementary_model = dynamic_cast<ElementaryModel&>(*model);

        meshes.emplace(elementary_model.getMesh()->getName(), MeshInstance{elementary_model.getMesh(), material});
    } catch (...) {
        throw std::logic_error{"Wrong model for ModelInstance"};
    }
}

void ModelInstance::draw(Context& ctx, const Assets& assets, ShaderType pass, ms::Blending blending, const UniformSetter& uniform_setter) {
    if (hidden) {
        return;
    }

    const_cast<UniformSetter&>(uniform_setter).add([&] (ShaderProgram& shader) {
       shader.setUniform("outline", outlined ? 1.0f : 0.0f);
    });

    for (auto& [name, mesh] : meshes) {
        mesh.draw(ctx, assets, pass, shader_type, final_matrix, blending, uniform_setter);
    }
}

MeshInstance& ModelInstance::operator[](const std::string& mesh) {
    try {
        return meshes.at(mesh);
    } catch (...) {
        throw no_such_mesh("with name " + mesh);
    }
}

MeshInstance& ModelInstance::operator[](uint32_t index) {
    if (index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(index));
    }

    return std::next(meshes.begin(), index)->second;
}

std::unique_ptr<Instance> ModelInstance::clone() noexcept {
    return std::make_unique<ModelInstance>(*this);
}

void ModelInstance::updateBoundingBox() noexcept {
    bounding_box.center = glm::vec4{position, 1.0f} + glm::vec4{model->getBoundingBox().center, 1.0f} * final_matrix;
    bounding_box.size = glm::vec4{model->getBoundingBox().size, 1.0f} * final_matrix;
}

void ModelInstance::update(Context& context, const Camera& camera) {
	Instance::update(context, camera);

	for (auto& [_, mesh] : meshes) {
		mesh.update();
	}
}

void ModelInstance::changeMaterial(uint32_t mesh_index, const std::shared_ptr<ms::Material> &material) {
    if (mesh_index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(mesh_index));
    }

    std::next(meshes.begin(), mesh_index)->second.changeMaterial(material);
}

void ModelInstance::changeMaterial(const std::string& mesh_name, const std::shared_ptr<ms::Material> &material) {
    try {
        meshes.at(mesh_name).changeMaterial(material);
    } catch (...) {
        throw no_such_mesh("with name " + mesh_name);
    }
}

void ModelInstance::changeMaterials(const std::shared_ptr<ms::Material> &material) {
    for (auto& [_, mesh] : meshes) {
        mesh.changeMaterial(material);
    }
}

void ModelInstance::changeBaseMaterial(uint32_t mesh_index, const std::shared_ptr<ms::Material> &material) {
    if (mesh_index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(mesh_index));
    }

    std::next(meshes.begin(), mesh_index)->second.changeBaseMaterial(material);
}

void ModelInstance::changeBaseMaterial(const std::string& mesh_name, const std::shared_ptr<ms::Material> &material) {
    try {
        meshes.at(mesh_name).changeBaseMaterial(material);
    } catch (...) {
        throw no_such_mesh("with name " + mesh_name);
    }
}

void ModelInstance::changeBaseMaterials(const std::shared_ptr<ms::Material>& material) {
    for (auto& [_, mesh]: meshes) {
        mesh.changeBaseMaterial(material);
    }
}

void ModelInstance::resetMaterial(uint32_t mesh_index) {
    if (mesh_index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(mesh_index));
    }

    std::next(meshes.begin(), mesh_index)->second.reset();
}

void ModelInstance::resetMaterial(const std::string& mesh_name) {
    try {
        meshes.at(mesh_name).reset();
    } catch (...) {
        throw no_such_mesh("with name " + mesh_name);
    }
}

void ModelInstance::resetMaterials() {
    for (auto& [_, mesh] : meshes) {
        mesh.reset();
    }
}

MeshInstance& ModelInstance::getMeshInstance(const std::string &mesh) {
    try {
        return meshes.at(mesh);
    } catch (...) {
        throw no_such_mesh("with name " + mesh);
    }
}

MeshInstance& ModelInstance::getMeshInstance(uint32_t index) {
    if (index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(index));
    }

    return std::next(meshes.begin(), index)->second;
}

const std::shared_ptr<ms::Material> &ModelInstance::getMaterial(uint32_t mesh_index) {
    if (mesh_index >= meshes.size()) {
        throw no_such_mesh("with index " + std::to_string(mesh_index));
    }

    return std::next(meshes.begin(), mesh_index)->second.getMaterial();
}

const std::shared_ptr<ms::Material> &ModelInstance::getMaterial(const std::string& mesh_name) {
    try {
        return meshes.at(mesh_name).getMaterial();
    } catch (...) {
        throw no_such_mesh("with name " + mesh_name);
    }
}
