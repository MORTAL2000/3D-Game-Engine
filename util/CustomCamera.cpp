/*
 * Copyright 2015 Alexander Koch
 * File: CustomCamera.cpp
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

#include "CustomCamera.h"

CustomCamera::CustomCamera() {}

void CustomCamera::setModelViewProjection(const mat4& mvp)
{
	m_mvp = mvp;
	m_projectionMatrix = m_mvp;
}

void CustomCamera::update()
{
	// do nothing
}

mat4 CustomCamera::getViewMatrix()
{
	return mat4(1.0);
}

mat4 CustomCamera::getProjectionMatrix()
{
	return m_mvp;
}
