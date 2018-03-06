#include "../Header Files/Render.h"

extern bool gFilterDebugGrid;
extern bool gFilterDebugGizmo;
extern bool gFilterRenderMesh;
extern bool gFilterDebugBSphereLight;
extern bool gFilterDebugWireframe;
extern bool gFilterRenderTessellation;
extern bool gFilterRenderRefraction;
extern bool gFilterRenderRefractionDynamic;
extern bool gFilterDebugBBoxes;
extern bool gFilterDebugCollision;
extern bool gFilterDebugSelection;
extern bool gFilterSSAO;
extern bool gFilterRenderShadow;

//*****************************************************************************************************************************
//Renderer
//*****************************************************************************************************************************
void KalenEngine::Render(float dt) //Rysuje obiekty
{
	
	//UpdateWindow(m_hAppWndPanelBottom);
	//**************************************
	// Update our time  -- Count FPS
	//**************************************
	static float t = 0.0f;

	if (!__stop_time) //tylko jeœli czas jest aktywny
	{

		if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			t += (float)XM_PI * 0.0125f;
		}
		else
		{
			static DWORD dwTimeStart = 0;
			DWORD dwTimeCur = GetTickCount();
			if (dwTimeStart == 0)
				dwTimeStart = dwTimeCur;
			t = (dwTimeCur - dwTimeStart) / 1000.0f;
		}
	}

	//**************************************
	//Shadow
	//**************************************
	if (gFilterRenderShadow == true)
	{
		mShadow->BindDsvAndSetNullRenderTarget(m_pImmediateContext);
		mFX_AllowShadowMap->SetBool(true);
		RenderShadowMap();
	}
	else
	{
		mFX_AllowShadowMap->SetBool(false);
	}



	//
	// Przywróæ bufor tylny i bufor g³êbokoœci dla etapu ³¹czenia wyników.
	//
	ID3D11RenderTargetView* renderTargets[1];
	renderTargets[0] = m_pRenderTargetView;




	//**************************************
	//SSAO
	//**************************************
	/*if (gFilterSSAO == true)
	{
		m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pImmediateContext->RSSetViewports(1, &m_Viewport);
		mSSAO->SetNormalDepthRenderTarget(mDepthStencilView);
		//DrawMesh(mCam, false); //Draw all mesh 
		RenderSSAOMap();
		mSSAO->ComputeSsao(mCam);
		//mSSAO->BlurAmbientMap(4);
		mFX_AllowSSAOMap->SetBool(true);
	}
	else
	{
		mFX_AllowSSAOMap->SetBool(false);
	}
	*/
	m_pImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	m_pImmediateContext->RSSetState(0);
	

	//**************************************
	//CubeMapping Dynamic
	//**************************************
	if (gFilterRenderRefractionDynamic == true)
	{
		mFX_AllowCubeMap->SetBool(true);
		BuildCubeFaceCamera(mCubeMapPosition.x, mCubeMapPosition.y, mCubeMapPosition.z);
		// Generate the cube map.
		m_pImmediateContext->RSSetViewports(1, &mCubeMapViewport);
		for (int i = 0; i < 6; ++i)
		{
			// Clear cube map face and depth buffer.
			m_pImmediateContext->ClearRenderTargetView(mDynamicCubeMapRTV[i], reinterpret_cast<const float*>(&DirectX::Colors::Black));
			m_pImmediateContext->ClearDepthStencilView(mDynamicCubeMapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			// Bind cube map face as render target.
			renderTargets[0] = mDynamicCubeMapRTV[i];
			m_pImmediateContext->OMSetRenderTargets(1, renderTargets, mDynamicCubeMapDSV);

			DrawMesh(mCubeMapCamera[i], true); //Draw all mesh 
		}

		// Mipmap levels of cube map.
		m_pImmediateContext->GenerateMips(mDynamicCubeMapSRV);
	}
/*	else
	{
		mFX_AllowCubeMap->SetBool(false);
	}*/

	//******************************************************************************
	//NormalRendering
	//******************************************************************************


	renderTargets[0] = m_pRenderTargetView;
	m_pImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
	//m_pImmediateContext->RSSetViewports(1, &m_Viewport2);

	m_pImmediateContext->RSSetViewports(1, &m_Viewport);


	//**************************************
	//Ustawia kolor t³a --> tutaj czarny
	//**************************************
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Black)); //kolor! --> mozna zmienic
	//**************************************
	//Ustawia bufor g³êbokoœci
	//**************************************
	m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Set the input layout
	m_pImmediateContext->IASetInputLayout(mInputLayout);



	m_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBufferGrid, &stride, &offset);
	//**************************************
	//**************************************



	//**************************************
	//Kamera UPDATE
	//**************************************
	mCam.UpdateViewMatrix();
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();

	//Macierz jednostkowa! -- taka, jak dla obiektu
	mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&m_worldBaseTransform));
//	mFXTess_pWorld->SetMatrix(reinterpret_cast<float*>(&m_worldBaseTransform));


	g_WorldInvTranspose = MathHelper::InverseTranspose(m_worldBaseTransform);
	mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));
	//mFXTess_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));


	g_WorldViewProjection = m_worldBaseTransform * view * proj;
	mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
