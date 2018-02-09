#pragma once

#define _PARAM_ p
#define _COMMA_ ,

#define REPEAT_P(n) _REPEAT_P_##n

#define _REPEAT_P_1 p0
#define _REPEAT_P_2 _REPEAT_P_1, p1
#define _REPEAT_P_3 _REPEAT_P_2, p2
#define _REPEAT_P_4 _REPEAT_P_3, p3
#define _REPEAT_P_5 _REPEAT_P_4, p4
#define _REPEAT_P_6 _REPEAT_P_5, p5
#define _REPEAT_P_7 _REPEAT_P_6, p6