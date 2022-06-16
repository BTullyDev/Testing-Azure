#ifndef PLAY_PLAYBLITTER_H
#define PLAY_PLAYBLITTER_H
//********************************************************************************************************************************
// File:		PlayBlitter.h
// Description:	A software pixel renderer for drawing 2D primitives into a PixelData buffer
// Platform:	Independent
//********************************************************************************************************************************

// A software pixel renderer for drawing 2D primitives into a PixelData buffer
// > A singleton class accessed using PlayBlitter::Instance()
class PlayBlitter
{
public:

	// Constructor and initialisation
	//********************************************************************************************************************************

	// Constructor
	PlayBlitter( PixelData* pRenderTarget = nullptr );
	// Set the render target for all subsequent drawing operations
	// Returns a pointer to any previous render target
	PixelData* SetRenderTarget( PixelData* pRenderTarget ) { PixelData* old = m_pRenderTarget; m_pRenderTarget = pRenderTarget; return old; }

	// Primitive drawing functions
	//********************************************************************************************************************************

	// Sets the colour of an individual pixel on the render target
	template< typename TBlend > void DrawPixel( int posX, int posY, Pixel pix );
	// Sets the colour of an individual pixel on the render target
	template< typename TBlend > void DrawPixelPreMult( int posX, int posY, Pixel pix );
	// Draws pixel data to the render target using a direct copy
	// > Setting alphaMultiply < 1 forces a less optimal rendering approach (~50% slower) 
	template< typename TBlend > void BlitPixels( const PixelData& srcImage, int srcOffset, int blitX, int blitY, int blitWidth, int blitHeight, BlendColour globalMultiply ) const;
	// Draws rotated and scaled pixel data to the render target (much slower than BlitPixels)
	// > Setting alphaMultiply isn't a signfiicant additional slow down on RotateScalePixels
	template< typename TBlend > void RotateScalePixels( const PixelData& srcPixelData, int srcOffset, int blitX, int blitY, int blitWidth, int blitHeight, int originX, int originY, float angle, float scale, BlendColour globalMultiply ) const;
	// Clears the render target using the given pixel colour
	void ClearRenderTarget( Pixel colour );
	// Copies a background image of the correct size to the render target
	void BlitBackground( PixelData& backgroundImage );

private:

	PixelData* m_pRenderTarget{ nullptr };

};

//********************************************************************************************************************************
// Function:	BlitPixels - draws image data with and without a global alpha multiply
// Parameters:	spriteId = the id of the sprite to draw
//				xpos, ypos = the position you want to draw the sprite
//				frameIndex = which frame of the animation to draw (wrapped)
// Notes:		Blend implmentation depends on TBlend class (see PlayBlends.h) - should all end up inlined!
//********************************************************************************************************************************
template< typename TBlend > void PlayBlitter::BlitPixels( const PixelData& srcPixelData, int srcOffset, int blitX, int blitY, int blitWidth, int blitHeight, BlendColour globalMultiply ) const
{
	PLAY_ASSERT_MSG( m_pRenderTarget, "Render target not set for PlayBlitter" );

	// Nothing within the display buffer to draw
	if( blitX > m_pRenderTarget->width || blitX + blitWidth < 0 || blitY > m_pRenderTarget->height || blitY + blitHeight < 0 )
		return;

	// Work out if we need to clip to the display buffer (and by how much)
	int xClipStart = -blitX;
	if( xClipStart < 0 ) { xClipStart = 0; }

	int xClipEnd = ( blitX + blitWidth ) - m_pRenderTarget->width;
	if( xClipEnd < 0 ) { xClipEnd = 0; }

	int yClipStart = -blitY;
	if( yClipStart < 0 ) { yClipStart = 0; }

	int yClipEnd = ( blitY + blitHeight ) - m_pRenderTarget->height;
	if( yClipEnd < 0 ) { yClipEnd = 0; }

	// Set up the source and destination pointers based on clipping
	int destOffset = ( m_pRenderTarget->width * ( blitY + yClipStart ) ) + ( blitX + xClipStart );
	uint32_t* destPixels = &m_pRenderTarget->pPixels->bits + destOffset;

	int srcClipOffset = ( srcPixelData.width * yClipStart ) + xClipStart;
	uint32_t* srcPixels = &srcPixelData.pPixels->bits + srcOffset + srcClipOffset;

	// Work out in advance how much we need to add to src and dest to reach the next row 
	int destInc = m_pRenderTarget->width - blitWidth + xClipEnd + xClipStart;
	int srcInc = srcPixelData.width - blitWidth + xClipEnd + xClipStart;

	//Work out final pixel in destination.
	int destColOffset = ( m_pRenderTarget->width * ( blitHeight - yClipEnd - yClipStart - 1 ) ) + ( blitWidth - xClipEnd - xClipStart );
	uint32_t* destColEnd = destPixels + destColOffset;

	//How many pixels per row in sprite.
	int endRow = blitWidth - xClipEnd - xClipStart;

	if( globalMultiply.alpha < 1.0f )
	{
		// Slightly more optimised to loop through without the additions 
		while( destPixels < destColEnd )
		{
			uint32_t* destRowEnd = destPixels + endRow;

			while( destPixels < destRowEnd )
				TBlend::BlendSkip( srcPixels, destPixels, globalMultiply, destRowEnd );

			// Increase buffers by pre-calculated amounts
			destPixels += destInc;
			srcPixels += srcInc;
		}
	}
	else
	{
		// Slightly more optimised to loop through without the additions 
		while( destPixels < destColEnd )
		{
			uint32_t* destRowEnd = destPixels + endRow;

			while( destPixels < destRowEnd )
				TBlend::BlendFastSkip( srcPixels, destPixels, destRowEnd );

			// Increase buffers by pre-calculated amounts
			destPixels += destInc;
			srcPixels += srcInc;
		}
	}

	return;
}

