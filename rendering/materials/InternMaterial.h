#ifndef INTERN_MATERIAL_H
#define INTERN_MATERIAL_H

#include "Material.h"

class InternMaterial : public Material
{
public:
	InternMaterial();
	~InternMaterial();

	void load(const std::string&, const std::string&);

	virtual void load();
	virtual void update(FilmCamera*, const mat4&);
};

#endif
