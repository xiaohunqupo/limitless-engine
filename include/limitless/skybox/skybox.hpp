#pragma once

#include <limitless/loaders/texture_loader.hpp>
#include <limitless/util/filesystem.hpp>

#include <memory>
#include <set>

namespace Limitless::ms {
    class Material;
}

namespace Limitless {
    class Context;
    class Assets;

    class Skybox final {
    private:
        std::shared_ptr<ms::Material> material;
    public:
        explicit Skybox(const std::shared_ptr<ms::Material>& material);
        Skybox(Assets& assets, const fs::path& path, const TextureLoaderFlags& flags = {});

        ~Skybox() = default;

        Skybox(const Skybox&) = delete;
        Skybox(Skybox&&) = delete;

        [[nodiscard]] const auto& getMaterial() const noexcept { return *material; }
        auto& getMaterial() noexcept { return *material; }

        void draw(Context& context, const Assets& assets);
    };
}