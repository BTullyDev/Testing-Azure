//File storing removed functions from PlayBlitter.cpp.
//Functions are less optimal verisons from the ones that are used.








//********************************************************************************************************************************
// Function:	ThreeShearRotation - draws a rotated sprite with alpha
// Parameters:	s = the sprite to draw
//				xpos, ypos = the position of the center of rotation.
//				frameIndex = which frame of the animation to draw (wrapped)
//				angle = rotation angle
//				scale = parameter to magnify the sprite.
//				rotOffX, rotOffY = offset of centre of rotation to the top left of the sprite
// Notes:		Approach that does shearing operations. Not optimal as complicated calculations done in the inner for loop.
//********************************************************************************************************************************
void PlayBlitter::ThreeShearRotation(const Sprite& s, int xpos, int ypos, int frameIndex, float angle, float scale)
{
	if (s_pDisplayBuffer == nullptr)
		return;

	//wrap frameindex so we don't try to access a frame that isn't there.
	frameIndex = frameIndex % s.nFrameCount;

	//pointer to start of display buffer (buffer[0][0])
	uint32_t* pDstBase = s_pDisplayBuffer;
	int dstW = s_nDisplayBufferWidth;
	int dstH = s_nDisplayBufferHeight;
	int dstDelta = s_nDisplayBufferWidth;

	//pointer to start of correct frame
	uint32_t* pSrcBase = s.pCanvasBuffer + (static_cast<size_t>(s.nWidth) * frameIndex);
	int srcW = s.nWidth;
	int srcH = s.nHeight;
	int srcDelta = s.nCanvasWidth;


	//initialize our parameters before iterating over u and v
	float alpha{ -tan(angle / 2) };
	float beta{ sin(angle) };

	for (int v = 0; v < srcH; v++)
	{
		for (int u = 0; u < srcW; u++)
		{
			//calculate the new v (y) value (shifted in proportion to the old u (x) value)
			//SHEAR 1
			//int v1 = static_cast<int>((v - srcH / 2.0)+ (u - srcW / 2.0) * alpha);
			int v1{ static_cast<int>((v)+(u)*alpha) };
			int u1{ static_cast<int>((u)) };
			//SHEAR 2
			int v2{ v1 };
			int u2{ static_cast<int>(u1 + v1 * beta) };
			//SHEAR 3
			int v3{ static_cast<int>(v2 + u2 * alpha) };
			int u3{ u2 };

			//check that the newly calculated position is actually on the screen
			if (v3 + ypos > 0 && v3 + ypos < dstH)
			{
				if (u3 + xpos > 0 && u3 + xpos < dstW)
				{
					uint32_t* pDest = (pDstBase + u3 + xpos + dstDelta * (v3 + ypos));
					uint32_t* pSrc = (pSrcBase + u + srcDelta * v);

					uint32_t src = *pSrc;
					uint32_t dest = *pDest;

					short srcAlpha{ src >> 24 };

					int destRed = srcAlpha * ((src >> 16) & 0xFF);
					int destGreen = srcAlpha * ((src >> 8) & 0xFF);
					int destBlue = srcAlpha * (src & 0xFF);

					short invSrcAlpha{ 0xFF - srcAlpha };

					destRed += invSrcAlpha * ((dest >> 16) & 0xFF);
					destGreen += invSrcAlpha * ((dest >> 8) & 0xFF);
					destBlue += invSrcAlpha * (dest & 0xFF);

					destRed >>= 8;
					destGreen >>= 8;
					destBlue >>= 8;

					*pDest = 0xFF000000 | (destRed << 16) | (destGreen << 8) | destBlue;
				}
			}
		}
	}


}


