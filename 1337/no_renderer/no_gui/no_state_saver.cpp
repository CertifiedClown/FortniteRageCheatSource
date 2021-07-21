#include "no_gui.h"

no_state_saver::no_state_saver() :
	m_saved_state(false),
	m_feature_level(D3D_FEATURE_LEVEL_11_0),
	m_pcontext(NULL),
	m_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED),
	m_pinput_layout(NULL),
	m_pblend_state(NULL),
	m_sample_mask(0xffffffff),
	m_pdepth_stencil_state(NULL),
	m_stencilRef(0),
	m_pRasterizerState(NULL),
	m_pPSSRV(NULL),
	m_pSamplerState(NULL),
	m_pVS(NULL),
	m_numVSClassInstances(0),
	m_pVSConstantBuffer(NULL),
	m_pGS(NULL),
	m_numGSClassInstances(0),
	m_pGSConstantBuffer(NULL),
	m_pGSSRV(NULL),
	m_pPS(NULL),
	m_numPSClassInstances(0),
	m_pHS(NULL),
	m_numHSClassInstances(0),
	m_pDS(NULL),
	m_numDSClassInstances(0),
	m_pVB(NULL),
	m_vertexStride(0),
	m_vertexOffset(0),
	m_pIndexBuffer(NULL),
	m_indexFormat(DXGI_FORMAT_UNKNOWN),
	m_indexOffset(0)
{
	for (int i = 0; i < 4; ++i)
		m_blend_factor[i] = 0.0f;
	for (int i = 0; i < 256; ++i)
	{
		m_pVSClassInstances[i] = NULL;
		m_pGSClassInstances[i] = NULL;
		m_pPSClassInstances[i] = NULL;
		m_pHSClassInstances[i] = NULL;
		m_pDSClassInstances[i] = NULL;
	}
}

no_state_saver::~no_state_saver()
{
	release_saved_state();
}

HRESULT no_state_saver::save_current_state(ID3D11DeviceContext *pContext)
{
	if (m_saved_state)
		release_saved_state();
	if (pContext == NULL)
		return E_INVALIDARG;

	ID3D11Device *p_device;
	pContext->GetDevice(&p_device);
	if (p_device != NULL) {
		m_feature_level = p_device->GetFeatureLevel();
		p_device->Release();
	}

	pContext->AddRef();
	m_pcontext = pContext;

	m_pcontext->IAGetPrimitiveTopology(&m_primitive_topology);
	m_pcontext->IAGetInputLayout(&m_pinput_layout);

	m_pcontext->OMGetBlendState(&m_pblend_state, m_blend_factor, &m_sample_mask);
	m_pcontext->OMGetDepthStencilState(&m_pdepth_stencil_state, &m_stencilRef);

	m_pcontext->RSGetState(&m_pRasterizerState);

	m_numVSClassInstances = 256;
	m_pcontext->VSGetShader(&m_pVS, m_pVSClassInstances, &m_numVSClassInstances);
	m_pcontext->VSGetConstantBuffers(0, 1, &m_pVSConstantBuffer);

	m_numPSClassInstances = 256;
	m_pcontext->PSGetShader(&m_pPS, m_pPSClassInstances, &m_numPSClassInstances);
	m_pcontext->PSGetShaderResources(0, 1, &m_pPSSRV);
	pContext->PSGetSamplers(0, 1, &m_pSamplerState);

	if (m_feature_level >= D3D_FEATURE_LEVEL_10_0)
	{
		m_numGSClassInstances = 256;
		m_pcontext->GSGetShader(&m_pGS, m_pGSClassInstances, &m_numGSClassInstances);
		m_pcontext->GSGetConstantBuffers(0, 1, &m_pGSConstantBuffer);

		m_pcontext->GSGetShaderResources(0, 1, &m_pGSSRV);

		if (m_feature_level >= D3D_FEATURE_LEVEL_11_0)
		{
			m_numHSClassInstances = 256;
			m_pcontext->HSGetShader(&m_pHS, m_pHSClassInstances, &m_numHSClassInstances);

			m_numDSClassInstances = 256;
			m_pcontext->DSGetShader(&m_pDS, m_pDSClassInstances, &m_numDSClassInstances);
		}
	}

	m_pcontext->IAGetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);

	m_pcontext->IAGetIndexBuffer(&m_pIndexBuffer, &m_indexFormat, &m_indexOffset);

	m_saved_state = true;

	return S_OK;
}

