#include "StdAfx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"


cObjLoader::cObjLoader(void)
{
}

cObjLoader::~cObjLoader(void)
{
}

void cObjLoader::Load( IN char* szFullPath,
	IN D3DXMATRIXA16* pmat,
	OUT std::vector<cGroup*>& vecGroup )
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<ST_PNT_VERTEX> vecVertex;

	std::string sMtlName;

	FILE* fp = NULL;
	fopen_s(&fp, szFullPath, "r");

	while(true)
	{
		if(feof(fp))
			break;

		char szTemp[1024];

		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			char szMtlPath[1024];
			sscanf(szTemp, "%*s %s", szMtlPath);
			LoadMtlLib(szMtlPath);
		}
		else if (szTemp[0] == 'g')
		{
			if(vecVertex.empty())
				continue;
			cGroup* pGroup = new cGroup;
			pGroup->SetVertex(vecVertex);
			pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
			vecGroup.push_back(pGroup);
			vecVertex.clear();
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == 't')
			{
				float u, v;
				sscanf(szTemp, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szTemp[1] == 'n')
			{
				float x, y, z;
				sscanf(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else // v
			{
				float x, y, z;
				sscanf(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf(szTemp, "%*s %s", szMtlName);
			sMtlName = szMtlName;
		}
		else if (szTemp[0] == 'f')
		{
			int aIndex[3][3];
			sscanf(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				v.n = vecVN[aIndex[i][2] - 1];

				if (pmat)
				{
					D3DXVec3TransformCoord(&v.p, &v.p, pmat);
					D3DXVec3TransformNormal(&v.n, &v.n, pmat);
				}
				vecVertex.push_back(v);
			}
		}
	}
	
	for each(auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}

	m_mapMtlTex.clear();

	fclose(fp);
}

void cObjLoader::LoadMtlLib( char* szFullPath )
{
	FILE* fp = NULL;
	fopen_s(&fp, szFullPath, "r");

	std::string sMtlName;
	int nMtlRef = 0;

	while(true)
	{
		if(feof(fp))
			break;

		char szTemp[1024];

		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
			sscanf(szTemp, "%*s %s", szMtlName);
			sMtlName = szMtlName;

			SAFE_RELEASE(m_mapMtlTex[sMtlName]);
			m_mapMtlTex[sMtlName] = new cMtlTex;
			m_mapMtlTex[sMtlName]->SetMtlRef(nMtlRef++);
		}
		else if (szTemp[0] == 'K')
		{
			if (szTemp[1] == 'a')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Ambient.r = r;
				stMtl.Ambient.g = g;
				stMtl.Ambient.b = b;
				stMtl.Ambient.a = 1.0f;
			}
			else if (szTemp[1] == 'd')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Diffuse.r = r;
				stMtl.Diffuse.g = g;
				stMtl.Diffuse.b = b;
				stMtl.Diffuse.a = 1.0f;
			}
			else if (szTemp[1] == 's')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Specular.r = r;
				stMtl.Specular.g = g;
				stMtl.Specular.b = b;
				stMtl.Specular.a = 1.0f;
			}
		}
		else if (szTemp[0] == 'm')
		{
			char szTexturePath[1024];
			sscanf(szTemp, "%*s %s", szTexturePath);
			
			m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(szTexturePath));
		}
	}

	fclose(fp);
}

LPD3DXMESH cObjLoader::LoadMesh( IN char* szFullPath, IN D3DXMATRIXA16* pmat, OUT std::vector<cMtlTex*>& vecMtlTex )
{
	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR2>	vecVT;
	std::vector<D3DXVECTOR3>	vecVN;
	std::vector<DWORD>			vecAttrBuf;
	std::vector<ST_PNT_VERTEX>	vecVertex;

	std::string sMtlName;

	FILE* fp = NULL;
	fopen_s(&fp, szFullPath, "r");

	while(true)
	{
		if(feof(fp))
			break;

		char szTemp[1024];

		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			char szMtlPath[1024];
			sscanf(szTemp, "%*s %s", szMtlPath);
			LoadMtlLib(szMtlPath);

			vecMtlTex.resize(m_mapMtlTex.size());
			for each(auto it in m_mapMtlTex)
			{
				vecMtlTex[it.second->GetMtlRef()] = it.second;
			}
		}
		else if (szTemp[0] == 'g')
		{
			if(vecVertex.empty())
				continue;
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == 't')
			{
				float u, v;
				sscanf(szTemp, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szTemp[1] == 'n')
			{
				float x, y, z;
				sscanf(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else // v
			{
				float x, y, z;
				sscanf(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf(szTemp, "%*s %s", szMtlName);
			sMtlName = szMtlName;
		}
		else if (szTemp[0] == 'f')
		{
			int aIndex[3][3];
			sscanf(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			vecAttrBuf.push_back(m_mapMtlTex[sMtlName]->GetMtlRef());

			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				v.n = vecVN[aIndex[i][2] - 1];

				if (pmat)
				{
					D3DXVec3TransformCoord(&v.p, &v.p, pmat);
					D3DXVec3TransformNormal(&v.n, &v.n, pmat);
				}
				vecVertex.push_back(v);
			}
		}
	}

	LPD3DXMESH pMesh = NULL;

	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&pMesh);

	ST_PNT_VERTEX* pV = NULL;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttrBuf[0], vecAttrBuf.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuf(pMesh->GetNumFaces() * 3);
	pMesh->GenerateAdjacency(0.0f, &vecAdjBuf[0]);

	pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuf[0], 0, 0, 0);

	fclose(fp);

	return pMesh;
}