//	mFXTess_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


	mFX_pTexTransform->SetMatrix(reinterpret_cast<float*>(&mTexTransform));
	//mFXTess_pTexTransform->SetMatrix(reinterpret_cast<float*>(&mTexTransform));
	mFXShadow_pTexTransform->SetMatrix(reinterpret_cast<float*>(&mTexTransform)); //Set UV in shader (FX) -- only for alphaMap

	mTech = mFX->GetTechniqueByName("FeatureLevel11NORMAL");
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);


	//For grid -- show
	if (gFilterDebugGrid == true)
	{
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		m_pImmediateContext->Draw(Grid_Count, 0);
	}


	//****************************************************************************
	//Rysowanie Mesh'u
	//****************************************************************************



	if (__normal_view)
		if (__normalTexture)
		{
			//mTech = mFX->GetTechniqueByName("FeatureLevel11NORMAL_TEXTURE");

			mTech = mFX->GetTechniqueByName("FeatureLevel11_NEW");
		//	mTechTess = mFXTess->GetTechniqueByName("FeatureLevel11TessellationNORMAL_TEXTURE");
		}
		else
		{
			//mTech = mFX->GetTechniqueByName("FeatureLevel11NORMAL");
			mTech = mFX->GetTechniqueByName("FeatureLevel11_NEW");
		//	mTechTess = mFXTess->GetTechniqueByName("FeatureLevel11TessellationNORMAL");
		}
	else
	{
		if (__diffuseTexture) {
			mFX_AllowDiffuse->SetBool(true);
			//mFXTess_AllowDiffuse->SetBool(true);
		}
		else
		{
			mFX_AllowDiffuse->SetBool(false);
			//mFXTess_AllowDiffuse->SetBool(false);
		}


		if (__normalTexture)
		{
			mFX_AllowNormal->SetBool(true);
		//	mFXTess_AllowNormal->SetBool(true);
		}

		else
		{
			mFX_AllowNormal->SetBool(false);
		//	mFXTess_AllowNormal->SetBool(false);
		}


		if (__specularTexture)
		{
			mFX_AllowSpecular->SetBool(true);
		//	mFXTess_AllowSpecular->SetBool(true);
		}

		else
		{
			mFX_AllowSpecular->SetBool(false);
			//mFXTess_AllowSpecular->SetBool(false);
		}

	}



	//Pozycja kamery na scenie
	mEyePosW = XMFLOAT3(mCam.GetPosition().x, mCam.GetPosition().y, mCam.GetPosition().z);




	//WIRE TYPE VIEW
	if (__show_type_wire_new == 0)
	{
		m_pImmediateContext->RSSetState(mWireframeRS);
	}
	else
		if (__show_type_wire_new == 1)
		{
			m_pImmediateContext->RSSetState(mWireThroughFrameRS);
		}
		else
			if (__show_type_wire_new == 2 || __show_type_wire_new == 3)
			{
				m_pImmediateContext->RSSetState(mSolidframeRS);
			}


	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		DrawMesh(mCam, false); //Draw all mesh 
		//RenderSSAOMap();
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


	//Render WireFrame and solid --> Render all mesh again 
	if (__show_type_wire_new == 3)
	{
		m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		mTech = mFX->GetTechniqueByName("FeatureLevel11WhiteWireFrame");
		m_pImmediateContext->RSSetState(mWireframeRS);
		mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
		DrawMesh(mCam, false);
	}
	
	m_pImmediateContext->RSSetState(0);
	m_pImmediateContext->OMSetDepthStencilState(0, 0);

	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	m_pImmediateContext->PSSetShaderResources(0, 16, nullSRV);
	m_pSwapChain->Present(0, 0);
}




