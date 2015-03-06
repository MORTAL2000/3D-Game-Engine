#include "UserMaterial.h"

UserMaterial::UserMaterial(const std::string& name)
{
	m_cache["name"] = GenericType(name);
}

UserMaterial::~UserMaterial()
{}

bool UserMaterial::contains(const std::string& key)
{
	return m_cache.find(key) != m_cache.end();
}

GenericType& UserMaterial::operator[](const std::string& key)
{
	return m_cache[key];
}

void UserMaterial::set(const std::string& key, const GenericType& type)
{
	m_cache[key] = type;
}

void UserMaterial::updateValue(const std::string& key, const GenericType& type)
{
	set(key, type);
	process(key, type);
}

void UserMaterial::setCache(const AttributeCache& cache)
{
	m_cache = cache;
}

std::string UserMaterial::getName()
{
	return contains("name") ? m_cache["name"].getString() : "";
}

void UserMaterial::load()
{
	if(!contains("shader")) return;

	std::string vertexShader = m_cache["shader"].getString() + ".vsh";
	std::string fragmentShader = m_cache["shader"].getString() + ".fsh";
	if(!m_shader->load(vertexShader, fragmentShader)) return;

	for(AttributeCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	{
		if(it->first == "name" || it->first == "shader") continue;
		process(it->first, it->second);
	}
}

void UserMaterial::update(FilmCamera* camera, const mat4& modelMatrix)
{
	Material::update(camera, modelMatrix);
}

void UserMaterial::process(const std::string& key, const GenericType& value)
{
	m_shader->bind();
	switch(value.type())
	{
		case GenericType::Type::Integer:
		{
			m_shader->valuei(key.c_str(), value.getInt());
			break;
		}
		case GenericType::Type::Double:
		{
			m_shader->valuef(key.c_str(), (float)value.getDouble());
			break;
		}
		case GenericType::Type::Boolean:
		{
			m_shader->valuei(key.c_str(), (int)value.getBool());
			break;
		}
		case GenericType::Type::String:
		{
			std::string textureKey = value.getString();
			ITexture* texture = TextureLibrary::getInstance().get(textureKey);

			if(texture)
			{
				m_texture_cache[textureKey] = texture;
				m_shader->valuei(key.c_str(), texture->getId());
			}

			break;
		}
		case GenericType::Type::Vector:
		{
			std::vector<double> values = value.getVector();
			if(values.size() == 3)
			{
				m_shader->vec3f(key.c_str(), vec3(values[0], values[1], values[2]));
			}
			else if(values.size() == 4)
			{
				m_shader->vec4f(key.c_str(), vec4(values[0], values[1], values[2], values[3]));
			}
			else
			{
				Console::log("Vector type of unsupported size (%d)", values.size());
			}
			break;
		}
		default:
		{}
	}
	m_shader->unbind();
}
