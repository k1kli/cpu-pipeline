#pragma once
#include "TriangleInterpolatorManagementInterface.h"
template<class T>
class TriangleInterpolator : public TriangleInterpolatorManagementInterface
{
	T vert1Value;
	T vert2Value;
	T vert3Value;

	float w1 = 0.0f;
	float w2 = 0.0f;
	float w3 = 0.0f;

	T lineLeftValue;
	T lineRightValue;

	T currentValue;

public:
	void initTriangleValues(
		const T& vert1Value,
		const T& vert2Value,
		const T& vert3Value);
	void setBarycentricWeights(float w1, float w2, float w3);
	T getValue();
};

template<class T>
void TriangleInterpolator<T>::initTriangleValues(
	const T& vert1Value,
	const T& vert2Value,
	const T& vert3Value)
{
	this->vert1Value = vert1Value;
	this->vert2Value = vert2Value;
	this->vert3Value = vert3Value;
}



template<class T>
void TriangleInterpolator<T>::setBarycentricWeights(float w1, float w2, float w3)
{
	this->w1 = w1;
	this->w2 = w2;
	this->w3 = w3;
}


template<class T>
T TriangleInterpolator<T>::getValue()
{
	return w1 * vert1Value + w2 * vert2Value + w3 * vert3Value;
}


