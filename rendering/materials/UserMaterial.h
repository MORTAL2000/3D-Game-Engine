#ifndef USER_MATERIAL_H
#define USER_MATERIAL_H

#include <string>
#include <map>

#include "Material.h"
#include "../../util/GenericType.h"
#include "../../util/TextureLibrary.h"
typedef std::map<std::string, GenericType> AttributeCache;

class UserMaterial : public Material
{
public:
	UserMaterial(const std::string&);
	~UserMaterial();

	bool contains(const std::string&);
	GenericType& operator[](const std::string&);
	void set(const std::string&, const GenericType&);
	void updateValue(const std::string&, const GenericType&);
	void setCache(const AttributeCache&);

	std::string getName();

	virtual void load();
	virtual void update(FilmCamera*, const mat4&);
private:
	AttributeCache m_cache;
	void process(const std::string&, const GenericType&);
};

#endif
