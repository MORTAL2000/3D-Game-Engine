/*
 * Copyright 2015 Alexander Koch
 * File: TextureLoader.cpp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TextureLoader.h"
#include "TgaLoader.h"

TextureLoader::TextureLoader() {}

TextureLoader& TextureLoader::getInstance() {
	static TextureLoader instance;
	return instance;
}

vector<unsigned char> TextureLoader::load(const string& filename, unsigned int& width, unsigned int& height, unsigned int& bpp) {
	vector<unsigned char> data;
	string ext = Tokenizer::getFileExtension(filename);

	if(ext == "png") {
		bpp = GL_RGBA;
		unsigned error = lodepng::decode(data, width, height, filename);
		if(error) {
			Console::log("Error loading texture '%s'\nError %u : %s", filename.c_str(), error, lodepng_error_text(error));
			return data;
		}
	}
	else if(ext == "tga") {
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
	else {
		Console::log("File %s is not supported (type: %s)", filename.c_str(), ext.c_str());
	}

	return data;
}
