#include "PostComposer.h"

PostComposer::PostComposer() :
	doSSAO(false), doDOF(false), doLensFlares(false), doTonemapping(false), focus(false), shaded(false), active(true)
{}

PostComposer::~PostComposer()
{
	dust.clear();
}

void PostComposer::load(FilmCamera& camera)
{
	dimension = camera.getViewport();

	mainBuffer = std::make_shared<Framebuffer>();
	readBuffer = std::make_shared<Framebuffer>();
	writeBuffer = std::make_shared<Framebuffer>();

	mainBuffer->initialize(dimension.x, dimension.y);
	readBuffer->initialize(dimension.x, dimension.y);
	writeBuffer->initialize(dimension.x, dimension.y);
	dust.load("resources/textures/lens.png");

	quad.load();
	ortho = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	if(base.load("resources/postprocessing/quad.vsh", "resources/postprocessing/base.fsh")) {
		base.bind();
		base.mat4x4("mvp", ortho);
		base.unbind();
	}

	if(ssao.load("resources/postprocessing/quad.vsh", "resources/postprocessing/ssao.fsh")) {
		ssao.bind();
		ssao.mat4x4("mvp", ortho);
		ssao.vec2f("dimension", dimension);
		ssao.valuei("samples", Property("ssao_samples"));
		ssao.valuei("rings", Property("ssao_rings"));
		ssao.vec2f("clipping", camera.getClipping());
		ssao.unbind();
	}

	if(dof.load("resources/postprocessing/quad.vsh", "resources/postprocessing/dof.fsh")) {
		dof.bind();
		dof.mat4x4("mvp", ortho);
		dof.vec2f("dimension", dimension);
		dof.valuef("focalDepth", 5.99);
		dof.valuef("focalLength", 20.0);
		dof.valuef("fstop", 1);
		dof.valuei("showFocus", 0);
		dof.vec2f("clipping", camera.getClipping());
		dof.unbind();
	}

	if(tonemapper.load("resources/postprocessing/quad.vsh", "resources/postprocessing/tonemapper.fsh")) {
		tonemapper.bind();
		tonemapper.mat4x4("mvp", ortho);
		tonemapper.vec2f("inverseTextureSize", vec2(1/camera.getViewport().x, 1/camera.getViewport().y));
		tonemapper.unbind();
	}

	if(flare.load("resources/postprocessing/quad.vsh", "resources/postprocessing/flare.fsh")) {
		flare.bind();
		flare.mat4x4("mvp", ortho);
		flare.unbind();
	}

	if(flareCompose.load("resources/postprocessing/quad.vsh", "resources/postprocessing/flareCompose.fsh")) {
		flareCompose.bind();
		flareCompose.mat4x4("mvp", ortho);
		flareCompose.vec2f("textureDimension", vec2(dust.getWidth(), dust.getHeight()));
		flareCompose.unbind();
	}

	if(bloom.load("resources/postprocessing/quad.vsh", "resources/postprocessing/bloom.fsh")) {
		bloom.bind();
		bloom.mat4x4("mvp", ortho);
		bloom.unbind();
	}

	shaded = Property("postprocessing");
	doSSAO = Property("ssao_enabled");
	doDOF = Property("dof_enabled");
	doLensFlares = Property("lens_flares_enabled");
	doTonemapping = Property("tonemapping_enabled");
}

void PostComposer::bind()
{
	if(active) mainBuffer->bind();
}

void PostComposer::unbind()
{
	if(active) mainBuffer->unbind();
}

void PostComposer::setActive(bool p_active)
{
	active = p_active;
}

bool PostComposer::isActive()
{
	return active;
}

void PostComposer::render()
{
	if(active)
	{
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		if(shaded)
		{
			compose();

			if(doSSAO) passSSAO();
			if(doDOF) passDoF();
			if(doLensFlares) passLensFlares();
			//passBloom();
			if(doTonemapping) passTonemap();

			passFinalRender();
		}
		else
		{
			mainBuffer->bindTexture(0, GL_TEXTURE0);
			quad.render(&base);
			mainBuffer->unbindTexture();
		}
	}
}

void PostComposer::compose()
{
	writeBuffer->bind();

	mainBuffer->bindTexture(0, GL_TEXTURE0);
	quad.render(&base);
	mainBuffer->unbindTexture();

	writeBuffer->unbind();
	swapBuffers();
}

void PostComposer::passSSAO()
{
	writeBuffer->bind();

	mainBuffer->bindTexture(1, GL_TEXTURE1);
	readBuffer->bindTexture(0, GL_TEXTURE0);

	ssao.bind();
	ssao.valuei("renderedTexture", 0);
	ssao.valuei("depthTexture", 1);
	ssao.unbind();

	quad.render(&ssao);
	readBuffer->unbindTexture();

	writeBuffer->unbind();
	swapBuffers();
}

void PostComposer::passDoF()
{
	writeBuffer->bind();

	mainBuffer->bindTexture(1, GL_TEXTURE1);
	readBuffer->bindTexture(0, GL_TEXTURE0);

	dof.bind();
	dof.valuei("renderedTexture", 0);
	dof.valuei("depthTexture", 1);
	dof.unbind();

	quad.render(&dof);
	readBuffer->unbindTexture();

	writeBuffer->unbind();
	swapBuffers();
}

void PostComposer::passTonemap()
{
	writeBuffer->bind();
	readBuffer->bindTexture(0, GL_TEXTURE0);

	tonemapper.bind();
	tonemapper.valuei("renderedTexture", 0);
	tonemapper.unbind();

	quad.render(&tonemapper);

	readBuffer->unbindTexture();
	writeBuffer->unbind();
	swapBuffers();
}

/* TODO: fix main buffer issues */
void PostComposer::passLensFlares()
{
	mainBuffer->bind();
	readBuffer->bindTexture(0, GL_TEXTURE0);
	quad.render(&flare);
	readBuffer->unbindTexture();
	mainBuffer->unbind();

	dust.bind(GL_TEXTURE2);
	writeBuffer->bind();
	mainBuffer->bindTexture(0, GL_TEXTURE1);
	readBuffer->bindTexture(0, GL_TEXTURE0);

	flareCompose.bind();
	flareCompose.valuei("renderedTexture", 0);
	flareCompose.valuei("flareTexture", 1);
	flareCompose.valuei("dust", 2);
	flareCompose.unbind();

	quad.render(&flareCompose);
	readBuffer->unbindTexture();
	writeBuffer->unbind();

	swapBuffers();
}

void PostComposer::passBloom()
{
	writeBuffer->bind();
	readBuffer->bindTexture(0, GL_TEXTURE0);

	bloom.bind();
	bloom.valuei("renderedTexture", 0);
	bloom.unbind();

	quad.render(&bloom);
	readBuffer->unbindTexture();
	writeBuffer->unbind();
	swapBuffers();
}

void PostComposer::passFinalRender()
{
	readBuffer->bindTexture(0, GL_TEXTURE0);
	quad.render(&base);
	readBuffer->unbindTexture();
}

void PostComposer::passCustom()
{
	quad.render(&base);
}

void PostComposer::swapBuffers()
{
	readBuffer.swap(writeBuffer);
}
