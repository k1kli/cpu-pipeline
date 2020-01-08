#pragma once
#include "TriangleInterpolatorManagementInterface.h"
template<class T>
class TriangleInterpolator : public TriangleInterpolatorManagementInterface
{
	T vertBaseLeftValue;
	T vertBaseRightValue;
	T vertPeakValue;

	T lineLeftValue;
	T lineRightValue;

	T currentValue;

public:
	void initTriangleValues(
		const T& vertBaseLeftValue,
		const T& vertBaseRightValue,
		const T& vertPeakValue);
	void startLine(float q, float oneMinusQLeft, float oneMinusQRight);
	void setLinePosition(float q, float oneMinusQ);
	T interpolateBetweenTwoVertices(const T& vertOneValue, const T& vertTwoValue,
		float vertOneDepth, float vertTwoDepth, float q);
	T getValue();
};

template<class T>
void TriangleInterpolator<T>::initTriangleValues(
	const T& vertBaseLeftValue,
	const T& vertBaseRightValue,
	const T& vertPeakValue)
{
	this->vertBaseLeftValue = vertBaseLeftValue;
	this->vertBaseRightValue = vertBaseRightValue;
	this->vertPeakValue = vertPeakValue;
}


template<class T>
void TriangleInterpolator<T>::startLine(float q, float oneMinusQLeft, float oneMinusQRight)
{
	lineLeftValue = (vertBaseLeftValue * oneMinusQLeft + vertPeakValue * q) / (oneMinusQLeft + q);
	lineRightValue = (vertBaseRightValue * oneMinusQRight + vertPeakValue * q) / (oneMinusQRight + q);
}

template<class T>
void TriangleInterpolator<T>::setLinePosition(float q, float oneMinusQ)
{
	currentValue = (lineLeftValue * oneMinusQ + lineRightValue * q) / (oneMinusQ + q);
}

template<class T>
inline T TriangleInterpolator<T>::interpolateBetweenTwoVertices(const T& vertOneValue, const T& vertTwoValue, float vertOneDepth, float vertTwoDepth, float q)
{
	float weightedQ = q / vertTwoDepth;
	float weightedOneMinusQ = (1 - q) / vertOneDepth;
	return (vertOneValue * weightedOneMinusQ + vertTwoValue * weightedQ) /
		(weightedOneMinusQ + weightedQ);
}

template<class T>
T TriangleInterpolator<T>::getValue()
{
	return currentValue;
}


