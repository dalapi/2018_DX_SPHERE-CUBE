#include "light.h"


void UpdateLight() 
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	
	// angle
	D3DXVECTOR3 vecDir(0.0f, -1.0f, 0.3f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Diffuse = { 1,1,1,1 };	// rgba
	//light.Ambient = {1,1,1,1};

	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
}