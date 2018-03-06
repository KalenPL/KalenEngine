#ifndef SHADOW_H
#define SHADOW_H
//***************************************************************************************
// Frank Luna (C) 2011 Wszelkie prawa zastrze¿one.
//***************************************************************************************

#include "DXApp.h"  

class Shadow
{
public:
	Shadow(ID3D11Device* device, UINT width, UINT height);
	~Shadow();
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* immediateContext);

private:
	UINT mWidth;
	UINT mHeight;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};

#endif // SHADOW_H