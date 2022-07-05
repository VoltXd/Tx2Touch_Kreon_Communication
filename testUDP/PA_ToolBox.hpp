#pragma once

namespace PA_ToolBox
{
	float ConvertBytesToFloat(const unsigned char *bytes, unsigned int startIndex);
	void ConvertFloatToBytes(float f, unsigned char *bytes, unsigned int startIndex);
}