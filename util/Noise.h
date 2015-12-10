/*
 * Copyright 2015 Alexander Koch
 * File: Noise.h
 * Notice: Algorithm by Ken Perlin.
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

#ifndef NOISE_H
#define NOISE_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define F2 0.3660254037844386f
#define G2 0.21132486540518713f
#define F3 (1.0f / 3.0f)
#define G3 (1.0f / 6.0f)

void seed(unsigned int);
float noise2(float, float);
float simplex2(float, float, float, float, float);

#endif
