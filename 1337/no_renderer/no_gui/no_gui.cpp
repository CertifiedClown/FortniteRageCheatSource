#include "no_gui.h"

bool no_gui::init(IDXGISwapChain* swapchain)
{
	HRESULT hr;
	m_swap_chain = swapchain;
	if (!m_swap_chain)
		return false;

	m_swap_chain->GetDevice(__uuidof(m_device), (void**)&m_device);
	m_device->GetImmediateContext(&m_device_context);
	hr = m_device->CreateVertexShader(vs_blob, sizeof(vs_blob), NULL, &m_vertex_shader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), vs_blob, sizeof(vs_blob), &m_input_layout);
	hr = m_device->CreatePixelShader(ps_blob, sizeof(ps_blob), NULL, &m_pixel_shader);

	D3D11_BUFFER_DESC buffer_desc;

	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = 4 * sizeof(COLOR_VERTEX);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;

	hr = m_device->CreateBuffer(&buffer_desc, NULL, &m_pvertex_buffer);
	if (FAILED(hr))
		return false;

	D3D11_BLEND_DESC blend_state_desc;
	ZeroMemory(&blend_state_desc, sizeof(blend_state_desc));
	blend_state_desc.AlphaToCoverageEnable = false;
	blend_state_desc.RenderTarget[0].BlendEnable = true;
	blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_device->CreateBlendState(&blend_state_desc, &m_transparency);
	if (FAILED(hr))
		return false;

	memset((void*)vs_blob, NULL, sizeof(vs_blob));
	memset((void*)ps_blob, NULL, sizeof(ps_blob));
	return true;
}

void no_gui::reset()
{
	init(m_swap_chain);
}

void no_gui::fill_rect(float x, float y, float w, float h, color color)
{
	if (!m_device_context)
		return;

	int a = color.A & 0xff;
	int r = color.R & 0xff;
	int g = color.G & 0xff;
	int b = color.B & 0xff;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	memset(&swap_chain_desc, 0, sizeof(swap_chain_desc));

	m_swap_chain->GetDesc(&swap_chain_desc);
	float width = swap_chain_desc.BufferDesc.Width;
	float height = swap_chain_desc.BufferDesc.Height;

	UINT numViewports = 1;
	D3D11_VIEWPORT vp = { 0, 0, width, height, 1, 1 };
	m_device_context->RSSetViewports(numViewports, &vp);

	float x0 = x;
	float y0 = y;
	float x1 = x + w;
	float y1 = y + h;

	float xx0 = 2.0f * (x0 - 0.5f) / vp.Width - 1.0f;
	float yy0 = 1.0f - 2.0f * (y0 - 0.5f) / vp.Height;
	float xx1 = 2.0f * (x1 - 0.5f) / vp.Width - 1.0f;
	float yy1 = 1.0f - 2.0f * (y1 - 0.5f) / vp.Height;

	COLOR_VERTEX* v = NULL;
	D3D11_MAPPED_SUBRESOURCE mapData;

	if (FAILED(m_device_context->Map(m_pvertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)))
		return;

	v = (COLOR_VERTEX*)mapData.pData;

	v[0].Position.x = (float)x0;
	v[0].Position.y = (float)y0;
	v[0].Position.z = 0;
	v[0].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[1].Position.x = (float)x1;
	v[1].Position.y = (float)y1;
	v[1].Position.z = 0;
	v[1].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[0].Position.x = xx0;
	v[0].Position.y = yy0;
	v[0].Position.z = 0;
	v[0].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[1].Position.x = xx1;
	v[1].Position.y = yy0;
	v[1].Position.z = 0;
	v[1].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[2].Position.x = xx0;
	v[2].Position.y = yy1;
	v[2].Position.z = 0;
	v[2].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[3].Position.x = xx1;
	v[3].Position.y = yy1;
	v[3].Position.z = 0;
	v[3].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));


	m_device_context->Unmap(m_pvertex_buffer, NULL);

	UINT Stride = sizeof(COLOR_VERTEX);
	UINT Offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &m_pvertex_buffer, &Stride, &Offset);
	m_device_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->IASetInputLayout(m_input_layout);

	m_device_context->VSSetShader(m_vertex_shader, 0, 0);
	m_device_context->PSSetShader(m_pixel_shader, 0, 0);
	m_device_context->GSSetShader(NULL, 0, 0);
	m_device_context->Draw(4, 0);
}