//********************************************************************************************************************************
// Function:	RotateScaleAlphaClip - draws a rotated sprite with alpha
// Parameters:	s = the sprite to draw
//				xpos, ypos = the position of the center of rotation.
//				frameIndex = which frame of the animation to draw (wrapped)
//				angle = rotation angle
//				scale = parameter to magnify the sprite.
//				rotOffX, rotOffY = offset of centre of rotation to the top left of the sprite
// Notes:		Approach that does goes through the display buffer but pre-calculates roughly where the sprite will be in the
//				display buffer. 
//********************************************************************************************************************************
void PlayBlitter::RotateScaleAlphaClip(const Sprite& s, int xpos, int ypos, int frameIndex, float angle, float scale)
{
	if (s_pDisplayBuffer == nullptr)
		return;

	//wrap frameindex so we don't try to access a frame that isn't there.
	frameIndex = frameIndex % s.nFrameCount;

	//pointer to start of display buffer (buffer[0][0])
	uint32_t* pDstBase = s_pDisplayBuffer;
	int dstW = s_nDisplayBufferWidth;
	int dstH = s_nDisplayBufferHeight;
	int dstDelta = s_nDisplayBufferWidth;

	//pointer to start of correct frame
	uint32_t* pSrcBase = s.pCanvasBuffer + (static_cast<size_t>(s.nWidth) * frameIndex);
	int srcW = s.nWidth;
	int srcH = s.nHeight;
	int srcDelta = s.nCanvasWidth;

	//Position in the display buffer of the center of rotation
	float fDstCX = static_cast<float>(xpos);
	float fDstCY = static_cast<float>(ypos);

	//the centre of rotation in the sprite frame relative to the top corner
	float fSrcCX = s.originX;
	float fSrcCY = s.originY;

	//u, v are co-ordinates in the rotated sprite frame. X, Y are screen buffer co-ordinates.
	//change in u for a unit change in X.
	float dudX = static_cast<float>(cos(-angle)) * (1.0f / scale);
	//change in v for a unit change in X
	float dvdX = static_cast<float>(sin(-angle)) * (1.0f / scale);
	//likewise to above.
	float dudY = -dvdX;
	float dvdY = dudX;


	//Position in the sprite rotated frame with origin at the center of rotation of the sprite corners.
	float leftSideu = -fSrcCX;
	float rightSideu = srcW + -fSrcCX;
	float topSidev = -fSrcCY;
	float bottomSidev = srcH + -fSrcCY;



	//{TOP LEFT, BOTTOM LEFT, BOTTOM RIGHT, TOP RIGHT}
	float boundingBoxCornersX[4]{ dudX * leftSideu + dvdX * topSidev,
								   dudX * leftSideu + dvdX * bottomSidev,
								   dudX * rightSideu + dvdX * bottomSidev,
								   dudX * rightSideu + dvdX * topSidev, };
	float boundingBoxCornersY[4]{ dudY * leftSideu + dvdY * topSidev,
								   dudY * leftSideu + dvdY * bottomSidev,
								   dudY * rightSideu + dvdY * bottomSidev,
								   dudY * rightSideu + dvdY * topSidev, };

	float minX{ std::numeric_limits<float>::infinity() };
	float minY{ std::numeric_limits<float>::infinity() };
	float maxX{ -std::numeric_limits<float>::infinity() };
	float maxY{ -std::numeric_limits<float>::infinity() };

	//calculate the extremes of the rotated corners.
	for (int i = 0; i < 4; i++) {
		if (boundingBoxCornersX[i] < minX) {
			minX = boundingBoxCornersX[i];
		}
		if (boundingBoxCornersX[i] > maxX) {
			maxX = boundingBoxCornersX[i];
		}
		if (boundingBoxCornersY[i] < minY) {
			minY = boundingBoxCornersY[i];
		}
		if (boundingBoxCornersY[i] > maxY) {
			maxY = boundingBoxCornersY[i];
		}
	}


	//clip the starting and finishing positions.
	int startY{ ypos + static_cast<int>(minY) };
	if (startY < 0) {
		startY = 0;
		minY = -ypos;
	}
	int endY{ ypos + static_cast<int>(maxY) };
	if (endY > dstH) {
		endY = dstH;

	}

	int startX{ xpos + static_cast<int>(minX) };
	if (startX < 0) {
		startX = 0;
		minX = -xpos;
	}

	int endX{ xpos + static_cast<int>(maxX) };
	if (endX > dstW) {
		endX = dstW;
	}



	//rotate the basis so we get the edge of the bounding box in the sprite frame.
	float startingu = dudX * (minX)+dudY * (minY)+fSrcCX;
	float startingv = dudX * (minY)+dvdX * (minX)+fSrcCY;

	float rowu = startingu;
	float rowv = startingv;

	uint32_t* pDst = pDstBase + (static_cast<size_t>(dstDelta) * startY) + startX;
	int nextRow = s_nDisplayBufferWidth - (endX - startX);

	uint32_t* pSrc{ pSrcBase };
	for (int y{ startY }; y < endY; y++)
	{
		float u = rowu;
		float v = rowv;


		for (int x{ startX }; x < endX; x++)
		{
			if (u > 0 && v > 0 && u < srcW && v < srcH)
			{
				pSrc = pSrcBase + static_cast<size_t>(u) + (static_cast<size_t>(v) * srcDelta);

				uint32_t src = *pSrc;
				int srcAlpha = src >> 24;

				int destRed = srcAlpha * ((src >> 16) & 0xFF);
				int destGreen = srcAlpha * ((src >> 8) & 0xFF);
				int destBlue = srcAlpha * (src & 0xFF);

				uint32_t dest = *pDst;
				int invSrcAlpha{ 0xFF - srcAlpha };

				destRed += invSrcAlpha * ((dest >> 16) & 0xFF);
				destGreen += invSrcAlpha * ((dest >> 8) & 0xFF);
				destBlue += invSrcAlpha * (dest & 0xFF);

				destRed >>= 8;
				destGreen >>= 8;
				destBlue >>= 8;

				*pDst++ = 0xFF000000 | (destRed << 16) | (destGreen << 8) | destBlue;

			}
			else
			{
				pDst++;
			}

			//change the position in the sprite frame for changing X in the display
			u += dudX;
			v += dvdX;
		}

		//work out the change in the sprite frame for changing Y in the display
		rowu += dudY;
		rowv += dvdY;
		//next row
		pDst += nextRow;
	}
	//END OF NEW CODE
}


