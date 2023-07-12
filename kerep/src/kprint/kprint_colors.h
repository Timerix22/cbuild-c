#pragma once

#if __cplusplus
extern "C" {
#endif

//         10000000 00000000  00000000 00000000
//         ^   ^^^^
//         |   color num
// fgColorSet flag   
PACKED_ENUM(kp_fgColor,
    /// black foreground
    kp_fgBlack   = 0x80000000,
    /// dark red foreground
    kp_fgRedD    = 0x81000000,
    /// dark green foreground
    kp_fgGreenD  = 0x82000000,
    /// dark yellow foreground
    kp_fgYellowD = 0x83000000,
    /// dark blue foreground
    kp_fgBlueD   = 0x84000000,
    /// dark magenta foreground
    kp_fgMagentaD= 0x85000000,
    /// dark cyan foreground
    kp_fgCyanD   = 0x86000000,
    /// gray foreground
    kp_fgGray    = 0x87000000,
    /// dark gray foreground
    kp_fgGrayD   = 0x88000000,
    /// red foreground
    kp_fgRed     = 0x89000000,
    /// green foreground
    kp_fgGreen   = 0x8a000000,
    /// yellow  foreground
    kp_fgYellow  = 0x8b000000,
    /// blue foreground
    kp_fgBlue    = 0x8c000000,
    /// magenta foreground
    kp_fgMagenta = 0x8d000000,
    /// cyan foreground
    kp_fgCyan    = 0x8e000000,
    /// white foreground
    kp_fgWhite      = 0x8f000000 
)

//          01000000 00000000  00000000 00000000
//           ^       ^^^^
// bgColorSet flag   color num
PACKED_ENUM(kp_bgColor,
    /// black background
    kp_bgBlack   = 0x40000000,
    /// dark red background
    kp_bgRedD    = 0x40100000,
    /// dark green background
    kp_bgGreenD  = 0x40200000,
    /// dark yellow background
    kp_bgYellowD = 0x40300000,
    /// dark blue background
    kp_bgBlueD   = 0x40400000,
    /// dark magenta background
    kp_bgMagentaD= 0x40500000,
    /// dark cyan background
    kp_bgCyanD   = 0x40600000,
    /// gray background
    kp_bgGray    = 0x40700000,
    /// dark gray background
    kp_bgGrayD   = 0x40800000,
    /// red background
    kp_bgRed     = 0x40900000,
    /// green background
    kp_bgGreen   = 0x40a00000,
    /// yellow background
    kp_bgYellow  = 0x40b00000,
    /// blue background
    kp_bgBlue    = 0x40c00000,
    /// magenta background
    kp_bgMagenta = 0x40d00000,
    /// cyan background
    kp_bgCyan    = 0x40e00000,
    /// white background
    kp_bgWhite   = 0x40f00000
)

char* kp_bgColor_toString(kp_bgColor c);
char* kp_fgColor_toString(kp_fgColor c);

#if __cplusplus
}
#endif