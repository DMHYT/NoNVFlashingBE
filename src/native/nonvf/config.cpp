#include "config.h"

float NoNVFConfig::maxBrightness = 1.0;
bool NoNVFConfig::fadeOut = true;
int NoNVFConfig::fadeTicks = 20;
float NoNVFConfig::fadeRate = NoNVFConfig::maxBrightness / NoNVFConfig::fadeTicks;