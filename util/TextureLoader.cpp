#include "TextureLoader.h"

#include <lodepng.h>
#include "TgaLoader.h"

namespace TextureLoader
{
	unsigned char* load(const std::string& filename, unsigned int& width, unsigned int& height, unsigned int& bpp)
	{
		unsigned char* data = 0;
		std::string extension = Tokenizer::getFileExtension(filename);
		if(extension != "png" && extension != "tga")
		{
			Console::log("File %s is not supported (type: %s)", filename.c_str(), extension.c_str());
			return data;
		}

		if(extension == "png")
		{
			if(FileReader::usesPackage())
			{
				long size;
				unsigned char* buffer;
				if(!FileReader::read(filename, buffer, size))
				{
					Console::log("Error reading texture from package");
					return data;
				}

				std::vector<unsigned char> out;
				unsigned error = lodepng::decode(out, width, height, buffer, size);
				delete[] buffer;
				if(error)
				{
					Console::log("PNG decode error: %s", lodepng_error_text(error));
					return data;
				}

				data = new unsigned char[size];
				for(auto i = 0; i < size; i++)
				data[i] = out[i];

				out.clear();
			}
			else
			{
				unsigned int error = lodepng_decode32_file(&data, &width, &height, filename.c_str());
				if(error)
				{
					Console::log("Error loading texture '%s'\nError %u : %s", filename.c_str(), error, lodepng_error_text(error));
					return data;
				}
			}

			bpp = GL_RGBA;
		}
		else if(extension == "tga")
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

		return data;
	}
}
