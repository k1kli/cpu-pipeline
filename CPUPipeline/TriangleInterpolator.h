#pragma once
#include "TriangleInterpolatorManagementInterface.h"
template<class T>
class TriangleInterpolator : public TriangleInterpolatorManagementInterface
{
	T vert1Value;
	T vert2Value;
	T vert3Value;

	float * w1 = nullptr;
	float* w2 = nullptr;
	float* w3 = nullptr;


public:
	TriangleInterpolator(int instanceCount)
	{
		w1 = new float[instanceCount];
		w2 = new float[instanceCount];
		w3 = new float[instanceCount];
	}
	~TriangleInterpolator()
	{
		delete[] w1;
		delete[] w2;
		delete[] w3;
	}
	void initTriangleValues(
		const T& vert1Value,
		const T& vert2Value,
		const T& vert3Value);
	void setBarycentricWeights(float w1, float w2, float w3, int instance);
	T getValue(int instance) const;
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
void TriangleInterpolator<T>::setBarycentricWeights(float w1, float w2, float w3, int instance)
{
	this->w1[instance] = w1;
	this->w2[instance] = w2;
	this->w3[instance] = w3;
}


template<class T>
T TriangleInterpolator<T>::getValue(int instance) const
{
	return w1[instance] * vert1Value + w2[instance] * vert2Value + w3[instance] * vert3Value;
}


