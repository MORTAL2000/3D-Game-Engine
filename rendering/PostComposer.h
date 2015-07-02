#ifndef POST_COMPOSER_H
#define POST_COMPOSER_H

#include <memory>

#include "Quad.h"
#include <core/Property.h>
#include <util/Framebuffer.h>
#include <util/FilmCamera.h>
#include <util/Shader.h>
#include <util/Texture2D.h>

/**
 *  A class for post-processing
 */
class PostComposer
{
private:
    vec2 dimension;
    bool shaded;
    bool active;

    // Framebuffers
    std::shared_ptr<Framebuffer> mainBuffer;
    std::shared_ptr<Framebuffer> readBuffer;
    std::shared_ptr<Framebuffer> writeBuffer;

    // Shaders
    Shader base;
    Shader dof;
    Shader tonemapper;
    Shader ssao;
    Shader flare, flareCompose;
    Shader bloom;

    // Shaders config
    bool doSSAO;
    bool doDOF;
    bool doLensFlares;
    bool doTonemapping;

    // Dummy texture
    Texture2D dust;

    // Utility
    Quad quad;
    mat4 ortho;
    bool focus;

    // Passes
    void compose();
    void passSSAO();
    void passDoF();
    void passMotionBlur();
    void passTonemap();
    void passLensFlares();
    void passBloom();
    void passFinalRender();

    // Swap the 3 framebuffers
    void swapBuffers();

public:
    PostComposer();
    ~PostComposer();

    void load(FilmCamera&);

    void bind();
    void unbind();

    void setActive(bool);
    bool isActive();

    void render();
    void passCustom();
};

#endif
