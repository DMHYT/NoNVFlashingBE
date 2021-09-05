declare interface NoNVFConfigJS {
    setMaxBrightness(brightness: number): void;
    setFadeOutEnabled(enabled: boolean): void;
    setFadeTicks(ticks: number): void;
}

declare function WRAP_NATIVE(module: "NoNVFConfigJS"): NoNVFConfigJS;