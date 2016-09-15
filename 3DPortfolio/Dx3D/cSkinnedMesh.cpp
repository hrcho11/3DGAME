#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
	, m_fRotY(0.0f)
	, m_dwCurrTrack(0)
	,m_isAtk(false)
	, m_isFireOnce(false)
	,m_fCooltime(60.f)
	,m_fTime(0.0f)
{
	cSkinnedMesh* pSkinnedMesh =  g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;
	m_stBoundingSphere = pSkinnedMesh->m_stBoundingSphere;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);
}

bool cSkinnedMesh::AtkAnimationMatch()
{
	if (!m_isFireOnce)
	{
		LPD3DXANIMATIONSET animSet;
		m_pAnimController->GetTrackAnimationSet(m_dwCurrTrack, &animSet);
		std::string str = animSet->GetName();
		if (str == "attack")
		{
			D3DXTRACK_DESC trackDesc;
			m_pAnimController->GetTrackDesc(m_dwCurrTrack, &trackDesc);
			if (trackDesc.Position >= 0.5f)
				m_isFireOnce = true;
			else 
				m_isFireOnce = false;
		}
		SAFE_RELEASE(animSet);
		return m_isFireOnce;
	}
	else
		return false;
}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
	Destroy();
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load( char* szDirectory, char* szFilename )
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	
	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	HRESULT hr = D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	m_stBoundingSphere.vCenter = (ah.GetMin() + ah.GetMax()) / 2.0f;
	m_stBoundingSphere.fRadius = D3DXVec3Length( &(ah.GetMin() - ah.GetMax()) );
	m_stBoundingSphere.isInFrustum = false;

	if( m_pmWorkingPalette )
		delete [] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[ m_dwWorkingPaletteSize];
	if( m_pmWorkingPalette == NULL )
	{
		m_dwWorkingPaletteSize = 0;
	}

	if(m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::UpdateAndRender()
{
	if(m_pAnimController)
	{
		m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}

	if(m_pRootFrame)
	{
		D3DXMATRIXA16 matWorld,matR,matT,matS;
		D3DXMatrixIdentity(&matR);
		D3DXMatrixRotationY(&matR, m_fRotY);
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		D3DXMatrixScaling(&matS, 2.0f, 2.0f, 2.0f);
		matWorld = matS*matR * matT;

		Update(m_pRootFrame, &matWorld);
		Render(m_pRootFrame);
	}

	if (m_isAtk)
	{
		m_fTime += 1.0f;
		if (m_fTime >= m_fCooltime)
		{
			m_isAtk = false;
			m_fTime = 0.0f;
		}
	}
	
}

void cSkinnedMesh::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if(pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos = 
			( LPD3DXBONECOMBINATION )( pBoneMesh->pBufBoneCombos->GetBufferPointer() );

		D3DXMATRIXA16 matViewProj, matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for( DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++ dwAttrib )
		{
			// set each transform into the palette
			for( DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++ dwPalEntry )
			{
				DWORD dwMatrixIndex = pBoneCombos[ dwAttrib ].BoneId[ dwPalEntry ];
				if( dwMatrixIndex != UINT_MAX )
				{
					m_pmWorkingPalette[ dwPalEntry ] = 
						pBoneMesh->pBoneOffsetMatrices[ dwMatrixIndex ] * 
						(*pBoneMesh->ppBoneMatrixPtrs[ dwMatrixIndex ]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray( "amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries );

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) );
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4( 500.0f, 500.0f, 500.0f, 1.0f ) );
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4( vEye, 1.0f ) );
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4( 0.53f, 0.53f, 0.53f, 0.53f ) );
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) );

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture( "g_txScene", pBoneMesh->vecTexture[ pBoneCombos[ dwAttrib ].AttribId ] );

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt( "CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1 );

			// set the technique we use to draw
			m_pEffect->SetTechnique( "Skinning20" );

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin( & uiPasses, 0 );
			for( uiPass = 0; uiPass < uiPasses; ++ uiPass )
			{
				m_pEffect->BeginPass( uiPass );
				pBoneMesh->pWorkingMesh->DrawSubset( dwAttrib );
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if(pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if(pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

LPD3DXEFFECT cSkinnedMesh::LoadEffect( char* szFilename )
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps( & caps );
	if( caps.VertexShaderVersion > D3DVS_VERSION( 1, 1 ) )
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if(FAILED(D3DXCreateEffectFromFile( g_pD3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer )))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent )
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if(pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if(pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if(pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}
}

void cSkinnedMesh::SetupBoneMatrixPtrs( ST_BONE* pBone )
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if(pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if(szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if(pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if(pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::SetAnimationIndex( int nIndex )
{
	if(!m_pAnimController)
		return;
	if (nIndex == 2)
	{
		m_isAtk = true;
		m_isFireOnce = false;
	}
	DWORD dwNewTrack = (m_dwCurrTrack == 1 ? 0 : 1);
	LPD3DXANIMATIONSET animSet = nullptr;

	m_pAnimController->GetAnimationSet(nIndex, &animSet);
	m_pAnimController->SetTrackAnimationSet(dwNewTrack, animSet);

	SAFE_RELEASE(animSet);

	m_pAnimController->UnkeyAllTrackEvents(dwNewTrack);
	m_pAnimController->UnkeyAllTrackEvents(m_dwCurrTrack);

	m_pAnimController->KeyTrackEnable(m_dwCurrTrack, FALSE, m_pAnimController->GetTime() + ANIMATION_TRANSITION_TIME);
	m_pAnimController->KeyTrackSpeed(m_dwCurrTrack, 0.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimController->KeyTrackWeight(m_dwCurrTrack, 0.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME,D3DXTRANSITION_LINEAR);

	m_pAnimController->SetTrackEnable(dwNewTrack, TRUE);
	m_pAnimController->KeyTrackSpeed(dwNewTrack, 1.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimController->KeyTrackWeight(dwNewTrack, 1.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME,D3DXTRANSITION_LINEAR);

	m_pAnimController->SetTrackPosition(dwNewTrack, 0.0f);
	m_dwCurrTrack = dwNewTrack;

}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}



void cSkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void cSkinnedMesh::AttackOnlyOnce()
{

	DWORD dwNewTrack = (m_dwCurrTrack == 1 ? 0 : 1);
	LPD3DXANIMATIONSET animSet = nullptr;

	m_pAnimController->GetAnimationSet(2, &animSet);
	m_pAnimController->SetTrackAnimationSet(dwNewTrack, animSet);

	SAFE_RELEASE(animSet);

	m_pAnimController->UnkeyAllTrackEvents(dwNewTrack);
	m_pAnimController->UnkeyAllTrackEvents(m_dwCurrTrack);

	m_pAnimController->KeyTrackEnable(m_dwCurrTrack, FALSE, m_pAnimController->GetTime() + ANIMATION_TRANSITION_TIME);
	m_pAnimController->KeyTrackSpeed(m_dwCurrTrack, 0.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimController->KeyTrackWeight(m_dwCurrTrack, 0.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_pAnimController->SetTrackEnable(dwNewTrack, TRUE);
	m_pAnimController->KeyTrackSpeed(dwNewTrack, 1.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimController->KeyTrackWeight(dwNewTrack, 1.0f, m_pAnimController->GetTime(), ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_pAnimController->SetTrackPosition(dwNewTrack, 0.0f);
	m_dwCurrTrack = dwNewTrack;

	//dwNewTrack = (m_dwCurrTrack == 1 ? 0 : 1);
	//
	//LPD3DXANIMATIONSET AtkanimSet = nullptr;
	//LPD3DXANIMATIONSET IdleAnimSet = nullptr;
	//m_pAnimController->GetAnimationSet(2, &AtkanimSet);
	//m_pAnimController->GetAnimationSet(0, &IdleAnimSet);
	//
	//m_pAnimController->SetTrackAnimationSet(dwNewTrack, IdleAnimSet);
	//
	//double startTime = m_pAnimController->GetTime() + AtkanimSet->GetPeriod();
	//
	//m_pAnimController->KeyTrackEnable(m_dwCurrTrack, false, startTime);
	//m_pAnimController->KeyTrackSpeed(m_dwCurrTrack, 0.0f, startTime, ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//m_pAnimController->KeyTrackWeight(m_dwCurrTrack, 0.0f, startTime, ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//
	//m_pAnimController->KeyTrackEnable(dwNewTrack, true, startTime);
	//m_pAnimController->KeyTrackSpeed(dwNewTrack, 1.0f, startTime, ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//m_pAnimController->KeyTrackWeight(dwNewTrack, 1.0f, startTime, ANIMATION_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	//
	//m_pAnimController->SetTrackPosition(dwNewTrack, 0.0f);
	//m_dwCurrTrack = dwNewTrack;
	//
	//SAFE_RELEASE(AtkanimSet);
	//SAFE_RELEASE(IdleAnimSet);
	//

}
