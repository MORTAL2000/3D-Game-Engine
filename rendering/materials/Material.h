#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <string>
#include <memory>

#include "../../core/Commons.h"
#include "../../util/FilmCamera.h"
#include "../../util/Shader.h"
#include "../../util/Texture2D.h"

class Material
{
public:
    Material();
    ~Material();

    virtual void load();
    virtual void update(FilmCamera*, const mat4&);

    Shader* getShader();

    void bindTextures();
    void unbindTextures();
protected:
    std::shared_ptr<Shader> m_shader;
    std::map<std::string, Texture2D*> m_texture_cache;
};

#endif
