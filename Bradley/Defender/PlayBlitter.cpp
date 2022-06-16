//********************************************************************************************************************************
// File:		PlayBlitter.cpp
// Description:	A software pixel renderer for drawing 2D primitives into a PixelData buffer
// Platform:	Independent
//********************************************************************************************************************************

#include "PlayBufferPCH.h"

PlayBlitter::PlayBlitter( PixelData* pRenderTarget )
{
	m_pRenderTarget = pRenderTarget;
}

void PlayBlitter::ClearRenderTarget( Pixel colour )
{
	Pixel* pBuffEnd = m_pRenderTarget->pPixels + ( m_pRenderTarget->width * m_pRenderTarget->height );
	for( Pixel* pBuff = m_pRenderTarget->pPixels; pBuff < pBuffEnd; *pBuff++ = colour.bits );
	m_pRenderTarget->preMultiplied = false;
}

void PlayBlitter::BlitBackground( PixelData& backgroundImage )
{
	PLAY_ASSERT_MSG( backgroundImage.height == m_pRenderTarget->height && backgroundImage.width == m_pRenderTarget->width, "Background size doesn't match render target!" );
	// Takes about 1ms for 720p screen on i7-8550U
	memcpy( m_pRenderTarget->pPixels, backgroundImage.pPixels, sizeof( Pixel ) * m_pRenderTarget->width * m_pRenderTarget->height );
}

