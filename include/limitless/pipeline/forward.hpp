#pragma once

#include <limitless/pipeline/pipeline.hpp>

namespace Limitless {
    class ContextEventObserver;
    class RenderSettings;

    class Forward final : public Pipeline {
    private:
        void create(ContextEventObserver& ctx, Scene& scene, const RenderSettings& settings);
    public:
        explicit Forward(ContextEventObserver& ctx, Scene& scene, const RenderSettings& settings);
        ~Forward() override = default;

        void update(ContextEventObserver& ctx, Scene& scene, const RenderSettings& settings) override;
    };
}