/*
 * Copyright 2015 Alexander Koch
 * File: TextureLoader.h
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
