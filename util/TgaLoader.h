// Copyright(c) Alexander Koch 2014, 12.11.2014

#ifndef TGA_LOADER_H
#define TGA_LOADER_H

namespace TGA
{

struct TGA
{
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	unsigned int memory;
 	vector<unsigned char> data;

	TGA()
	{}
};

bool load(const std::string& filename, TGA* tga)
{
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file)
	{
		return false;
	}

	unsigned char compressed = 0;
	unsigned char uncompressed[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
	unsigned char signature[12];
	unsigned char header[6];
	if(fread(signature, 1, sizeof(signature), file) != sizeof(signature))
	{
		fclose(file);
		return false;
	}

	if(std::memcmp(uncompressed, signature, sizeof(uncompressed)) != 0)
	{
		compressed = 1;
	}

	if(fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		fclose(file);
		return false;
	}

	unsigned int width = header[1] * 256 + header[0];
	unsigned int height = header[3] * 256 + header[2];

	if(width < 1 || height < 1 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		return false;
	}

	vector<unsigned char> data;
	unsigned int bpp = header[4];
	unsigned int bytesPerPixel = bpp / 8;
	unsigned int memory = width * height * bytesPerPixel;
	data.reserve(memory);

	if(compressed)
	{
		unsigned int count = width * height;
		unsigned int current = 0;
		unsigned int current_byte = 0;
		unsigned char buffer[bytesPerPixel];

		do
		{
			unsigned char chunk = 0;
			if(fread(&chunk, sizeof(unsigned char), 1, file) == 0)
			{
				fclose(file);
				return false;
			}

			if(chunk < 128)
			{
				chunk++;
				for(short counter = 0; counter < chunk; counter++)
				{
					if(fread(buffer, 1, bytesPerPixel, file) != bytesPerPixel)
					{
						fclose(file);
						return false;
					}

					data[current_byte] = buffer[2];
					data[current_byte+1] = buffer[1];
					data[current_byte+2] = buffer[0];

					if(bytesPerPixel == 4)
					{
						data[current_byte+3] = buffer[3];
					}

					current_byte += bytesPerPixel;
					current++;

					if(current > count)
					{
						fclose(file);
						return false;
					}
				}
			}
			else
			{
				chunk -= 127;
				if(fread(buffer, 1, bytesPerPixel, file) != bytesPerPixel)
				{
					fclose(file);
					return false;
				}

				for(short counter = 0; counter < chunk; counter++)
				{
					data[current_byte] = buffer[2];
					data[current_byte+1] = buffer[1];
					data[current_byte+2] = buffer[0];

					if(bytesPerPixel == 4)
					{
						data[current_byte+3] = buffer[3];
					}

					current_byte += bytesPerPixel;
					current++;
				}

				if(current > count)
				{
					fclose(file);
					return false;
				}
			}

		} while(current < count);
	}
	else
	{
		if(fread(data.data(), 1, memory, file) != memory)
		{
			fclose(file);
			return false;
		}
	}

	fclose(file);

	unsigned int temp;
	for(unsigned int i = 0; i < memory; i += bytesPerPixel)
	{
		temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}

	tga->width = width;
	tga->height = height;
	tga->bpp = bpp;
	tga->memory = memory;
	tga->data = data;

	return true;
}

}

#endif