//***************************************************************************************************************************************
//DrawMesh
//***************************************************************************************************************************************
void KalenEngine::DrawMesh(Camera& camera, bool cubeMap)
{

	camera.UpdateViewMatrix();
	XMMATRIX view = camera.View();
	XMMATRIX proj = camera.Proj();
	float distance = 0.0;
	XMVECTOR distanceVector;
	mPointLightsCounter = 0;
	XMMATRIX shadowTransform = XMLoadFloat4x4(&mShadowTransform);
	XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);


	//-----------------------
	//Reset lights
	//-----------------------
	for (int i = 0; i < 10; ++i)
	{
		mPointLight[i].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		mPointLight[i].Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		mPointLight[i].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		mPointLight[i].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mPointLight[i].Range = 0.0f;
		mPointLight[i].Att = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}


	//----------------------
	// Set mRenderObjectsBuffer
	//----------------------
	mRenderObjectsBuffer->clear();
	mRenderObjectsTransparencyBuffer->clear();
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
		for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
		{
			if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0)//only KLMESH
			{
				if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetIsAllowRender() == true)
				{
					bool isTransparency = false;
					for (int k = 0; k < (int)mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->size(); ++k)//materials
						if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->at(k).shaderName == "Phong Transparency"
							||
							mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->at(k).shaderName == "Cook-Torrance Transparency"
							)
							isTransparency = true;
						else
							isTransparency = false;

					if (isTransparency == false)
						mRenderObjectsBuffer->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));
					else
						mRenderObjectsTransparencyBuffer->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));//add to transparency buffer
				}
			}
			else //light
			{
				mRenderObjectsBuffer->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));
			}
		}




	
	XMVECTOR detView = XMMatrixDeterminant(mCam.View());
	XMMATRIX invView = XMMatrixInverse(&detView, mCam.View());
	XMVECTOR scale;
	XMVECTOR rotQuat;
	XMVECTOR translation;
	Frustum localSpaceFrustum;
	XMMatrixDecompose(&scale, &rotQuat, &translation, invView);
	TransformFrustum(&localSpaceFrustum, &mCamFrustum, XMVectorGetX(scale), rotQuat, translation);

	if (cubeMap == false)
	{
		std::vector<Object*>* objsVec = new std::vector<Object*>();
		objsVec = mOctree->Test(localSpaceFrustum);

		OctreeNode* abcd = mOctree->GetRootNode();
		wxString name123 = abcd->GetName();

		/*for (int x = 0; x < (int)objsVec->size(); ++x)
		{
			if (IntersectAxisAlignedBoxFrustum(&objsVec->at(x)->GetAxisAlignedBoundingBox(), &localSpaceFrustum) == 0)
				objsVec->erase(objsVec->begin() + x);
		}*/

		//std::vector<Object*>* tempVec = new std::vector<Object*>();
		//Delete invisible objects
		bool isExists = false;
		for (int i = 0; i < (int)mRenderObjectsTransparencyBuffer->size(); ++i)
		{
			isExists = false;
			for (int x = 0; x < (int)objsVec->size(); ++x)
			{
				if (mRenderObjectsTransparencyBuffer->at(i) == objsVec->at(x))
				{
					isExists = true;
					break;
				}
			}
			if (isExists == false)
				mRenderObjectsTransparencyBuffer->erase(mRenderObjectsTransparencyBuffer->begin() + i);
		}

		isExists = false;
		for (int i = 0; i < (int)mRenderObjectsBuffer->size(); ++i)
		{
			if (mRenderObjectsBuffer->at(i)->GetType() == 0)//only KLMESH
			{
				isExists = false;
				for (int x = 0; x < (int)objsVec->size(); ++x)
				{
					if (mRenderObjectsBuffer->at(i) == objsVec->at(x))
					{ 
						isExists = true;
						break;
					}
						
				}
				if (isExists == false)
					mRenderObjectsBuffer->erase(mRenderObjectsBuffer->begin() + i);
			}
		}


		//Delete invisible object from 'mRenderObjectsBuffer'
		for (int x = 0; x < (int)mRenderObjectsBuffer->size(); ++x)
		{
			if(mRenderObjectsBuffer->at(x)->GetType() == 0)//only klmesh
				if (IntersectAxisAlignedBoxFrustum(&mRenderObjectsBuffer->at(x)->GetAxisAlignedBoundingBox(), &localSpaceFrustum) == 0)
					mRenderObjectsBuffer->erase(mRenderObjectsBuffer->begin() + x);
		}
		//Delete invisible object from 'mRenderObjectsTransparencyBuffer'
		for (int x = 0; x < (int)mRenderObjectsTransparencyBuffer->size(); ++x)
		{
			if (mRenderObjectsTransparencyBuffer->at(x)->GetType() == 0)//only klmesh
				if (IntersectAxisAlignedBoxFrustum(&mRenderObjectsTransparencyBuffer->at(x)->GetAxisAlignedBoundingBox(), &localSpaceFrustum) == 0)
					mRenderObjectsTransparencyBuffer->erase(mRenderObjectsTransparencyBuffer->begin() + x);
		}
	}
	//*/






	//---------------------------
	//Sort objects in the transparency object buffer
	//---------------------------
	for (int i = 0; i < (int)mRenderObjectsTransparencyBuffer->size(); ++i)
	{
		distanceVector = XMVector3Length((camera.GetPositionXM() - mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
		distance = XMVectorGetX(distanceVector);
		mRenderObjectsTransparencyBuffer->at(i)->SetTempDistance(distance);
	}
	QuicksortObjects(mRenderObjectsTransparencyBuffer, 0, (int)mRenderObjectsTransparencyBuffer->size() - 1);

	//---------------------------


	


	//-------------
	//Draw all mesh from mRenderObjectsBuffer
	//-------------
	for (int i = 0; i < (int)mRenderObjectsBuffer->size(); ++i)
	{
		if (mRenderObjectsBuffer->at(i)->GetType() == 0)//Render only KLMESH
		{
			if(mRenderObjectsBuffer->at(i)->GetIsVisible() == true)
			{
				m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsBuffer->at(i)->GetKLMESH()->GetVertexBuffer(), &stride, &offset);
				m_pImmediateContext->IASetIndexBuffer(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
				m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//Tessellation OFF
				m_pImmediateContext->HSSetShader(0, 0, 0);
				m_pImmediateContext->DSSetShader(0, 0, 0);

				//Update object coordinates
				mRenderObjectsBuffer->at(i)->GetCoordinate()->UpdateCoordinateMatrix();

				XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsBuffer->at(i)->GetCoordinate()->GetMatrix());
				mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));

				g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
				mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));

				g_WorldViewProjection = odwrotnoscMacierzyPrzejscia  * view * proj;
				mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
				mFX_pShadowTransform->SetMatrix(reinterpret_cast<const float*>(&XMMatrixMultiply(odwrotnoscMacierzyPrzejscia, shadowTransform)));

				mFX_pWorldViewProjectionTex->SetMatrix(reinterpret_cast<float*>(&(g_WorldViewProjection * toTexSpace)));

				//Distance to object 
				distanceVector = XMVector3Length((camera.GetPositionXM() - mRenderObjectsBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
				distance = XMVectorGetX(distanceVector);


				if (mRenderObjectsBuffer->at(i)->GetForceLODLevel() == 0)
					distance = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetDistanceLOD0() - 1;
				if (mRenderObjectsBuffer->at(i)->GetForceLODLevel() == 1)
					distance = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetDistanceLOD1() - 1;
				if (mRenderObjectsBuffer->at(i)->GetForceLODLevel() == 2)
					distance = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetDistanceLOD2() - 1;

				int matPos = -1;
				if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance) != nullptr)//If the object is visible
					for (int j = 0; j < (int)mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->size(); ++j)//rendering all chunks of object
					{
						matPos = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->materialPosition;
						//Render Diffuse?
						if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMapName == "NULL")
							mFX_AllowDiffuse->SetBool(false);
						else
							mFX_AllowDiffuse->SetBool(true);
						//Render ASH?
						if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).ashMapName == "NULL")
							mFX_AllowSpecular->SetBool(false);
						else
							mFX_AllowSpecular->SetBool(true);
						//Render Normal?
						if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).normalMapName == "NULL")
							mFX_AllowNormal->SetBool(false);
						else
							mFX_AllowNormal->SetBool(true);

						//Cubemap render flag
						if (gFilterRenderRefraction == false)
							mFX_AllowCubeMap->SetBool(false);
						else
							mFX_AllowCubeMap->SetBool(true);
						//Render Cubemap?
						if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMapName == "NULL")
							mFX_AllowCubeMap->SetBool(false);
						else
							if (gFilterRenderRefraction == false)
								mFX_AllowCubeMap->SetBool(false);
							else
								mFX_AllowCubeMap->SetBool(true);

						//Set Resources Textures
						mFX_pDiffuseMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap);
						mFX_pNormalMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).normalMap);
						mFX_pASHMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap);

						//if (gFilterSSAO == true)
						//	mFX_pSSAOMap->SetResource(mSSAO->AmbientSRV());

						if (gFilterRenderRefraction == true)
							if(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMapDynamic == true)
								mFX_pCubeMap->SetResource(mDynamicCubeMapSRV);//Dynamic cube mapping
							else
								mFX_pCubeMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMap); //Normal cube mapping
							
						if (gFilterRenderShadow == true)
							mFX_pShadowMap->SetResource(mShadow->DepthMapSRV());

						//Select object
						if (mSelectedObject != nullptr)
						{
							if (mRenderObjectsBuffer->at(i)->GetTreeItemId() == mSelectedObject->GetTreeItemId() && gFilterDebugSelection)
							{
								mFX_AllowSpecular->SetBool(false);
								mSelectedMaterialHlsl = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl;
								mSelectedMaterialHlsl.Ambient.x = 0.2f;
								mSelectedMaterialHlsl.Ambient.y = 0.9f;
								mSelectedMaterialHlsl.Ambient.z = 0.2f;
								mFX_pMaterial->SetRawValue(&mSelectedMaterialHlsl, 0, sizeof(mSelectedMaterialHlsl));
							}
							else
							{
								mFX_pMaterial->SetRawValue(&mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl, 0, sizeof(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl));
							}
						}
						else
						{
							mFX_pMaterial->SetRawValue(&mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl, 0, sizeof(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl));
						}
						

						//Transparency
						mFX_Transparency->SetFloat(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).transparency);
						//Material
						//mFX_pMaterial->SetRawValue(&mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl, 0, sizeof(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl));


						//Get & set shader
						if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong")
							mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
						else if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong Transparency")
							mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTransparency");
						else if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong Tessellation")
						{
							if (gFilterRenderTessellation == true)
							{
								mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTessellation");
								m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
								g_WorldViewProjection = camera.ViewProj();
								mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
							}
							else
								mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
						}
						else if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Transparency")
							mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorranceTransparency");
						else if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Tessellation")
						{
							if (gFilterRenderTessellation == true)
							{
								mTech = mFX->GetTechniqueByName("FeatureLevel11Tessellation");
								m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
								g_WorldViewProjection = camera.ViewProj();
								mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
							}
							else
								mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");
						}
						else
							mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");

						//Wireframe
						if (gFilterDebugWireframe == true)
							m_pImmediateContext->RSSetState(mWireframeRS);
						else
							m_pImmediateContext->RSSetState(mSolidframeRS);

						//Update
						mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

						//Draw
						if (gFilterRenderMesh == true)//Draw mesh?
							if (mRenderObjectsBuffer->at(i)->GetKLMESH() != nullptr)
								m_pImmediateContext->DrawIndexed
								(
									mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
									mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startIndex,
									mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startVertex
									);

						//Tessellation OFF
						m_pImmediateContext->HSSetShader(0, 0, 0);
						m_pImmediateContext->DSSetShader(0, 0, 0);
					}
				}


				//-----------------------------
				//Draw BoundingBox
				//-----------------------------
				if (gFilterDebugBBoxes)
				{
					g_WorldViewProjection = XMMatrixIdentity() * view * proj;
					mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));

					//Draw boundingBox
					RebuildBoundingBox(mRenderObjectsBuffer->at(i));//rebuild bbox
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
					m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsBuffer->at(i)->GetKLMESH()->GetBBoxVertexBuffer(), &stride, &offset);
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
					mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(24, 0);
				}

				//-----------------------------
				//Draw collisionMesh
				//-----------------------------
				if (gFilterDebugCollision == true)
					DrawCollisionMesh(mRenderObjectsBuffer->at(i));
		}



		//---------------
		//Lights case
		//---------------
		if (mRenderObjectsBuffer->at(i)->GetType() == 1)
		{
			mRenderObjectsBuffer->at(i)->GetCoordinate()->UpdateLocalCoordinateMatrix();
			XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsBuffer->at(i)->GetCoordinate()->GetMatrix());
			mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));
			g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
			mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));
			g_WorldViewProjection = odwrotnoscMacierzyPrzejscia  * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


			if (gFilterDebugBSphereLight == true)
			{
				m_pImmediateContext->IASetVertexBuffers(0, 1, &mVerticesBufferLightSphere, &stride, &offset);
				m_pImmediateContext->IASetIndexBuffer(mIndicesBufferLightSphere, DXGI_FORMAT_R16_UINT, 0);
				m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
				mFX_SetLineColor->SetInt(0);//Normal colour


				//m_pImmediateContext->RSSetState(mWireframeRS);
				mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

				if (mRenderObjectsBuffer->at(i)->GetPointLight() != nullptr)
					TransformLightSphere(mRenderObjectsBuffer->at(i)->GetPointLight()->Range, mRenderObjectsBuffer->at(i)->GetPointLight()->Diffuse);
				else
					TransformLightSphere(1.0f, XMFLOAT4(255.0f, 255.0f, 255.0f, 255.0f));
				m_pImmediateContext->DrawIndexed(mLightSphere.Indices.size(), 0, 0);
			}

			//Send light to GPU
			if (mPointLightsCounter < 11)
			{
				mPointLight[mPointLightsCounter].Ambient = mRenderObjectsBuffer->at(i)->GetPointLight()->Ambient;
				mPointLight[mPointLightsCounter].Diffuse = mRenderObjectsBuffer->at(i)->GetPointLight()->Diffuse;
				mPointLight[mPointLightsCounter].Specular = mRenderObjectsBuffer->at(i)->GetPointLight()->Specular;
				mPointLight[mPointLightsCounter].Position = mRenderObjectsBuffer->at(i)->GetPointLight()->Position;
				mPointLight[mPointLightsCounter].Range = mRenderObjectsBuffer->at(i)->GetPointLight()->Range;
				mPointLight[mPointLightsCounter].Att = mRenderObjectsBuffer->at(i)->GetPointLight()->Att;

				++mPointLightsCounter;
			}

		}
	}


	//-------------
	//Draw all mesh from mRenderObjectsTransparencyBuffer
	//-------------
	for (int i = (int)mRenderObjectsTransparencyBuffer->size() - 1; i >= 0 ; --i)
	{
		if (mRenderObjectsTransparencyBuffer->at(i)->GetIsVisible() == true)
		{
			m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetVertexBuffer(), &stride, &offset);
			m_pImmediateContext->IASetIndexBuffer(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//Tessellation OFF
			m_pImmediateContext->HSSetShader(0, 0, 0);
			m_pImmediateContext->DSSetShader(0, 0, 0);

			//Update object coordinates
			mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->UpdateCoordinateMatrix();

			XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->GetMatrix());
			mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));

			g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
			mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));

			g_WorldViewProjection = odwrotnoscMacierzyPrzejscia  * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
			//mFX_pShadowTransform->SetMatrix(reinterpret_cast<const float*>(&XMMatrixMultiply(odwrotnoscMacierzyPrzejscia, shadowTransform)));
			mFX_pWorldViewProjectionTex->SetMatrix(reinterpret_cast<float*>(&(g_WorldViewProjection * toTexSpace)));

			//Distance to object 
			distanceVector = XMVector3Length((camera.GetPositionXM() - mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
			distance = XMVectorGetX(distanceVector);


			if (mRenderObjectsTransparencyBuffer->at(i)->GetForceLODLevel() == 0)
				distance = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetDistanceLOD0() - 1;
			if (mRenderObjectsTransparencyBuffer->at(i)->GetForceLODLevel() == 1)
				distance = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetDistanceLOD1() - 1;
			if (mRenderObjectsTransparencyBuffer->at(i)->GetForceLODLevel() == 2)
				distance = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetDistanceLOD2() - 1;

			int matPos = -1;
			if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance) != nullptr)//If the object is visible
				for (int j = 0; j < (int)mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->size(); ++j)//rendering all chunks of object
				{
					matPos = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->materialPosition;
					//Render Diffuse?
					if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMapName == "NULL")
						mFX_AllowDiffuse->SetBool(false);
					else
						mFX_AllowDiffuse->SetBool(true);
					//Render ASH?
					if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).ashMapName == "NULL")
						mFX_AllowSpecular->SetBool(false);
					else
						mFX_AllowSpecular->SetBool(true);
					//Render Normal?
					if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).normalMapName == "NULL")
						mFX_AllowNormal->SetBool(false);
					else
						mFX_AllowNormal->SetBool(true);

					//Cubemap render flag
					if (gFilterRenderRefraction == false)
						mFX_AllowCubeMap->SetBool(false);
					else
						mFX_AllowCubeMap->SetBool(true);
					//Render Cubemap?
					if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMapName == "NULL")
						mFX_AllowCubeMap->SetBool(false);
					else
						if (gFilterRenderRefraction == false)
							mFX_AllowCubeMap->SetBool(false);
						else
							mFX_AllowCubeMap->SetBool(true);

					//Set Resources Textures
					mFX_pDiffuseMap->SetResource(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap);
					mFX_pNormalMap->SetResource(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).normalMap);
					mFX_pASHMap->SetResource(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap);




					//if (gFilterSSAO == true)
					//	mFX_pSSAOMap->SetResource(mSSAO->AmbientSRV());

					if (gFilterRenderRefraction == true)
						if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMapDynamic == true)
							mFX_pCubeMap->SetResource(mDynamicCubeMapSRV);//Dynamic cube mapping
						else
							mFX_pCubeMap->SetResource(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).cubeMap); //Normal cube mapping

					if (gFilterRenderShadow == true)
						mFX_pShadowMap->SetResource(mShadow->DepthMapSRV());


					//Select object
					if (mSelectedObject != nullptr)
					{
						if (mRenderObjectsTransparencyBuffer->at(i)->GetTreeItemId() == mSelectedObject->GetTreeItemId() && gFilterDebugSelection)
						{
							mFX_AllowSpecular->SetBool(false);
							mSelectedMaterialHlsl = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl;
							mSelectedMaterialHlsl.Ambient.x = 0.2f;
							mSelectedMaterialHlsl.Ambient.y = 0.9f;
							mSelectedMaterialHlsl.Ambient.z = 0.2f;
							mFX_pMaterial->SetRawValue(&mSelectedMaterialHlsl, 0, sizeof(mSelectedMaterialHlsl));
						}
						else
						{
							mFX_pMaterial->SetRawValue(&mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl, 0,
								sizeof(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl));
						}
					}
					else
					{
						mFX_pMaterial->SetRawValue(&mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl, 0, 
							sizeof(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).materialHlsl));
					}

					//Transparency
					mFX_Transparency->SetFloat(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).transparency);


					//Get & set shader
					if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong")
						mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
					else if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong Transparency")
						mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTransparency");
					else if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Phong Tessellation")
					{
						if (gFilterRenderTessellation == true)
						{
							mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTessellation");
							m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
							g_WorldViewProjection = camera.ViewProj();
							mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
						}
						else
							mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
					}
					else if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Transparency")
						mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorranceTransparency");
					else if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Tessellation")
					{
						if (gFilterRenderTessellation == true)
						{
							mTech = mFX->GetTechniqueByName("FeatureLevel11Tessellation");
							m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
							g_WorldViewProjection = camera.ViewProj();
							mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
						}
						else
							mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");
					}
					else
						mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");

					//Wireframe
					if (gFilterDebugWireframe == true)
						m_pImmediateContext->RSSetState(mWireframeRS);
					else
						m_pImmediateContext->RSSetState(mSolidframeRS);

					//Update
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

					//Draw
					if (gFilterRenderMesh == true)//Draw mesh?
						if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH() != nullptr)
							m_pImmediateContext->DrawIndexed
							(
								mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
								mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startIndex,
								mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startVertex
								);

					//Tessellation OFF
					m_pImmediateContext->HSSetShader(0, 0, 0);
					m_pImmediateContext->DSSetShader(0, 0, 0);
				}
		}


		//-----------------------------
		//Draw BoundingBox
		//-----------------------------
		if (gFilterDebugBBoxes)
		{
			g_WorldViewProjection = XMMatrixIdentity() * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));

			//Draw boundingBox
			RebuildBoundingBox(mRenderObjectsTransparencyBuffer->at(i));//rebuild bbox
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetBBoxVertexBuffer(), &stride, &offset);
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
			mFX_SetLineColor->SetInt(5); //Yellow colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(24, 0);
		}

		//-----------------------------
		//Draw collisionMesh
		//-----------------------------
		if (gFilterDebugCollision == true)
			DrawCollisionMesh(mRenderObjectsTransparencyBuffer->at(i));

		
	}

	//-----------------------------
	// Gizmo
	//-----------------------------
	if (cubeMap == false)
	{
		m_pImmediateContext->RSSetState(mTwoSideTriangleFrameRS);
		if (mLockLeftClickMove == false) //Move, rotate or scale gizmo is NOT selected
			mGizmo->DetectRayCollision(&mCam, &mMousePosition, &m_worldBaseTransform, m_ClientWidth, m_ClientHeight);
		if (mSelectedObject != nullptr)
			mGizmo->Draw(&mCam, mSelectedObject->GetCoordinate(), mFX_pWorld, mFX_pWorldViewProjection, mTech, mFX, mDepthStencilView, mFX_SetLineColor, mFX_pWorldInvTranspose, m_ClientWidth, m_ClientHeight);
	}



	//if (__showShadowMap)
	//{
		//Render quads:
	//	m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//	DrawScreenQuad();
	//}
}


