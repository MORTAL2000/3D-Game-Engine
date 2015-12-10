/*
 * Copyright 2015 Alexander Koch
 * File: Commons.h
 * Description: Standard definitions.
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

#ifndef COMMONS_H
#define COMMONS_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG_TO_RAD ((PI * 2) / 360.0)
#define RAD_TO_DEG (1.0 / DEG_TO_RAD)

inline double clamp(double x, double min, double max)
{
	return x < min ? min : x > max ? max : x;
}

#if defined(_WIN32)
	#define __WINDOWS_API__
#elif defined(__APPLE__)
	#define __APPLE_API
#else
	#define __LINUX_API__
#endif

#ifdef __WINDOWS_API__
	#include <glew.h>
	#include <GLFW/glfw3.h>
#endif

#ifdef __LINUX_API__
	#include <glew.h>
	#include <GLFW/glfw3.h>
#endif

#ifdef __WEBGL_API__
	#include <GLES2/gl2.h>
#endif

#ifdef __ANDROID_API__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>

	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
	#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))
#endif

#include <AL/al.h>
#include <AL/alc.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::quat;
using glm::length;
using glm::normalize;
using glm::cross;
using glm::dot;

#endif
