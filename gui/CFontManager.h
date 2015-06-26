#ifndef CFONT_MANAGER_H
#define CFONT_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include "CFont.h"

class CFontManager
{
public:
	static void initialize();
	static void loadFont(const std::string&, const std::string&, const uint16_t);
	static CFont* getFont(const std::string&);
private:
	CFontManager();
	~CFontManager();
	static std::map<std::string, std::shared_ptr<CFont>> m_fonts;
};

#endif
