#pragma once

namespace PA_ToolBox
{
	float convertBytesToFloat(unsigned char *bytes, unsigned int startIndex);
	void convertFloatToBytes(float f, unsigned char *bytes, unsigned int startIndex);
	float meanf(const float* datas, unsigned int size);
	float standartDeviationf(const float* datas, unsigned int size);
}