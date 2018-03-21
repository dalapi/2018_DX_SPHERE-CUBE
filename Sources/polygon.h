////////////////////////////////////////////////////////////////////////
//
//	�|���S���`��
//														2017.06.05 ����
////////////////////////////////////////////////////////////////////////

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
#include "Texture.h"

/*
// �z�񐔂�����@
// ���ڂɃ}�N����`���Ȃ��ł��ł���isizeof�j
#define ARRAYSCOUNT(x)�@sizeof(x)/sizeof(x[0])
static const int TEXTURE_COUNT = ARRAYSIZE(g_aTexture);
*/

bool InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void SetPolygonColor(D3DCOLOR color);
void SetPolygonScale(float sx, float sy);
void SetPolygonRotation(float cx, float cy, float angle);
void Set2DTexture(TEXTURE_ENUM TexNum);
void DrawPolygon( float dx, float dy,  // �|���S���̕��ƍ���
				  int tcx, int tcy, int tcw, int tch, bool bAffine); //Texture�w��Ƃ���ƕ��ƍ���


#endif // _POLYGON_H_