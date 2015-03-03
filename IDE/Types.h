#ifndef TYPES_H
#define TYPES_H

struct rect_t
{
	rect_t(float x, float y, int w, int h) : x(x), y(y), w(w), h(h) {}
	rect_t() : x(0.f), y(0.f), w(0), h(0) {}

	float x, y;
	int w, h;
};

struct vertex2_t
{
	vec2 position;
	vec2 texcoord;

	vertex2_t(float x, float y, float u, float v) : position(x,y), texcoord(u,v) {}
	vertex2_t(const vec2& position, const vec2& texcoord) : position(position), texcoord(texcoord) {}
	vertex2_t() : position(0.0), texcoord(0.0) {}
};

#endif
