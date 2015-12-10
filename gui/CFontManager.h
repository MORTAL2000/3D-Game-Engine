/*
 * Copyright 2015 Alexander Koch
 * File: CFontManager.h
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

#ifndef CFONT_MANAGER_H
#define CFONT_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include "CFont.h"

using string;

class CFontManager
{
public:
	static void initialize();
	static void loadFont(const string&, const string&, const uint16_t);
	static CFont* getFont(const string&);
private:
	CFontManager();
	static std::map<string, std::shared_ptr<CFont>> m_fonts;
};

#endif
