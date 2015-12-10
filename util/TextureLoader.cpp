#include "TextureLoader.h"
#include "TgaLoader.h"

TextureLoader::TextureLoader() {}

TextureLoader& TextureLoader::getInstance()
{
	static TextureLoader instance;
	return instance;
}

vector<unsigned char> TextureLoader::load(const string& filename, unsigned int& width, unsigned int& height, unsigned int& bpp)
{
	vector<unsigned char> data;
	string ext = Tokenizer::getFileExtension(filename);

	if(ext == "png")
	{
		bpp = GL_RGBA;
		if(FileReader::usesPackage())
		{
			long size;
			unsigned char* buffer;
			if(!FileReader::read(filename, buffer, size))
			{
				Console::log("Error reading texture from package");
				return data;
			}

			unsigned error = lodepng::decode(data, width, height, buffer, size);
			delete[] buffer;
			if(error)
			{
				Console::log("PNG decode error: %s", lodepng_error_text(error));
				return data;
			}
		}
		else
		{
			unsigned error = lodepng::decode(data, width, height, filename);
			if(error)
			{
				Console::log("Error loading texture '%s'\nError %u : %s", filename.c_str(), error, lodepng_error_text(error));
				return data;
			}
		}
	}
	else if(ext == "tga")
	{
		TGA::TGA tga;
		if(!TGA::load(filename.c_str(), &tga))
		{
			Console::log("Error loading texture '%s'", filename.c_str());
			return data;
		}

		data = tga.data;
		width = tga.width;
		height = tga.height;
		int bytesPerPixel = tga.bpp/8;
		bpp = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;
	}
	else
	{
		Console::log("File %s is not supported (type: %s)", filename.c_str(), ext.c_str());
	}

	return data;
}
