#pragma once
#include "TriangleInterpolatorManagementInterface.h"
template<class T>
class TriangleInterpolator : public TriangleInterpolatorManagementInterface
{
	T vert1Value;
	T vert2Value;
	T vert3Value;

	std::vector<float> w1;
	std::vector<float> w2;
	std::vector<float> w3;



public:
	void initTriangleValues(
		const T& vert1Value,
		const T& vert2Value,
		const T& vert3Value);
	void setBarycentricWeights(float w1, float w2, float w3, int instance);
	void setInstanceCount(int instanceCount);
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
void TriangleInterpolator<T>::setInstanceCount(int instanceCount)
{
	this->w1.resize(instanceCount);
	this->w2.resize(instanceCount);
	this->w3.resize(instanceCount);
}


template<class T>
T TriangleInterpolator<T>::getValue(int instance) const
{
	return w1[instance] * vert1Value + w2[instance] * vert2Value + w3[instance] * vert3Value;
}


