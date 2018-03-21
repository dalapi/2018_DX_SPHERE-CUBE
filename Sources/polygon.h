////////////////////////////////////////////////////////////////////////
//
//	ポリゴン描画
//														2017.06.05 ちん
////////////////////////////////////////////////////////////////////////

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
#include "Texture.h"

/*
// 配列数える方法
// 直接にマクロ定義しないでもできる（sizeof）
#define ARRAYSCOUNT(x)　sizeof(x)/sizeof(x[0])
static const int TEXTURE_COUNT = ARRAYSIZE(g_aTexture);
*/

bool InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void SetPolygonColor(D3DCOLOR color);
void SetPolygonScale(float sx, float sy);
void SetPolygonRotation(float cx, float cy, float angle);
void Set2DTexture(TEXTURE_ENUM TexNum);
void DrawPolygon( float dx, float dy,  // ポリゴンの幅と高さ
				  int tcx, int tcy, int tcw, int tch, bool bAffine); //Texture指定ところと幅と高さ


#endif // _POLYGON_H_