
#ifndef NEWAGE_BASE_MATH_TRIG_H
#define NEWAGE_BASE_MATH_TRIG_H


#define Math_PI 3.141592f


extern float mathSin(__reg("fp0") float f);
extern float mathCos(__reg("fp0") float f);
extern float mathTan(__reg("fp0") float f);
extern float mathASin(__reg("fp0") float f);
extern float mathACos(__reg("fp0") float f);
extern float mathATan(__reg("fp0") float f);
extern float mathATan2(__reg("fp0") float x, __reg("fp1") float y);


#endif
