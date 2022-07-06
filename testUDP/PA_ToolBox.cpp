#include "PA_ToolBox.hpp"

#include <cmath>

namespace PA_ToolBox
{
	float convertBytesToFloat(unsigned char *bytes, unsigned int startIndex)
	{
		char bytesReversed[4];
		for (int i = 0; i < 4; i++)
			bytesReversed[i] = bytes[startIndex + 3 - i];
		float *fPtr = (float*)(bytesReversed);
		float f = *fPtr;
		return f;
	}

	void convertFloatToBytes(float f, unsigned char* bytes, unsigned int startIndex)
	{
		float *fPtr = &f;
		unsigned char *cPtr = (unsigned char *)fPtr;
		for (int i = 0; i < 4; i++)
			bytes[startIndex + i] = cPtr[i];

	}

	float meanf(const float* datas, unsigned int size)
	{
		float mean = 0;

		for (int i = 0; i < size; i++)
			mean += datas[i];
		mean *= 1.0 / size;
		
		return mean;
	}

	float standartDeviationf(const float* datas, unsigned int size)
	{
		float mean = meanf(datas, size);
		float stDev = 0;
		
		for (int i = 0; i < size; i++)
			stDev += datas[i] * datas[i];
		stDev = sqrt(stDev / size - mean * mean);

		return stDev;
	}
}