//************************************************************************************************************************************************
//************************************************************************************************************************************************
void KalenEngine::RenderShadowMap()
{

	XMMATRIX view = XMLoadFloat4x4(&mLightView);
	XMMATRIX proj = XMLoadFloat4x4(&mLightProj);
	//proj = XMMatrixInverse(nullptr, proj);
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);


	float distance = 0.0;
	XMVECTOR distanceVector;


	//TESSELLATION OFF
	m_pImmediateContext->HSSetShader(0, 0, 0);
	m_pImmediateContext->DSSetShader(0, 0, 0);




	//-------------
	//Draw all mesh from mRenderObjectsBuffer
	//-------------
	for (int i = 0; i < (int)mRenderObjectsBuffer->size(); ++i)
	{
		if (mRenderObjectsBuffer->at(i)->GetType() == 0)//Render only KLMESH
		{
			if (mRenderObjectsBuffer->at(i)->GetIsVisible() == true)
				if (mRenderObjectsBuffer->at(i)->GetIsCastShadow() == true)
			{
				m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsBuffer->at(i)->GetKLMESH()->GetVertexBuffer(), &stride, &offset);
				m_pImmediateContext->IASetIndexBuffer(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
				m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//Tessellation OFF
				m_pImmediateContext->HSSetShader(0, 0, 0);
				m_pImmediateContext->DSSetShader(0, 0, 0);


				mFXShadow_pEyePosition->SetRawValue(&mCam.GetPosition(), 0, sizeof(mCam.GetPosition()));
				mFXShadow_pViewProjection->SetMatrix(reinterpret_cast<const float*>(&viewProj));

				//Update object coordinates
				mRenderObjectsBuffer->at(i)->GetCoordinate()->UpdateCoordinateMatrix();

				XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsBuffer->at(i)->GetCoordinate()->GetMatrix());

				mFXShadow_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));
				g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
				mFXShadow_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));
				g_WorldViewProjection = odwrotnoscMacierzyPrzejscia *view* proj;
				mFXShadow_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


				//Distance to object 
				distanceVector = XMVector3Length((mCam.GetPositionXM() - mRenderObjectsBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
				distance = XMVectorGetX(distanceVector);


				if (distance <= mRenderObjectsBuffer->at(i)->GetShadowDistance())
				{

					int matPos = -1;
					if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance) != nullptr)//If the object is visible
						for (int j = 0; j < (int)mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->size(); ++j)//rendering all chunks of object
						{
							matPos = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->materialPosition;

							mTechShadow = mFXShadow->GetTechniqueByName("BuildShadowMapTechAlpha");
							//Set Resources Textures
							mFXShadow_pAlphaMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap); //RGB_A

							//Update
							mTechShadow->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

							//Draw
							if (gFilterRenderMesh == true)//Draw mesh?
								if (mRenderObjectsBuffer->at(i)->GetKLMESH() != nullptr)
									m_pImmediateContext->DrawIndexed
									(
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startIndex,
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startVertex
										);

							//Tessellation OFF
							m_pImmediateContext->HSSetShader(0, 0, 0);
							m_pImmediateContext->DSSetShader(0, 0, 0);
						}
				}
			}

		}
	}
	//-------------
	//Draw all mesh from mRenderObjectsTransparencyBuffer
	//-------------
	for (int i = 0; i < (int)mRenderObjectsTransparencyBuffer->size(); ++i)
	{
		if (mRenderObjectsTransparencyBuffer->at(i)->GetType() == 0)//Render only KLMESH
		{
			if (mRenderObjectsTransparencyBuffer->at(i)->GetIsVisible() == true)
				if (mRenderObjectsTransparencyBuffer->at(i)->GetIsCastShadow() == true)
				{
					m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetVertexBuffer(), &stride, &offset);
					m_pImmediateContext->IASetIndexBuffer(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					//Tessellation OFF
					m_pImmediateContext->HSSetShader(0, 0, 0);
					m_pImmediateContext->DSSetShader(0, 0, 0);


					mFXShadow_pEyePosition->SetRawValue(&mCam.GetPosition(), 0, sizeof(mCam.GetPosition()));
					mFXShadow_pViewProjection->SetMatrix(reinterpret_cast<const float*>(&viewProj));

					//Update object coordinates
					mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->UpdateCoordinateMatrix();

					XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->GetMatrix());

					mFXShadow_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));
					g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
					mFXShadow_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));
					g_WorldViewProjection = odwrotnoscMacierzyPrzejscia *view* proj;
					mFXShadow_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


					//Distance to object 
					distanceVector = XMVector3Length((mCam.GetPositionXM() - mRenderObjectsTransparencyBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
					distance = XMVectorGetX(distanceVector);


					if (distance <= mRenderObjectsTransparencyBuffer->at(i)->GetShadowDistance())
					{

						int matPos = -1;
						if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance) != nullptr)//If the object is visible
							for (int j = 0; j < (int)mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->size(); ++j)//rendering all chunks of object
							{
								matPos = mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->materialPosition;

								mTechShadow = mFXShadow->GetTechniqueByName("BuildShadowMapTechAlpha");
								//Set Resources Textures
								mFXShadow_pAlphaMap->SetResource(mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap); //RGB_A

								//Update
								mTechShadow->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

								//Draw
								if (gFilterRenderMesh == true)//Draw mesh?
									if (mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH() != nullptr)
										m_pImmediateContext->DrawIndexed
										(
											mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
											mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startIndex,
											mRenderObjectsTransparencyBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startVertex
											);

								//Tessellation OFF
								m_pImmediateContext->HSSetShader(0, 0, 0);
								m_pImmediateContext->DSSetShader(0, 0, 0);
							}
					}
				}

		}
	}









}