//Old verison
//********************************************************************************************************************************
// Function:	RotateScaleAlpha - draws a rotated, scaled sprite with alpha
// Parameters:	s = the sprite to draw
//				xpos, ypos = the position you want to draw the sprite
//				frameIndex = which frame of the animation to draw (wrapped)
//				angle = angle in radians
//				scale = scale (1.0 = normal)
//				rotOffX, rotOffY = offset from centre of sprite to centre of rotation (in pixels)
// Notes:		Stupidly basic approach which iterates through the entire display buffer
//				So unoptimal - I'm not even going to explain it (it'll get replaced soon)
//********************************************************************************************************************************
void PlayBlitter::RotateScaleAlpha(const Sprite& s, int xpos, int ypos, int frameIndex, float angle, float scale)
{
	if (s_pDisplayBuffer == nullptr)
		return;

	frameIndex = frameIndex % s.nFrameCount;

	uint32_t* pDstBase = s_pDisplayBuffer;
	int dstW = s_nDisplayBufferWidth;
	int dstH = s_nDisplayBufferHeight;
	int dstDelta = s_nDisplayBufferWidth;

	uint32_t* pSrcBase = s.pCanvasBuffer + (static_cast<size_t>(s.nWidth) * frameIndex);
	int srcW = s.nWidth;
	int srcH = s.nHeight;
	int srcDelta = s.nCanvasWidth;

	float fDstCX = static_cast<float>(xpos);
	float fDstCY = static_cast<float>(ypos);
	float fSrcCX = static_cast<float>(s.nWidth) / 2.0f + s.originX;
	float fSrcCY = static_cast<float>(s.nHeight) / 2.0f + s.originY;

	float duCol = static_cast<float>(sin(-angle)) * (1.0f / scale);
	float dvCol = static_cast<float>(cos(-angle)) * (1.0f / scale);
	float duRow = dvCol;
	float dvRow = -duCol;

	float startingu = fSrcCX - (fDstCX * dvCol + fDstCY * duCol);
	float startingv = fSrcCY - (fDstCX * dvRow + fDstCY * duRow);

	float rowu = startingu;
	float rowv = startingv;

	for (int y = 0; y < dstH; y++)
	{
		float u = rowu;
		float v = rowv;

		uint32_t* pDst = pDstBase + (static_cast<size_t>(dstDelta) * y);

		for (int x = 0; x < dstW; x++)
		{
			if (u > 0 && v > 0 && u < srcW && v < srcH)
			{
				uint32_t* pSrc = pSrcBase + static_cast<size_t>(u) + (static_cast<size_t>(v) * srcDelta);

				uint32_t src = *pSrc;
				int srcAlpha = src >> 24;

				int destRed = srcAlpha * ((src >> 16) & 0xFF);
				int destGreen = srcAlpha * ((src >> 8) & 0xFF);
				int destBlue = srcAlpha * (src & 0xFF);

				uint32_t dest = *pDst;
				int invSrcAlpha = 0xFF - srcAlpha;

				destRed += invSrcAlpha * ((dest >> 16) & 0xFF);
				destGreen += invSrcAlpha * ((dest >> 8) & 0xFF);
				destBlue += invSrcAlpha * (dest & 0xFF);

				destRed >>= 8;
				destGreen >>= 8;
				destBlue >>= 8;

				*pDst++ = 0xFF000000 | (destRed << 16) | (destGreen << 8) | destBlue;

			}
			else
			{
				pDst++;
			}

			u += duRow;
			v += dvRow;
		}

		rowu += duCol;
		rowv += dvCol;
	}
}


