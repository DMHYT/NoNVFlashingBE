const NoNVFConfig = WRAP_NATIVE("NoNVFConfigJS");
NoNVFConfig.setMaxBrightness(Math.min(Math.max(__config__.getNumber("maxBrightness").floatValue(), 0.0), 1.0));
NoNVFConfig.setFadeOutEnabled(__config__.getBool("fadeOut"));
NoNVFConfig.setFadeTicks(__config__.getNumber("fadeTicks").intValue());