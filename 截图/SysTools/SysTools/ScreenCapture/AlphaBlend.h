#ifndef __ALPHA_BLEND_H__
#define __ALPHA_BLEND_H__

typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);

LPALPHABLEND GetAlphaBlendFunc();

#endif 

