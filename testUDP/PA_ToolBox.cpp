#include "PA_ToolBox.hpp"

namespace PA_ToolBox
{
	float ConvertBytesToFloat(const unsigned char *bytes, unsigned int startIndex)
	{
		float *fPtr;
		fPtr = (float *)(bytes + startIndex);
		return *fPtr;
	}

	void ConvertFloatToBytes(float f, unsigned char* bytes, unsigned int startIndex)
	{
		float *fPtr = &f;
		unsigned char *ucPtr = (unsigned char *)fPtr;
		for (int i = 0; i < 4; i++)
			bytes[startIndex + i] = ucPtr[i];

	}
}
