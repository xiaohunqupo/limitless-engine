#include "catch_amalgamated.hpp"

#include <core/context.hpp>
#include <material_system/material_loader.hpp>
#include <material_system/material_builder.hpp>
#include <iostream>
#include <asset_loader.hpp>
#include <particle_system/effect_builder.hpp>


using namespace LimitlessEngine;

class FakeBackend {
private:
    Context ctx;
public:
    FakeBackend() : ctx{"test", {1, 1}, {{WindowHint::Visible, false}}} {

    }
};

TEST_CASE("material loader") {
    FakeBackend fake;

    MaterialLoader loader;

std::cout << "dir = " << MATERIAL_DIR << std::endl;
//    MaterialBuilder builder;
//    auto mat = builder  .create("test")
//                        .add(PropertyType::Color, {1.0f, 0.5f, 0.5f, 1.0f})
//                        .setBlending(Blending::Additive)
//                        .build();
//
//    loader.save("test");

    loader.load("test");
}

TEST_CASE("effect loader") {
    FakeBackend fake;

    EffectBuilder eb;
    auto effect1 = eb.create("test_effect1")
            .createEmitter<SpriteEmitter>("test")
//            .addModule<InitialVelocity>(EmitterModuleType::InitialVelocity, new RangeDistribution{glm::vec3{-5.0f}, glm::vec3{5.0f}})
            .addModule<Lifetime>(EmitterModuleType::Lifetime, new RangeDistribution(0.2f, 0.5f))
//            .addModule<InitialSize>(EmitterModuleType::InitialSize, new RangeDistribution(0.0f, 50.0f))
//            .addModule<SizeByLife>(EmitterModuleType::SizeByLife, new RangeDistribution(0.0f, 100.0f), -1.0f)
            .setMaterial()
            .setSpawnMode(EmitterSpawn::Mode::Burst)
            .setBurstCount(std::make_unique<ConstDistribution<uint32_t>>(1000))
    .setMaxCount(1000)
            .setSpawnRate(1.0f)
            .build();
}