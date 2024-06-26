#include <limitless/pipeline/forward/postprocessing_pass.hpp>

using namespace Limitless;

PostEffectsPass::PostEffectsPass(Pipeline& pipeline, Context& context)
    : PipelinePass(pipeline)
    , postprocess {context.getSize()} {
}

void PostEffectsPass::draw([[maybe_unused]] Instances& instances, [[maybe_unused]] Context& ctx, [[maybe_unused]] const Assets& assets, [[maybe_unused]] const Camera& camera, [[maybe_unused]] UniformSetter& setter) {
    //TODO:
    //    postprocess.process(ctx, assets, dynamic_cast<const Framebuffer&>(prev_pass->getTarget()));
}

PostEffectsPass::PostEffectsPass(Pipeline& pipeline, Context& context, RenderTarget& target)
	: PipelinePass(pipeline)
	, postprocess {context.getSize(), target} {

}