//********************************************************************************************************************************
// Function:	RotateScaleSprite - draws a rotated and scaled sprite with global alpha multiply
// Parameters:	s = the sprite to draw
//				xpos, ypos = the position of the center of rotation.
//				frameIndex = which frame of the animation to draw (wrapped)
//				angle = rotation angle
//				scale = parameter to magnify the sprite.
//				rotOffX, rotOffY = offset of centre of rotation to the top left of the sprite
//				alpha = the fraction defining the amount of sprite and background that is draw. 255 = all sprite, 0 = all background.
// Notes:		Pre-calculates roughly where the sprite will be in the display buffer and only processes those pixels. 
//				Approx 15 times slower than not rotating. Blend implmentation depends on TBlend class (see PlayBlends.h)
//********************************************************************************************************************************
template< typename TBlend >
void PlayBlitter::RotateScalePixels( const PixelData& srcPixelData, int srcOffset, int blitX, int blitY, int blitWidth, int blitHeight, int originX, int originY, float angle, float scale, BlendColour globalMultiply ) const
{
	PLAY_ASSERT_MSG( m_pRenderTarget, "Render target not set for PlayBlitter" );

	//pointers to start of source and destination buffers
	uint32_t* pSrcBase = &srcPixelData.pPixels->bits + srcOffset;
	uint32_t* pDstBase = &m_pRenderTarget->pPixels->bits;

	//the centre of rotation in the sprite frame relative to the top corner
	float fRotCentreU = static_cast<float>( originX );
	float fRotCentreV = static_cast<float>( originY );

	//u/v are co-ordinates in the rotated sprite frame. x/y are screen buffer co-ordinates.
	//change in u/v for a unit change in x/y.
	float dUdX = static_cast<float>( cos( -angle ) ) * ( 1.0f / scale );
	float dVdX = static_cast<float>( sin( -angle ) ) * ( 1.0f / scale );
	float dUdY = -dVdX;
	float dVdY = dUdX;

	//Position in the sprite rotated frame with origin at the center of rotation of the sprite corners.
	float leftU = -fRotCentreU * scale;
	float rightU = ( blitWidth + -fRotCentreU ) * scale;
	float topV = ( -fRotCentreV ) * scale;
	float bottomV = ( blitHeight - fRotCentreV ) * scale;

	//Scale added in to cancel out the scale in dUdX.
	float boundingBoxCorners[4][2]
	{
		{ ( dUdX * leftU + dVdX * topV ) * scale,			( dUdY * leftU + dVdY * topV ) * scale		},	// Top left
		{ ( dUdX * leftU + dVdX * bottomV ) * scale,		( dUdY * leftU + dVdY * bottomV ) * scale		},	// Bottom left
		{ ( dUdX * rightU + dVdX * bottomV ) * scale,		( dUdY * rightU + dVdY * bottomV ) * scale	},	// Bottom right
		{ ( dUdX * rightU + dVdX * topV ) * scale,			( dUdY * rightU + dVdY * topV ) * scale,		},	// Top right
	};

	float minX = std::numeric_limits<float>::infinity();
	float minY = std::numeric_limits<float>::infinity();
	float maxX = -std::numeric_limits<float>::infinity();
	float maxY = -std::numeric_limits<float>::infinity();

	//calculate the extremes of the rotated corners.
	for( int i = 0; i < 4; i++ )
	{
		minX = std::min( minX, boundingBoxCorners[i][0] );
		maxX = std::max( maxX, boundingBoxCorners[i][0] );
		minY = std::min( minY, boundingBoxCorners[i][1] );
		maxY = std::max( maxY, boundingBoxCorners[i][1] );
	}

	//clip the starting and finishing positions.
	int startY = blitY + static_cast<int>( minY );
	if( startY < 0 ) { startY = 0; minY = static_cast<float>( -blitY ); }

	int endY = blitY + static_cast<int>( maxY );
	if( endY > m_pRenderTarget->height ) { endY = m_pRenderTarget->height; }

	int startX = blitX + static_cast<int>( minX );
	if( startX < 0 ) { startX = 0; minX = static_cast<float>( -blitX ); }

	int endX = blitX + static_cast<int>( maxX );
	if( endX > m_pRenderTarget->width ) { endX = m_pRenderTarget->width; }

	//rotate the basis so we get the edge of the bounding box in the sprite frame.
	float startingU = dUdX * minX + dUdY * minY + fRotCentreU;
	float startingV = dVdY * minY + dVdX * minX + fRotCentreV;

	float rowU = startingU;
	float rowV = startingV;

	uint32_t* destPixels = pDstBase + ( static_cast<size_t>( m_pRenderTarget->width ) * startY ) + startX;
	int nextRow = m_pRenderTarget->width - ( endX - startX );

	uint32_t* srcPixels = pSrcBase;

	//Start of double for loop. 
	for( int y = startY; y < endY; y++ )
	{
		float u = rowU;
		float v = rowV;

		for( int x = startX; x < endX; x++ )
		{
			//Check to see if u and v correspond to a valid pixel in sprite.
			if( u > 0 && v > 0 && u < blitWidth && v < blitHeight )
			{
				srcPixels = pSrcBase + static_cast<size_t>( u ) + ( static_cast<size_t>( v ) * srcPixelData.width );
				TBlend::Blend( srcPixels, destPixels, globalMultiply );
			}

			destPixels++;

			// Change the position in the sprite frame for changing X in the display
			u += dUdX;
			v += dVdX;
		}

		// Work out the change in the sprite frame for changing Y in the display
		rowU += dUdY;
		rowV += dVdY;
		// Next row
		destPixels += nextRow;
	}

}

