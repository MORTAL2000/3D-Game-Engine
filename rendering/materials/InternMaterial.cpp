#include "InternMaterial.h"

InternMaterial::InternMaterial()
{

}

InternMaterial::~InternMaterial()
{

}

void InternMaterial::load(const std::string& vsh, const std::string& fsh)
{
	m_shader->load(vsh, fsh);
}

void InternMaterial::load()
{}

void InternMaterial::update(FilmCamera* camera, const mat4& modelMatrix)
{
	Material::update(camera, modelMatrix);
}
