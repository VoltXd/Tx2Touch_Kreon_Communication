#pragma once

namespace PA_ToolBox
{
	float ConvertBytesToFloat(unsigned char *bytes, unsigned int startIndex);
	void ConvertFloatToBytes(float f, unsigned char *bytes, unsigned int startIndex);
}