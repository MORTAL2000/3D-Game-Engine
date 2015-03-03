#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../core/Commons.h"
#include "Mesh.h"
#include "materials/Material.h"

class MeshRenderer : public Mesh
{
public:
	MeshRenderer();
	~MeshRenderer();

	void setMaterial(Material*);
	Material* getMaterial();

	void setPosition(const vec3&);
	void setRotation(const vec3&);
	void setRotation(const quat&);
	void setScale(const vec3&);

	vec3 getPosition() const;
	quat getRotation() const;
	vec3 getScale() const;

	void setCullFace(GLenum);
	void setUseDepthMask(bool);
	GLenum getCullFace();
	bool getDepthMaskUsage();

	mat4 getModelMatrix();

	void update(FilmCamera*);
	void update(const mat4&, FilmCamera*);
	void render(Shader*);
	void render();

	int getTriangleCount();
private:
	Material* m_material;

	vec3 m_position;
	quat m_rotation;
	vec3 m_scale;

	GLenum m_cullFace;
	bool m_useDepthMask;
};

#endif
