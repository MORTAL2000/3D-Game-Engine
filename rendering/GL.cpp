/*
 * Copyright 2015 Alexander Koch
 * File: GL.cpp
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

#include "GL.h"

namespace gl
{
	Shader m_debug;
	vec4 m_color;
	mat4 m_mvp;

	void init()
	{
		m_debug.load("shader/debug.vsh", "shader/debug.fsh");
		m_color = vec4(1.0);
	}

	void update(const mat4& mvp)
	{
		m_mvp = mvp;
	}

	void setColor(const vec4& color)
	{
		m_color = color;
	}

	void rotate(float angle, const vec3& rotation)
	{
		m_mvp = glm::rotate(m_mvp, angle, rotation);
	}

	void translate(const vec3& position)
	{
		m_mvp = glm::translate(m_mvp, vec3(position.x, position.y, position.z));
	}

	void viewport(const vec4& viewport)
	{
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

	void clearColor(const vec4& color)
	{
		glClearColor(color[0], color[1], color[2], color[3]);
	}

	void clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void begin()
	{
		m_debug.bind();
		m_debug.mat4x4("mvp", m_mvp);
		m_debug.vec4f("color", m_color);
	}

	void end()
	{
		m_debug.unbind();
	}

	void drawLine(const vec2& start, const vec2& end)
	{
		float lineVerts[2*2];
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, lineVerts);
		lineVerts[0] = start.x; lineVerts[1] = start.y;
		lineVerts[2] = end.x; lineVerts[3] = end.y;
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawLine(const vec3& start, const vec3& end)
	{
		float lineVerts[3*2];
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, lineVerts);
		lineVerts[0] = start.x; lineVerts[1] = start.y; lineVerts[2] = start.z;
		lineVerts[3] = end.x; lineVerts[4] = end.y; lineVerts[5] = end.z;
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawWireCube(const vec3& center, const vec3& size)
	{
		vec3 min = center - size * 0.5f;
		vec3 max = center + size * 0.5f;

		drawLine(vec3(min.x, min.y, min.z), vec3(max.x, min.y, min.z));
		drawLine(vec3(max.x, min.y, min.z), vec3(max.x, max.y, min.z));
		drawLine(vec3(max.x, max.y, min.z), vec3(min.x, max.y, min.z));
		drawLine(vec3(min.x, max.y, min.z), vec3(min.x, min.y, min.z));

		drawLine(vec3(min.x, min.y, max.z), vec3(max.x, min.y, max.z));
		drawLine(vec3(max.x, min.y, max.z), vec3(max.x, max.y, max.z));
		drawLine(vec3(max.x, max.y, max.z), vec3(min.x, max.y, max.z));
		drawLine(vec3(min.x, max.y, max.z), vec3(min.x, min.y, max.z));

		drawLine(vec3(min.x, min.y, min.z), vec3(min.x, min.y, max.z));
		drawLine(vec3(min.x, max.y, min.z), vec3(min.x, max.y, max.z));
		drawLine(vec3(max.x, max.y, min.z), vec3(max.x, max.y, max.z));
		drawLine(vec3(max.x, min.y, min.z), vec3(max.x, min.y, max.z));
	}

	void drawVector(const vec3& start, const vec3& end, float head_length, float head_radius)
	{
		const int NUM_SEGMENTS = 32;
		float lineVerts[3*2];
		vec3 coneVerts[NUM_SEGMENTS+2];
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, lineVerts);
		lineVerts[0] = start.x; lineVerts[1] = start.y; lineVerts[2] = start.z;
		lineVerts[3] = end.x; lineVerts[4] = end.y; lineVerts[5] = end.z;
		glDrawArrays(GL_LINES, 0, 2);

	 	vec3 axis = normalize(end - start);
		vec3 temp = dot(axis, vec3(0, 1, 0)) > 0.999f ? cross(axis, vec3(1, 0, 0)) : cross(axis, vec3(0, 1, 0));
		vec3 left = normalize(cross(axis, temp));
		vec3 up = normalize(cross(axis, left));

		glVertexPointer(3, GL_FLOAT, 0, &coneVerts[0].x);
		coneVerts[0] = end + axis * vec3(head_length);
		for(int s = 0; s <= NUM_SEGMENTS; ++s)
		{
			float t = s / (float)NUM_SEGMENTS;
			coneVerts[s+1] = end + left * vec3(head_radius) * vec3(cos(t * 2 * PI)) + up * vec3(head_radius) * vec3(sin(t * 2 * PI));
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS+2);

		glVertexPointer(3, GL_FLOAT, 0, &coneVerts[0].x);
		coneVerts[0] = end;
		for(int s = 0; s <= NUM_SEGMENTS; ++s)
		{
			float t = s / (float)NUM_SEGMENTS;
			coneVerts[s+1] = end - left * vec3(head_radius) * vec3(cos(t * 2 * PI)) + up * vec3(head_radius) * vec3(sin(t * 2 * PI));
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS+2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	mat4 getModelViewProjection()
	{
		return m_mvp;
	}
}
