#include <limitless/instances/instance.hpp>
#include <limitless/core/uniform/uniform_setter.hpp>
#include <limitless/instances/instance_builder.hpp>

using namespace Limitless;

Instance::Instance(InstanceType _shader_type, const glm::vec3& _position) noexcept
	: id {next_id++}
	, shader_type {_shader_type}
	, position {_position} {
}

Instance::Instance(const Instance& rhs)
    : InstanceAttachment {rhs}
    , id {next_id++}
    , shader_type {rhs.shader_type}
    , final_matrix {rhs.final_matrix}
    , parent {rhs.parent}
    , transformation_matrix {rhs.transformation_matrix}
    , model_matrix {rhs.model_matrix}
    , rotation {rhs.rotation}
    , position {rhs.position}
    , scale {rhs.scale}
    , bounding_box {rhs.bounding_box}
    , shadow_cast {rhs.shadow_cast}
    , outlined {rhs.outlined}
    , hidden {rhs.hidden}
    , done {rhs.done} {
}


void Instance::updateModelMatrix() noexcept {
    const auto translation_matrix = glm::translate(glm::mat4{1.0f}, position);
    const auto rotation_matrix = glm::toMat4(rotation);
    const auto scale_matrix = glm::scale(glm::mat4{1.0f}, scale);

    model_matrix = translation_matrix * rotation_matrix * scale_matrix;
}

void Instance::updateFinalMatrix() noexcept {
	final_matrix = parent * transformation_matrix * model_matrix;
}

void Instance::reveal() noexcept {
    hidden = false;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->reveal();
    }
}

void Instance::hide() noexcept {
    hidden = true;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->hide();
    }
}

bool Instance::isHidden() const noexcept {
	return hidden;
}

void Instance::kill() noexcept {
    done = true;
}

bool Instance::isKilled() const noexcept {
    return done;
}

void Instance::castShadow() noexcept {
	shadow_cast = true;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->castShadow();
    }
}

void Instance::removeShadow() noexcept {
	shadow_cast = false;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->removeShadow();
    }
}

bool Instance::doesCastShadow() const noexcept {
	return shadow_cast;
}

Instance& Instance::setPosition(const glm::vec3& _position) noexcept {
    position = _position;
    return *this;
}

Instance& Instance::setRotation(const glm::quat& _rotation) noexcept {
    rotation = _rotation;
    return *this;
}

Instance& Instance::rotateBy(const glm::quat& _rotation) noexcept {
    rotation = _rotation * rotation;
    return *this;
}

Instance& Instance::setScale(const glm::vec3& _scale) noexcept {
    scale = _scale;
    return *this;
}

Instance& Instance::setTransformation(const glm::mat4& transformation) {
	transformation_matrix = transformation;
	return *this;
}

Instance& Instance::setParent(const glm::mat4& _parent) noexcept {
	parent = _parent;
	return *this;
}

void Instance::draw(Context& ctx, const Assets& assets, ShaderType material_shader_type, ms::Blending blending) {
    draw(ctx, assets, material_shader_type, blending, UniformSetter {});
}

void Instance::update(Context& context, const Camera& camera) {
	// updates current model matrices
	updateModelMatrix();
	updateFinalMatrix();
	updateBoundingBox();

	// propagates current instance values to attachments
    InstanceAttachment::setAttachmentsParent(final_matrix);
    InstanceAttachment::updateAttachments(context, camera);
}

void Instance::removeOutline() noexcept {
	outlined = false;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->removeOutline();
    }
}

void Instance::makeOutlined() noexcept {
	outlined = true;
    for (const auto& [_, attachment]: getAttachments()) {
        attachment->makeOutlined();
    }
}

bool Instance::isOutlined() const noexcept {
	return outlined;
}

Instance::Builder Instance::builder() noexcept {
    return {};
}


