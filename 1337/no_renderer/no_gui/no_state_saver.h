#pragma once
#include "no_gui.h"

class no_state_saver
{
private:
	bool						m_saved_state;
	D3D_FEATURE_LEVEL			m_feature_level;
	ID3D11DeviceContext			*m_pcontext;
	D3D11_PRIMITIVE_TOPOLOGY	m_primitive_topology;
	ID3D11InputLayout			*m_pinput_layout;
	ID3D11BlendState			*m_pblend_state;
	float						m_blend_factor[4];
	UINT						m_sample_mask;
	ID3D11DepthStencilState		*m_pdepth_stencil_state;
	UINT						m_stencilRef;
	ID3D11RasterizerState		*m_pRasterizerState;
	ID3D11ShaderResourceView	*m_pPSSRV;
	ID3D11SamplerState			*m_pSamplerState;
	ID3D11VertexShader			*m_pVS;
	ID3D11ClassInstance			*m_pVSClassInstances[256];
	UINT						m_numVSClassInstances;
	ID3D11Buffer				*m_pVSConstantBuffer;
	ID3D11GeometryShader		*m_pGS;
	ID3D11ClassInstance			*m_pGSClassInstances[256];
	UINT						m_numGSClassInstances;
	ID3D11Buffer				*m_pGSConstantBuffer;
	ID3D11ShaderResourceView	*m_pGSSRV;
	ID3D11PixelShader			*m_pPS;
	ID3D11ClassInstance			*m_pPSClassInstances[256];
	UINT						m_numPSClassInstances;
	ID3D11HullShader			*m_pHS;
	ID3D11ClassInstance			*m_pHSClassInstances[256];
	UINT						m_numHSClassInstances;
	ID3D11DomainShader			*m_pDS;
	ID3D11ClassInstance			*m_pDSClassInstances[256];
	UINT						m_numDSClassInstances;
	ID3D11Buffer				*m_pVB;
	UINT						m_vertexStride;
	UINT						m_vertexOffset;
	ID3D11Buffer				*m_pIndexBuffer;
	DXGI_FORMAT					m_indexFormat;
	UINT						m_indexOffset;

	no_state_saver(const no_state_saver&);
	no_state_saver& operator=(const no_state_saver&);
public:

	no_state_saver();
	~no_state_saver();

	HRESULT save_current_state(ID3D11DeviceContext *p_context);
	HRESULT restore_saved_state();
	void release_saved_state();
};

