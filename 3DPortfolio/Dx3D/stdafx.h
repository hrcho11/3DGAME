// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <assert.h>

#include <d3dx9.h>
#include <windowsx.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern HWND g_hWnd;

//�˰��� §�Ÿ� �����ִ� �����ι�. �ּ��� Ǯ�� ����Ʈ��, ��ŷ�� �ٴ� ���� ������
//#define SHOW_ALGORITHM_HYUNJAE


//	1. Position ������ ��ǥ x,y,z(float)            : D3DFVF_XYZ
// 	2. RHW (float)                                  : D3DFVF_XYZRHW (D3DFVF_XYZ �Ǵ� D3DFVF_NORMAL�� ���� ���Ұ�)
// 	3. Blending Weight Data ���� ����ġ (float)		: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 	4. Vertex Normal ������ ���� ���� x,y,z(float)	: D3DFVF_NORMAL
// 	5. Vertex Point Size ������ �� ũ�� (float)		: D3DFVF_PSIZE
// 	6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE
// 	7. Specular Color (DWORD)						: D3DFVF_SPECULAR
// 	8. Texture Coordinate Set 1 (float)             : D3DFVF_TEX0 - D3DFVF_TEX8

struct ST_PC_VERTEX
{
	ST_PC_VERTEX() {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	D3DXVECTOR3 p;
	D3DCOLOR	c;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct ST_PN_VERTEX
{
	ST_PN_VERTEX() {}
	ST_PN_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n) : p(_p), n(_n) {}

	D3DXVECTOR3 p;
	D3DXVECTOR3	n;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL, };
};

struct ST_PNT_VERTEX
{
	ST_PNT_VERTEX() {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}

	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct ST_RHWC_VERTEX
{
	ST_RHWC_VERTEX() {}
	ST_RHWC_VERTEX(D3DXVECTOR4 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	D3DXVECTOR4 p;
	D3DCOLOR	c;

	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE, };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct ST_SIZE
{
	float fWidth;
	float fHeight;

	ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
	ST_SIZE(float w, float h) : fWidth(w), fHeight(h) {}
};

struct ST_POS_SAMPLE
{
	int				n;
	D3DXVECTOR3		v;
};

struct ST_ROT_SAMPLE
{
	int				n;
	D3DXQUATERNION	q;
};

struct ST_ASE_SCENE
{
	int nFirstFrame;
	int nLastFrame;
	int nFrameSpeed;
	int nTicksPerFrame;
};

struct ST_SPHERE
{
	bool		isInFrustum;
	float		fRadius;
	D3DXVECTOR3 vCenter;
};

#define SINGLETONE(class_name) private: \
	class_name(void);\
	~class_name(void);\
public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}

#define SAFE_RELEASE(p) if(p) {(p)->Release(); (p) = NULL;}
#define SAFE_DELETE(p) if(p) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] (p); (p) = NULL;}
#define SAFE_ADDREF(p) if(p) { (p)->AddRef(); }
#define SAFE_DESTROY(p) {if(p) {p->Destroy(); delete p; p =nullptr;}}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
		SAFE_RELEASE(varName);\
		SAFE_ADDREF(var);\
		varName = var;\
	}\
}

#include "cObject.h"
#include "cObjectManager.h"
#include "stdafxCollider.h"
#include "cAutoReleasePool.h"
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cTimeManager.h"
#include "cSkinnedMeshManager.h"
#include "cKeyManager.h"
#include "cSkillManager.h"