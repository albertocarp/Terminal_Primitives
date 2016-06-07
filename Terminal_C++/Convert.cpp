#include "Convert.h"

char * Convert::decToBin(float input, const short & out_size)
{
	char * ret = (char*)malloc(CHUNK_DATA);
	short left = (short)input;
	float right = (float)((float)input - (short)input);

	if (ret == NULL)
	{
		printf("Error allocating memory");
		exit(1);
	}
	memset(ret, 0x00, out_size);
	bool integer = false;
	int index = 3;
	if (right == 0)
	{
		integer = true;
		ret[0] = 0;
		index = 8;
	}
	else
	{
		ret[0] = 1;
	}
	while (left != 0)
	{
		short t = left % 2;
		ret[index] = t;
		left /= 2;
		index--;
	}
	if (integer)
	{
		return ret;
	}
	short left_result = 0;
	float right_result = 0.0;
	right_result = (right - (short)(right));
	short i = 0;
	index = 4;
	while (i < CHUNK_DATA-3)
	{
		right_result = right_result * 2;
		left_result = (short)right_result;
		right_result = (right_result - (short)(right_result));
		if (left_result == 1)
		{
			ret[index] = 1;
		}
		else
		{
			ret[index] = 0;
		}
		if (right_result == 0.0)
			break;
		index++;
	}
	return ret;

}