//********************************************************************************************************************************
// Function:	BlitPremultShift - draws a sprite with alpha
// Parameters:	s = the sprite to draw
//				xpos, ypos = the position you want to draw the sprite
//				frameIndex = which frame of the animation to draw (wrapped)
// Notes:		More optimised approach which pre-multiplies the alpha on the source and multiplies the destination RGB channels 
//				at the same time (slight loss of accuracy on alpha multiply, but rarely noticable)
//********************************************************************************************************************************
void PlayBlitter::BlitPremultShift(const Sprite& s, int xpos, int ypos, int frameIndex)
{
	// No display buffer to draw to
	if (s_pDisplayBuffer == nullptr)
		return;

	// Nothing within the display buffer to draw
	if (xpos > s_nDisplayBufferWidth || xpos + s.nWidth < 0 || ypos > s_nDisplayBufferHeight || ypos + s.nHeight < 0)
		return;

	// Wrap the frameIndex just in case
	frameIndex = frameIndex % s.nFrameCount;

	// Work out if we need to clip to the display buffer (and by how much)
	int xClipStart = -xpos;
	if (xClipStart < 0) { xClipStart = 0; }

	int xClipEnd = (xpos + s.nWidth) - s_nDisplayBufferWidth;
	if (xClipEnd < 0) { xClipEnd = 0; }

	int yClipStart = -ypos;
	if (yClipStart < 0) { yClipStart = 0; }

	int yClipEnd = (ypos + s.nHeight) - s_nDisplayBufferHeight;
	if (yClipEnd < 0) { yClipEnd = 0; }

	// Set up the source and destination pointers based on clipping
	int destOffset = (s_nDisplayBufferWidth * (ypos + yClipStart)) + (xpos + xClipStart);
	uint32_t* destPixels = s_pDisplayBuffer + destOffset;

	int sourceOffset = (frameIndex * s.nWidth) + (s.nCanvasWidth * yClipStart) + xClipStart;
	uint32_t* sourcePixels = s.pPreMultAlpha + sourceOffset;

	// Work out in advance how much we need to add to src and dest to reach the next row 
	int destInc = s_nDisplayBufferWidth - s.nWidth + xClipEnd + xClipStart;
	int srcInc = s.nCanvasWidth - s.nWidth + xClipEnd + xClipStart;

	// Iterate through the bits in the current sprite frame
	for (int bh = yClipStart; bh < s.nHeight - yClipEnd; bh++)
	{
		for (int bw = xClipStart; bw < s.nWidth - xClipEnd; bw++)
		{
			uint32_t src = *sourcePixels++;
			uint32_t dest = *destPixels;

			// If the source pixel is complete transparent then we can skip it 
			if (src < 0xFF000000)
			{
				// Divide all channels together by 16 and mask out the low bits (accuracy loss)
				// Then multiply by the inverse alpha (inversed in PreMultiplyAlpha) divided by 16
				// This provides our dest*(1-SrcAlpha) to the nearest 16
				// A SIMD approach could be better/faster, but it's an interesting binary problem
				dest = (((dest >> 4) & 0x000F0F0F) * (src >> 28));
				// Add the (pre-multiplied Alpha) source to the destination and force alpha to opaque
				*destPixels++ = (src + dest) | 0xFF000000;
			}
			else
			{
				destPixels++;
			}
		}
		// Increase buffers by pre-calculated amounts
		destPixels += destInc;
		sourcePixels += srcInc;
	}
}