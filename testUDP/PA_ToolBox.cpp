#include "PA_ToolBox.hpp"

namespace PA_ToolBox
{
	float ConvertBytesToFloat(unsigned char *bytes, unsigned int startIndex)
	{
		char bytesReversed[4];
		for (int i = 0; i < 4; i++)
			bytesReversed[i] = bytes[startIndex + 3 - i];
		float *fPtr = (float*)(bytesReversed);
		float f = *fPtr;
		return f;
	}

	void ConvertFloatToBytes(float f, unsigned char* bytes, unsigned int startIndex)
	{
		float *fPtr = &f;
		unsigned char *cPtr = (unsigned char *)fPtr;
		for (int i = 0; i < 4; i++)
			bytes[startIndex + i] = cPtr[i];

	}
}
