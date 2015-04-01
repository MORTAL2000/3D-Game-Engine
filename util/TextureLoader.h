#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <memory>
#include "../core/Commons.h"
#include "../core/Console.h"
#include "../core/FileReader.h"

namespace TextureLoader
{
 	unsigned char* load(const std::string& filename, unsigned int& width, unsigned int& height, unsigned int& bpp);
}

#endif