void no_gui::draw_string(std::string str, float fontSize, float x, float y, float r, float g, float b, float a)
{
	
}

void no_gui::draw_line(float x1, float y1, float x2, float y2, color color)
{
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_device_context->OMSetBlendState(m_transparency, blendFactor, 0xffffffff);

	if (m_device_context == NULL) {
		return;
	}
	int a = color.A & 0xff;
	int r = color.R & 0xff;
	int g = color.G & 0xff;
	int b = color.B & 0xff;

	UINT viewportNumber = 1;

	D3D11_VIEWPORT vp;

	m_device_context->RSGetViewports(&viewportNumber, &vp);

	float xx0 = 2.0f * (x1 - 0.5f) / vp.Width - 1.0f;
	float yy0 = 1.0f - 2.0f * (y1 - 0.5f) / vp.Height;
	float xx1 = 2.0f * (x2 - 0.5f) / vp.Width - 1.0f;
	float yy1 = 1.0f - 2.0f * (y2 - 0.5f) / vp.Height;

	COLOR_VERTEX* v = NULL;

	D3D11_MAPPED_SUBRESOURCE mapData;

	if (FAILED(m_device_context->Map(m_pvertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)))
		return;

	v = (COLOR_VERTEX*)mapData.pData;
	v[0].Position.x = xx0;
	v[0].Position.y = yy0;

	v[0].Position.z = 0;
	v[0].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	v[1].Position.x = xx1;
	v[1].Position.y = yy1;
	v[1].Position.z = 0;
	v[1].Color = vec4(
		((float)r / 255.0f),
		((float)g / 255.0f),
		((float)b / 255.0f),
		((float)a / 255.0f));

	m_device_context->Unmap(m_pvertex_buffer, NULL);
	UINT Stride = sizeof(COLOR_VERTEX);
	UINT Offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &m_pvertex_buffer, &Stride, &Offset);
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	m_device_context->IASetInputLayout(m_input_layout);

	m_device_context->VSSetShader(m_vertex_shader, 0, 0);
	m_device_context->PSSetShader(m_pixel_shader, 0, 0);
	m_device_context->GSSetShader(NULL, 0, 0);
	m_device_context->Draw(2, 0);
}

void no_gui::draw_rect(float x, float y, float w, float h, color strokeColor)
{
	fill_rect(x, y, w, 1, strokeColor);
	fill_rect(x, y + h - 1, w, 1, strokeColor);
	fill_rect(x, y + 1, 1, h - 2 * 1, strokeColor);
	fill_rect(x + w - 1, y + 1, 1, h - 2 * 1, strokeColor);
}

void no_gui::draw_outlined_rect(const vec4& rect, const color& strokeColor, const color& color)
{
	fill_rect(rect.x, rect.y, rect.z, rect.w, color);
	draw_rect(rect.x, rect.y, rect.z, rect.w, strokeColor);
}

void no_gui::draw_corner_box(int x, int y, int w, int h, int borderPx, color& temp)
{
	fill_rect(x - (w / 2), (y - h + borderPx), w / 3, borderPx, temp); //top
	fill_rect(x - (w / 2) + w - w / 3, (y - h + borderPx), w / 3, borderPx, temp); //top
	fill_rect(x - (w / 2), (y - h + borderPx), borderPx, w / 3, temp); //left 
	fill_rect(x - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, temp); //left 
	fill_rect(x - (w / 2), y, w / 3, borderPx, temp); //bottom 
	fill_rect(x - (w / 2) + w - w / 3, y, w / 3, borderPx, temp); //bottom 
	fill_rect((x + w - borderPx) - (w / 2), (y - h + borderPx), borderPx, w / 3, temp); //right 
	fill_rect((x + w - borderPx) - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, temp); //right 
}

