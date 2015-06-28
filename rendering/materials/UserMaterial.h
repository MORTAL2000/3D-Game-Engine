#ifndef USER_MATERIAL_H
#define USER_MATERIAL_H

#include <string>
#include <map>
using std::map;
using std::string;

#include "Material.h"
#include <core/Console.h>
#include <util/GenericType.h>
#include <util/TextureLibrary.h>

typedef map<string, GenericType> AttributeCache;

class UserMaterial : public Material
{
public:
	UserMaterial(const string&);
	~UserMaterial();

	bool contains(const string&);
	GenericType& operator[](const string&);
	void set(const string&, const GenericType&);
	void updateValue(const string&, const GenericType&);
	void setCache(const AttributeCache&);

	string getName();

	virtual void load();
	virtual void update(FilmCamera*, const mat4&);
private:
	AttributeCache m_cache;
	void process(const string&, const GenericType&);
};

#endif