HRESULT no_state_saver::restore_saved_state()
{
	if (!m_saved_state)
		return E_FAIL;

	m_pcontext->IASetPrimitiveTopology(m_primitive_topology);
	m_pcontext->IASetInputLayout(m_pinput_layout);

	m_pcontext->OMSetBlendState(m_pblend_state, m_blend_factor, m_sample_mask);
	m_pcontext->OMSetDepthStencilState(m_pdepth_stencil_state, m_stencilRef);

	m_pcontext->RSSetState(m_pRasterizerState);

	m_pcontext->VSSetShader(m_pVS, m_pVSClassInstances, m_numVSClassInstances);
	m_pcontext->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);

	m_pcontext->PSSetShader(m_pPS, m_pPSClassInstances, m_numPSClassInstances);
	m_pcontext->PSSetShaderResources(0, 1, &m_pPSSRV);
	m_pcontext->PSSetSamplers(0, 1, &m_pSamplerState);

	if (m_feature_level >= D3D_FEATURE_LEVEL_10_0)
	{
		m_pcontext->GSSetShader(m_pGS, m_pGSClassInstances, m_numGSClassInstances);
		m_pcontext->GSSetConstantBuffers(0, 1, &m_pGSConstantBuffer);
		m_pcontext->GSSetShaderResources(0, 1, &m_pGSSRV);

		if (m_feature_level >= D3D_FEATURE_LEVEL_11_0)
		{
			m_pcontext->HSSetShader(m_pHS, m_pHSClassInstances, m_numHSClassInstances);
			m_pcontext->DSSetShader(m_pDS, m_pDSClassInstances, m_numDSClassInstances);
		}
	}

	m_pcontext->IASetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);
	m_pcontext->IASetIndexBuffer(m_pIndexBuffer, m_indexFormat, m_indexOffset);
	return S_OK;
}

void no_state_saver::release_saved_state()
{
	m_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	SAFE_RELEASE(m_pinput_layout);
	SAFE_RELEASE(m_pblend_state);
	for (int i = 0; i < 4; ++i)
		m_blend_factor[i] = 0.0f;
	m_sample_mask = 0xffffffff;
	SAFE_RELEASE(m_pdepth_stencil_state);
	m_stencilRef = 0;
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pPSSRV);
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pVS);
	for (UINT i = 0; i < m_numVSClassInstances; ++i)
		SAFE_RELEASE(m_pVSClassInstances[i]);
	m_numVSClassInstances = 0;
	SAFE_RELEASE(m_pVSConstantBuffer);
	SAFE_RELEASE(m_pGS);
	for (UINT i = 0; i < m_numGSClassInstances; ++i)
		SAFE_RELEASE(m_pGSClassInstances[i]);
	m_numGSClassInstances = 0;
	SAFE_RELEASE(m_pGSConstantBuffer);
	SAFE_RELEASE(m_pGSSRV);
	SAFE_RELEASE(m_pPS);
	for (UINT i = 0; i < m_numPSClassInstances; ++i)
		SAFE_RELEASE(m_pPSClassInstances[i]);
	m_numPSClassInstances = 0;
	SAFE_RELEASE(m_pHS);
	for (UINT i = 0; i < m_numHSClassInstances; ++i)
		SAFE_RELEASE(m_pHSClassInstances[i]);
	m_numHSClassInstances = 0;
	SAFE_RELEASE(m_pDS);
	for (UINT i = 0; i < m_numDSClassInstances; ++i)
		SAFE_RELEASE(m_pDSClassInstances[i]);
	m_numDSClassInstances = 0;
	SAFE_RELEASE(m_pVB);
	m_vertexStride = 0;
	m_vertexOffset = 0;
	SAFE_RELEASE(m_pIndexBuffer);
	m_indexFormat = DXGI_FORMAT_UNKNOWN;
	m_indexOffset = 0;

	SAFE_RELEASE(m_pcontext);
	m_feature_level = D3D_FEATURE_LEVEL_11_0;
	m_saved_state = false;
}