void KalenEngine::DrawBoundingBox(Object* object)
{
	mCam.UpdateViewMatrix();
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();


	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVerticesBufferBoundingBox, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pIndicesBufferBoundingBox, DXGI_FORMAT_R16_UINT, 0);


	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//TESSELLATION OFF
	m_pImmediateContext->HSSetShader(0, 0, 0);
	m_pImmediateContext->DSSetShader(0, 0, 0);

	mTech = mFX->GetTechniqueByName("FeatureLevel11RedWireFrame");

	XMFLOAT4X4 matrix;
	XMMATRIX macierzPrzejscia2 = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix, macierzPrzejscia2);

	matrix(3, 0) = object->GetCoordinate()->GetLocalCoordinatePosition().x;
	matrix(3, 1) = object->GetCoordinate()->GetLocalCoordinatePosition().y;
	matrix(3, 2) = object->GetCoordinate()->GetLocalCoordinatePosition().z;

	macierzPrzejscia2 = XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix));
	//mFXTess_pWorld->SetMatrix(reinterpret_cast<float*>(&macierzPrzejscia2));
	g_WorldInvTranspose = MathHelper::InverseTranspose(macierzPrzejscia2);
	//mFXTess_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));


	g_WorldViewProjection = macierzPrzejscia2  * view * proj;
	//mFXTess_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));



	m_pImmediateContext->RSSetState(mWireThroughFrameRS);
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(36, 0, 0); 
}


