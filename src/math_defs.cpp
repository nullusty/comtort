#include "math_defs.h"

bool intervalContains(float a, float b, float x) {
	if (((a < x) & (x < b)) | ((b < x) & (x < a))) return true;
	else return false;
}
