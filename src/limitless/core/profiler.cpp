#include <limitless/core/profiler.hpp>

#include <limitless/assets.hpp>

using namespace Limitless;

void Profiler::draw(Context& ctx, const Assets& assets) {
    TextInstance text {"text", glm::vec2{0.0f}, assets.fonts.at("nunito")};
    text.setSize(glm::vec2{0.5f});
    glm::vec2 position = {400, 400};
    for (const auto& [name, query] : queries) {
        text.setText(name + " " + std::to_string(query.getDuration()));
        text.setPosition(position);
        text.draw(ctx, assets);

        position -= glm::vec2{0.0f, 0.1f};
    }
}

TimeQuery& Profiler::operator[] (const std::string& name) {
    return queries[name];
}

ProfilerScope::ProfilerScope(const std::string& name)
    : query {profiler[name]} {
    query.start();
}

ProfilerScope::~ProfilerScope() {
    query.stop();
    query.calculate();
}