//************************************************************************************************************************************************
//************************************************************************************************************************************************
void KalenEngine::DrawCollisionMesh(Object* object)
{
	mCam.UpdateViewMatrix();
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();

	m_pImmediateContext->IASetVertexBuffers(0, 1, object->GetKLMESH()->GetVertexBufferCollsion(), &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(object->GetKLMESH()->GetIndexBufferCollsion(), DXGI_FORMAT_R16_UINT, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//TESSELLATION OFF
	m_pImmediateContext->HSSetShader(0, 0, 0);
	m_pImmediateContext->DSSetShader(0, 0, 0);

	mTech = mFX->GetTechniqueByName("FeatureLevel11COLLISION_RENDER");

	//mLocalCoordinateSystem->UpdateCoordinateMatrix();
	XMMATRIX macierzPrzejscia2 = XMMatrixInverse(nullptr, object->GetCoordinate()->GetMatrix());
	//XMMATRIX macierzPrzejscia2 = object->GetCoordinate()->GetMatrix();

	mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&macierzPrzejscia2));
	g_WorldInvTranspose = MathHelper::InverseTranspose(macierzPrzejscia2);
	mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));

	g_WorldViewProjection = macierzPrzejscia2  * view * proj;
	mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));

	m_pImmediateContext->RSSetState(mSolidframeRS);
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(object->GetKLMESH()->GetMeshCollision().Indices.size(), 0, 0);

	mTech = mFX->GetTechniqueByName("FeatureLevel11RedWireFrame");
	m_pImmediateContext->RSSetState(mWireframeRS);
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(object->GetKLMESH()->GetMeshCollision().Indices.size(), 0, 0);
}




