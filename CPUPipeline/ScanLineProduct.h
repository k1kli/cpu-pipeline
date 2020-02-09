#pragma once
struct ScanLineProduct
{
	ScanLineProduct(int y, int startX, int endX, float startDepth, float endDepth)
		:y(y), startX(startX), endX(endX), startDepth(startDepth), endDepth(endDepth) {}
	int y;
	int startX;
	int endX;
	float startDepth;
	float endDepth;
};