void no_gui::draw_border(int x, int y, int w, int h, int px, color& temp)
{
	fill_rect(x, (y + h - px), w, px, temp);
	fill_rect(x, y, px, h, temp);
	fill_rect(x, y, w, px, temp);
	fill_rect((x + w - px), y, px, h, temp);
}

void no_gui::draw_cross(float x, float y, float width, float height, const color& color)
{
	fill_rect(x - width, y - 0.5f, (width * 2.0f), 1.0f, color);
	fill_rect(x - 0.5f, y - height, 1.0f, (height * 2.0f), color);
}

void no_gui::draw_circle(float x0, float y0, float radius, const color& color, float thickness)
{
	int x = radius, y = 0;
	int radius_error = 1 - x;

	while (x >= y)
	{
		fill_rect(x + x0, y + y0, thickness, thickness, color);
		fill_rect(y + x0, x + y0, thickness, thickness, color);
		fill_rect(-x + x0, y + y0, thickness, thickness, color);
		fill_rect(-y + x0, x + y0, thickness, thickness, color);
		fill_rect(-x + x0, -y + y0, thickness, thickness, color);
		fill_rect(-y + x0, -x + y0, thickness, thickness, color);
		fill_rect(x + x0, -y + y0, thickness, thickness, color);
		fill_rect(y + x0, -x + y0, thickness, thickness, color);
		y++;

		if (radius_error < 0)
			radius_error += 2 * y + 1;
		else 
		{
			x--;
			radius_error += 2 * (y - x + 1);
		}
	}
}

void no_gui::draw_health_bar(int x, int y, float health, float max_health, int w, int h, bool is_shield)
{
	auto c_black = ::color(12, 12, 12, 228);
	auto c_red = ::color(255, 0, 0, 255);
	auto c_green = ::color(255, 0, 255, 0);
	auto c_yellow = ::color(255, 255, 255, 0);
	auto c_cyan = ::color(255, 0, 255, 255);
	auto c_orange = ::color(255, 255, 153, 0);

	if (is_shield)
	{
		c_red = ::color(255, 102, 2, 209);
		c_green = ::color(255, 19, 105, 244);
		c_yellow = ::color(255, 13, 175, 229);
		c_orange = ::color(255, 171, 13, 229);
	}

	if (health > max_health)
		health = max_health;

	FLOAT hp = health;
	FLOAT factor = max_health / 4;
	auto color = ::color(0.f, 0.f, 0.f, 0.f);

	if (hp <= factor)
		color = c_red;
	else if (hp <= factor * 2)
		color = c_orange;
	else if (hp <= factor * 3)
		color = c_yellow;
	else if (hp <= max_health)
		color = c_green;

	fill_rect(x, y, w, h + 1, ::color(255, 0, 0, 0));
	UINT hw = (UINT)(((w - 2) * health) / max_health);
	fill_rect(x + 1, y + 1, hw, h - 1, color);

}

HWND no_gui::get_window()
{
	return m_hwnd;
}

float no_gui::get_width()
{
	return m_width;
}

float no_gui::get_height()
{
	return m_height;
}

void no_gui::begin_scene()
{
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_back_buffer);
	ID3D11RenderTargetView* p_rtv;
	m_device->CreateRenderTargetView(m_back_buffer, NULL, &p_rtv);
	m_device_context->OMSetRenderTargets(1, &p_rtv, NULL);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	memset(&swap_chain_desc, 0, sizeof(swap_chain_desc));
	m_swap_chain->GetDesc(&swap_chain_desc);
	m_width =  swap_chain_desc.BufferDesc.Width;
	m_height = swap_chain_desc.BufferDesc.Height;

	m_restore_state = false;
	if (SUCCEEDED(m_state_saver.save_current_state(m_device_context)))
		m_restore_state = true;
	m_device_context->IASetInputLayout(m_input_layout);
}

void no_gui::end_scene()
{
	D3D11_TEXTURE2D_DESC backBufferDesc;
	m_back_buffer->GetDesc(&backBufferDesc);
	m_back_buffer->Release();

	if (m_restore_state)
		m_state_saver.restore_saved_state();
}