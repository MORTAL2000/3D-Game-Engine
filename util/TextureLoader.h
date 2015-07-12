#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <vector>
using std::vector;
using std::string;

#include <core/Commons.h>
#include <core/Console.h>
#include <core/FileReader.h>
#include <lodepng.h>

class TextureLoader
{
public:
    static TextureLoader& getInstance();
    vector<unsigned char> load(const string& filename, unsigned int& width, unsigned int& height, unsigned int& bpp);

private:
    TextureLoader();
};

#endif