//************************************************************************************************************************************************
//************************************************************************************************************************************************

void KalenEngine::DrawScreenQuad()
{
	//Set vertices & indices buffer
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVerticesBufferQuad, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pIndicesBufferQuad, DXGI_FORMAT_R16_UINT, 0);

	m_pImmediateContext->RSSetState(mSolidframeRS);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//TESSELLATION OFF
	m_pImmediateContext->HSSetShader(0, 0, 0);
	m_pImmediateContext->DSSetShader(0, 0, 0);


	XMMATRIX world(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f
		);

	mTech = mFX->GetTechniqueByName("FeatureLevel11_NEW");

	mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&world));
	mFX_pDiffuseMap->SetResource(mShadow->DepthMapSRV());
	//mFX_pDiffuseMap->SetResource(mSSAO->AmbientSRV());//testView
	//mFX_pDiffuseMap->SetResource(testView);//testView
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(6, 0, 0); //only two triangles

}

//**********************************************************************************************************************************************************************************
void KalenEngine::RebuildBoundingBox(Object* object)
{
	if (object->GetType() == 0)//Only KLMESH
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_pImmediateContext->Map(*object->GetKLMESH()->GetBBoxVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);

		XMFLOAT3 vectorMin = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 vectorMax = XMFLOAT3(0.0f, 0.0f, 0.0f);

		XMVECTOR rotation;
		XMVECTOR translation;
		XMVECTOR scale;

		object->GetCoordinate()->UpdateLocalCoordinateMatrix();

		//XMMatrixInverse(nullptr, object->GetCoordinate()->GetMatrix());
		//XMMatrixDecompose(&temp, &rotation, &temp, object->GetCoordinate()->GetMatrix());
		XMMatrixDecompose(&scale, &rotation, &translation, XMMatrixInverse(nullptr, object->GetCoordinate()->GetMatrix()));

		//	temp = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));

			/*XMStoreFloat3(&vectorMax, scale);
			vectorMax.x = 1.0f / vectorMax.x;
			vectorMax.y = 1.0f / vectorMax.y;
			vectorMax.z = 1.0f / vectorMax.z;

			scale = XMLoadFloat3(&vectorMax);

			vectorMax = XMFLOAT3(0.0f, 0.0f, 0.0f);*/

			//Counting new max and min vector of this mesh
		AxisAlignedBox AABBTemp;
		TransformAxisAlignedBox(&AABBTemp, object->GetKLMESH()->GetAxisAlignedBoxOrigin(), 1.0f, scale, rotation, translation);

		vectorMax.x = AABBTemp.Center.x + AABBTemp.Extents.x;
		vectorMax.y = AABBTemp.Center.y + AABBTemp.Extents.y;
		vectorMax.z = AABBTemp.Center.z + AABBTemp.Extents.z;

		vectorMin.x = AABBTemp.Center.x - AABBTemp.Extents.x;
		vectorMin.y = AABBTemp.Center.y - AABBTemp.Extents.y;
		vectorMin.z = AABBTemp.Center.z - AABBTemp.Extents.z;

		object->SetAxisAlignedBoundingBox(AABBTemp);

		v[0].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
		v[1].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);

		v[2].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
		v[3].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);

		v[4].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
		v[5].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);

		v[6].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
		v[7].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);

		v[8].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
		v[9].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);

		v[10].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
		v[11].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);

		v[12].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
		v[13].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);

		v[14].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
		v[15].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);

		v[16].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
		v[17].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);

		v[18].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
		v[19].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);

		v[20].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
		v[21].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);

		v[22].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
		v[23].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);

		m_pImmediateContext->Unmap(*object->GetKLMESH()->GetBBoxVertexBuffer(), 0);
	}
}

