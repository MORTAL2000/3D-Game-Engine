/*
 * Copyright 2015 Alexander Koch
 * File: FilmCamera.cpp
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

#include "FilmCamera.h"

FilmCamera::FilmCamera() :
	m_fovy(45), m_nearPlane(0.1), m_farPlane(10000), m_velocity(0.3),
	m_pitchSensitivity(0.1), m_yawSensitivity(0.1), m_flight(false)
{
	setPosition(vec3(0, 0, -10));
	setUpVector(vec3(0, 1, 0));
}

void FilmCamera::setFOV(float fovy)
{
	m_fovy = fovy;
}

void FilmCamera::setViewport(float width, float height)
{
	m_viewport = vec2(width, height);
}

void FilmCamera::setViewport(const vec2& viewport)
{
	m_viewport = viewport;
}

void FilmCamera::setClipping(float nearPlane, float farPlane)
{
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

void FilmCamera::setClipping(const vec2& clipping)
{
	setClipping(clipping.x, clipping.y);
}

void FilmCamera::setPosition(const vec3& position)
{
	m_position = position;
}

void FilmCamera::setTarget(const vec3& target)
{
	m_target = glm::normalize(target - m_position);
}

void FilmCamera::setTargetRaw(const vec3& target)
{
	m_target = target;
}

void FilmCamera::setUpVector(const vec3& up)
{
	m_upVector = glm::normalize(up);
}

float FilmCamera::getFOV()
{
	return m_fovy;
}

float FilmCamera::getAspectRatio()
{
	return (float)m_viewport.x/m_viewport.y;
}

vec2 FilmCamera::getViewport()
{
	return m_viewport;
}

vec2 FilmCamera::getClipping()
{
	return vec2(m_nearPlane, m_farPlane);
}

vec3 FilmCamera::getPosition()
{
	return m_position;
}

vec3 FilmCamera::getTarget()
{
	return m_target;
}

vec3 FilmCamera::getOldPosition()
{
	return m_oldPosition;
}

mat4 FilmCamera::getViewMatrix()
{
	return m_viewMatrix;
}

mat4 FilmCamera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

void FilmCamera::setFlightMode(bool state)
{
	m_flight = state;
}
bool FilmCamera::getFlightMode()
{
	return m_flight;
}

void FilmCamera::move(const MovementType& type)
{
	m_oldPosition = m_position;

#define FLYMODE
#ifdef FLYMODE
	vec3 dir(0.0);
	if(type == FORWARD) dir += m_direction;
	if(type == BACKWARD) dir -= m_direction;
	if(type == STRAFE_LEFT) dir -= glm::cross(m_direction, m_upVector);
	if(type == STRAFE_RIGHT) dir += glm::cross(m_direction, m_upVector);
	if(type == UP) dir += m_upVector;
	if(type == DOWN) dir -= m_upVector;

	if(length(dir) > 0) normalize(dir);
	dir *= m_velocity;
	m_position += dir;
	m_target += dir;
#endif
}

void FilmCamera::rotate(double x, double y)
{
	float dx = (x - m_viewport.x/2+1) * m_yawSensitivity;
	float dy = (y - m_viewport.y/2) * m_pitchSensitivity;

	m_rotation += vec3(dy, dx, 0.0f) * -0.025f;
	m_rotation.x = (float)clamp(m_rotation.x, -90, 90);
}

void FilmCamera::update()
{
	// Erstellen der Projektionsmatrix
	m_projectionMatrix = glm::perspective(m_fovy, getAspectRatio(), m_nearPlane, m_farPlane);

	if(m_flight)
	{

		// Berechnung der neuen Blickrichtung
		m_direction = glm::normalize(m_target - m_position);
		vec3 axis = glm::cross(m_direction, m_upVector);
		quat qPitch = glm::angleAxis(m_rotation.x, axis);
		quat qYaw = glm::angleAxis(m_rotation.y, m_upVector);

		quat temp = glm::normalize(glm::cross(qPitch, qYaw));
		m_direction = glm::rotate(temp, m_direction);
		m_target = m_position + m_direction;

		m_rotation.x *= 0.5;
		m_rotation.y *= 0.5;
	}

	// Sichtmatrix berechnen
	m_viewMatrix = glm::lookAt(m_position, m_target, m_upVector);

	// View-Frustum erstellen
	m_frustum.calculate(m_projectionMatrix * m_viewMatrix);
}

Frustum FilmCamera::getFrustum()
{
	return m_frustum;
}

bool FilmCamera::boxInFrustum(const BoundingBox& bbox)
{
	return m_frustum.boxInFrustum(bbox);
}
