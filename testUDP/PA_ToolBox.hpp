#pragma once

namespace PA_ToolBox
{
	float ConvertBytesToFloat(const char *bytes, unsigned int startIndex);
	void ConvertFloatToBytes(float f, char *bytes, unsigned int startIndex);
}