void KalenEngine::RenderSSAOMap()
{

	mCam.UpdateViewMatrix();
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	float distance = 0.0;
	XMVECTOR distanceVector;






	//----------------------
	// Set mRenderObjectsBuffer
	//----------------------
	mRenderObjectsBuffer->clear();
	mRenderObjectsTransparencyBuffer->clear();
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
		for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
		{
			bool isTransparency = false;
			if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0)//only KLMESH
			{
				for (int k = 0; k < (int)mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->size(); ++k)//materials
					if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->at(k).shaderName == "Phong Transparency"
						||
						mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetKLMESH()->GetMaterialBuffer()->at(k).shaderName == "Cook-Torrance Transparency"
						)
						isTransparency = true;
			}
			else
				isTransparency = false;

			if (isTransparency == false)
				mRenderObjectsBuffer->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));
			else
				mRenderObjectsTransparencyBuffer->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));//add to transparency buffer
		}











	mSSAO_ND_NormalDepthTech = mFX_mSSAO_ND_NormalDepthTech->GetTechniqueByName("NormalDepthAlphaClip");

	//-------------
	//Draw all mesh from mRenderObjectsBuffer
	//-------------
	for (int i = 0; i < (int)mRenderObjectsBuffer->size(); ++i)
	{
		if (mRenderObjectsBuffer->at(i)->GetType() == 0)//Render only KLMESH
		{
			if (mRenderObjectsBuffer->at(i)->GetIsVisible() == true)
				{
					m_pImmediateContext->IASetVertexBuffers(0, 1, mRenderObjectsBuffer->at(i)->GetKLMESH()->GetVertexBuffer(), &stride, &offset);
					m_pImmediateContext->IASetIndexBuffer(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					//Tessellation OFF
					m_pImmediateContext->HSSetShader(0, 0, 0);
					m_pImmediateContext->DSSetShader(0, 0, 0);

					//Update object coordinates
					mRenderObjectsBuffer->at(i)->GetCoordinate()->UpdateCoordinateMatrix();
					XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mRenderObjectsBuffer->at(i)->GetCoordinate()->GetMatrix());

					mSSAO_ND_WorldView->SetMatrix(reinterpret_cast<const float*>(&(odwrotnoscMacierzyPrzejscia * view)));

					g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
					mSSAO_ND_WorldInvTransposeView->SetMatrix(reinterpret_cast<const float*>(&(g_WorldInvTranspose * view)));

					g_WorldViewProjection = odwrotnoscMacierzyPrzejscia *view* proj;
					mSSAO_ND_WorldViewProj->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


					mSSAO_ND_TexTransform->SetMatrix(reinterpret_cast<const float*>(&XMMatrixIdentity()));


					//Distance to object 
					distanceVector = XMVector3Length((mCam.GetPositionXM() - mRenderObjectsBuffer->at(i)->GetCoordinate()->GetLocalCoordinatePositionVECTOR()));
					distance = XMVectorGetX(distanceVector);
					int matPos = -1;
					if (mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance) != nullptr)//If the object is visible
						for (int j = 0; j < (int)mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->size(); ++j)//rendering all chunks of object
						{
							matPos = mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->materialPosition;
							//Set Resources Textures
							mSSAO_ND_DiffuseMap->SetResource(mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMaterialBuffer()->at(matPos).diffuseMap); //RGB_A

							//Update
							mSSAO_ND_NormalDepthTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

							mSSAO_ND_NormalDepthTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

							//Draw
							if (gFilterRenderMesh == true)//Draw mesh?
								if (mRenderObjectsBuffer->at(i)->GetKLMESH() != nullptr)
									m_pImmediateContext->DrawIndexed
									(
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startIndex,
										mRenderObjectsBuffer->at(i)->GetKLMESH()->GetMeshToDraw(distance)->at(j)->startVertex
										);
						}
					
				}

		}
	}


	









}
