#pragma once
#include <memory>
#include <d3d11.h>
#include <vector>
#include <dxgi1_2.h>

#include "no_shaders.h"
#include "no_state_saver.h"
#include <string>

template<class T> inline void SAFE_DELETE(T*& p)
{
	if (p)
	{
		delete p;
		p = NULL;
	}
}

template<class T> inline void SAFE_DELETE_ARRAY(T*& p)
{
	if (p)
	{
		delete[] p;
		p = NULL;
	}
}

template<class T> inline void SAFE_RELEASE(T*& p)
{
	if (p)
	{
		p->Release();
		p = NULL;
	}
}

class vec3
{
public:
	float x, y, z;
	vec3();
	vec3(float, float, float);

	vec3 operator+(const vec3& v) {
		return vec3{ x + v.x, y + v.y, z + v.z };
	}

	vec3 operator-(const vec3& v) {
		return vec3{ x - v.x, y - v.y, z - v.z };
	}

	vec3 operator*(const float v) {
		return vec3{ x * v, y * v, z * v };
	}

	vec3 operator/(const float fl) const {
		return vec3(x / fl, y / fl, z / fl);
	}

	inline float distance(vec3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}
};

inline vec3::vec3()
{
	x = y = z = 0.f;
}

inline vec3::vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
class vec4
{
public:
	float x, y, z, w;
	vec4();
	vec4(float, float, float, float);
};

inline vec4::vec4()
{
	x = y = z = w = 0.f;
}

inline vec4::vec4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}
struct COLOR_VERTEX
{
	vec3	Position;
	vec4 Color;
};

typedef unsigned char uchar;

class color
{
public:
	uchar A, R, G, B;

	color()
	{
		A = R = G = B = 0;
	}

	color(uchar A, uchar R, uchar G, uchar B)
	{
		this->A = A;
		this->R = R;
		this->G = G;
		this->B = B;
	}
};

enum _ALIGN : DWORD
{
	ALIGN_LEFT = 0,
	ALIGN_RIGHT,
	ALIGN_CENTER,
};


class no_gui
{
public:

	bool init(IDXGISwapChain* swap_chain);
	void begin_scene();
	void end_scene();
	void reset();

	void fill_rect(float x, float y, float w, float h, color color);
	void draw_circle(float x0, float y0, float radius, const color& color, float thickness = 1.5f);

	void draw_string(std::string str, float fontSize, float x, float y, float r, float g, float b, float a);

	void draw_outlined_rect(const vec4& rect, const color& stroke_color, const color& color);
	void draw_line(float x1, float y1, float x2, float y2, color color);
	void draw_rect(float x, float y, float w, float h, color stroke_color);
	void draw_corner_box(int x, int y, int w, int h, int border_px, color& temp);
	void draw_border(int x, int y, int w, int h, int px, color& temp);
	void draw_cross(float X, float Y, float Width, float Height, const color& d_color);
	void draw_health_bar(int x, int y, float health, float max_health, int w, int h, bool is_shield = false);

	float get_width();
	float get_height();
	HWND get_window();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11InputLayout* m_input_layout;
	ID3D11Buffer* m_pvertex_buffer;
	no_state_saver m_state_saver;
	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11BlendState* m_transparency;
	ID3D11Texture2D* m_back_buffer;

	float m_width, m_height;
	HWND m_hwnd;
	bool m_restore_state;
};
