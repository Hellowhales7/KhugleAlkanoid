#pragma once
#include "KhuGleSprite.h"

class KhuGleBlock : public CKhuGleSprite
{
public:
	int m_Life;

public:
	KhuGleBlock();
	KhuGleBlock(int nType, int nCollisionType, CKgLine lnLine, KgColor24 fgColor, bool bFill, int nSliceOrWidth , int nLife);
	virtual void Render();
};