template< typename TBlend > void PlayBlitter::DrawPixelPreMult( int posX, int posY, Pixel srcPixel )
{
	if( srcPixel.a == 0x00 || posX < 0 || posX >= m_pRenderTarget->width || posY < 0 || posY >= m_pRenderTarget->height )
		return;

	// Pre-multiply alpha and invert
	srcPixel.r = ( srcPixel.r * srcPixel.a) >> 8;
	srcPixel.g = ( srcPixel.g * srcPixel.a ) >> 8;
	srcPixel.b = ( srcPixel.b * srcPixel.a ) >> 8;
	srcPixel.a = 0xFF - srcPixel.a;

	uint32_t* pDest = &m_pRenderTarget->pPixels[( posY * m_pRenderTarget->width ) + posX].bits;
	uint32_t* pSrc = &srcPixel.bits;

	TBlend::Blend( pSrc, pDest, { 1.0f, 1.0f, 1.0f, 1.0f } );

	return;
}

template< typename TBlend > void PlayBlitter::DrawPixel( int posX, int posY, Pixel srcPixel )
{
	if( srcPixel.a == 0x00 || posX < 0 || posX >= m_pRenderTarget->width || posY < 0 || posY >= m_pRenderTarget->height )
		return;

	uint32_t* pDest = &m_pRenderTarget->pPixels[( posY * m_pRenderTarget->width ) + posX].bits;
	uint32_t* pSrc = &srcPixel.bits;

	TBlend::Blend( pSrc, pDest, { 1.0f, 1.0f, 1.0f, 1.0f } );

	return;
}




#endif