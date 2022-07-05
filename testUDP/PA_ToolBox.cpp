#include "PA_ToolBox.hpp"

namespace PA_ToolBox
{
	float ConvertBytesToFloat(const char *bytes, unsigned int startIndex)
	{
		float *fPtr;
		fPtr = (float *)(bytes + startIndex);
		return *fPtr;
	}

	void ConvertFloatToBytes(float f, char* bytes, unsigned int startIndex)
	{
		float *fPtr = &f;
		char *cPtr = (char *)fPtr;
		for (int i = 0; i < 4; i++)
			bytes[startIndex + i] = cPtr[i];

	}
}
