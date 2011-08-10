
/**
 * libcomposite-hal
 * Signal processing functions
 * (C) 2010-2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Implements signal processing functions.
 */

#include <math.h>

#include "OEVector.h"

OEVector::OEVector()
{
}

OEVector::OEVector(int size)
{
	data.resize(size);
}

OEVector::OEVector(vector<float> data)
{
	this->data = data;
}

float OEVector::getValue(unsigned int i)
{
	return data[i];
}

OEVector OEVector::operator *(const float value)
{
	OEVector w(data);
	
	for (int i = 0; i < data.size(); i++)
		w.data[i] *= value;
	
	return w;
}

OEVector OEVector::operator *(const OEVector& v)
{
	if (v.data.size() != data.size())
		return OEVector();
	
	OEVector w(data);
	
	for (int i = 0; i < data.size(); i++)
		w.data[i] *= v.data[i];
	
	return w;
}

OEVector OEVector::normalize()
{
	float sum = 0.0;
	
	for (int i = 0; i < data.size(); i++)
		sum += data[i];
	
	OEVector w(data);
	float gain = 1.0 / sum;
	
	for (int i = 0; i < data.size(); i++)
		w.data[i] *= gain;
	
	return w;
}

OEVector OEVector::realIDFT()
{
	OEVector w;
	w.data.resize(data.size());
	
	for (int i = 0; i < data.size(); i++)
	{
		float omega = 2.0 * M_PI * i / data.size();
		
		for (int j = 0; j < data.size(); j++)
			w.data[i] += data[j] * cosf(j * omega);
	}
	
	for (int i = 0; i < data.size(); i++)
		w.data[i] /= data.size();
	
	return w;
}

OEVector OEVector::lanczosWindow(unsigned int n, float fc)
{
	OEVector v;
	v.data.resize(n);
	
	int n2 = n / 2;
	
	for (int i = 0; i < n; i++)
	{
		float x = (i - n2) * fc;
		
		if (x == 0.0F)
			v.data[i] = 1.0F;
		else
			v.data[i] = sinf(M_PI * x) / (M_PI * x);
	}
	
	return v;
}

//
// Based on ideas at:
// http://www.dsprelated.com/showarticle/42.php
//
OEVector OEVector::chebyshevWindow(unsigned int n, float sidelobeDb)
{
	int m = n - 1;
	
	OEVector w;
	w.data.resize(m);
	
	float alpha = coshf(acoshf(powf(10, sidelobeDb / 20.0)) / m);
	
	for (int i = 0; i < m; i++)
	{
		float a = fabsf(alpha * cosf(M_PI * i / m));
		if (a > 1)
			w.data[i] = powf(-1, i) * coshf(m * acosh(a));
		else
			w.data[i] = powf(-1, i) * cosf(m * acos(a));
	}
	
	w = w.realIDFT();
	
	w.data.resize(n);
	w.data[0] /= 2.0;
	w.data[n - 1] = w.data[0];
	
	float max = 0.0;
	
	for (int i = 0; i < n; i++)
		if (fabs(w.data[i]) > max)
			max = fabs(w.data[i]);
	
	for (int i = 0; i < n; i++)
		w.data[i] /= max;
	
	return w;
}