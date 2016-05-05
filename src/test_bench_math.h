#if !defined(JB_MATH_H)
// ---------------------------------------------------------------------------- //
// <author>Joshua Brauns</author>												//
// <email>joshua_brauns@live.de</email>											//
// <date>04:05:2016</date>														//
// <summary></summary>															//
// ---------------------------------------------------------------------------- //

#include <math.h>

union v2
{
    struct
    {
        real32 X, Y;
    };
    real32 E[2];
};

union v3
{
    struct
    {
        real32 X, Y, Z;
    };
    struct
    {
        real32 R, G, B;
    };
    struct
    {
        v2 XY;
        real32 Ignored0_;
    };
    struct
    {
        real32 Ignored1_;
        v2 YZ;
    };
    real32 E[3];
};

union v4
{
    struct
    {
        real32 X, Y, Z, W;
    };
    struct
    {
        real32 R, G, B, A;
    };
    real32 E[4];
};

inline v2 ToV2(real32 x, real32 y)
{
    v2 result;
    
    result.X = x;
    result.Y = y;
    
    return(result);
}

inline v3 ToV3(real32 x, real32 y, real32 z)
{
    v3 result;
    
    result.X = x;
    result.Y = y;
    result.Z = z;
    
    return(result);
}

inline v3 ToV3(v2 xy, real32 z)
{
    v3 result;
    
    result.X = xy.X;
    result.Y = xy.Y;
    result.Z = z;
    
    return(result);
}

inline v4 ToV4(real32 x, real32 y, real32 z, real32 w)
{
    v4 result;
    
    result.X = x;
    result.Y = y;
    result.Z = z;
    result.W = w;
    
    return(result);
}

struct rectangle2
{
    v2 Min;
    v2 Max;
};

struct rectangle3
{
    v3 Min;
    v3 Max;
};

//
// NOTE (joshua): Scalar operations
//

inline int32 SignOf(int32 value)
{
    int32 result = (value >= 0) ? 1 : -1;
    
    return(result);
}

inline real32 Square(real32 a)
{
    real32 result = a*a;
    
    return(result);
}

inline real32 SquareRoot(real32 value)
{
    real32 result = sqrtf(value);
    
    return(result);
}

inline real32 AbsoluteValue(real32 value)
{
    real32 result = fabs(value);
    return(result);
}

inline uint32 RotateLeft(uint32 value, int32 ammount)
{
#if COMPILER_MSVC
    uint32 result = _rotl(value, ammount);
#else
#endif
    
    return(result);
}

inline uint32 RotateRight(uint32 value, int32 ammount)
{
#if COMPILER_MSVC
    uint32 result = _rotr(value, ammount);
#else
#endif

    return(result);
}

inline int32 RoundReal32ToInt32(real32 value)
{
    int32 result = (int32)roundf(value);
    return(result);
}

inline uint32 RoundReal32ToUInt32(real32 value)
{
    uint32 result = (uint32)roundf(value);
    return(result);
}

inline int32 CeilReal32ToInt32(real32 value)
{
    int32 result = (int32)ceilf(value);
    return(result);
}

inline int32 TruncateReal32ToInt32(real32 value)
{
    int32 result = (int32)value;
    return(result);
}

inline real32 Sin(real32 angle)
{
    real32 result = sinf(angle);
    return(result);
}

inline real32 Cos(real32 angle)
{
    real32 result = cosf(angle);
    return(result);
}

inline real32 ATan2(real32 y, real32 x)
{
    real32 result = atan2f(y, x);
    return(result);
}

//
// NOTE (joshua): v2 operations
//

inline v2 operator*(real32 a, v2 b)
{
    v2 result;
    
    result.X = a*b.X;
    result.Y = a*b.Y;
    
    return(result);
}

inline v2 &operator*=(v2 &a, real32 b)
{
    a = b * a;
    
    return(a);
}

inline v2 operator-(v2 a)
{
    v2 result;
    
    result.X = -a.X;
    result.Y = -a.Y;
    
    return(result);
}

inline v2 operator+(v2 a, v2 b)
{
    v2 result;
    
    result.X = a.X + b.X;
    result.Y = a.Y + b.Y;
    
    return(result);
}

inline v2 &operator+=(v2 &a, v2 b)
{
    a = a + b;
    
    return(a);
}

inline v2 operator-(v2 a, v2 b)
{
    v2 result;
    
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    
    return(result);
}

inline v2 &operator-=(v2 &a, v2 b)
{
    a = a - b;
    
    return(a);
}

inline v2 Hadamard(v2 a, v2 b)
{
    v2 result = { a.X*b.X, a.Y*b.Y };
    
    return(result);
}

inline real32 Inner(v2 a, v2 b)
{
    real32 result = a.X*b.X + a.Y*b.Y;
    
    return(result);
}

inline real32 LengthSq(v2 a)
{
    real32 result = Inner(a, a);
    
    return(result);
}

inline real32 Length(v2 a)
{
    real32 result = SquareRoot(LengthSq(a));
    
    return(result);
}

//
// NOTE (joshua): v3 operations
//

inline v3 operator*(real32 a, v3 b)
{
    v3 result;
    
    result.X = a*b.X;
    result.Y = a*b.Y;
    result.Z = a*b.Z;
    
    return(result);
}

inline v3 &operator*=(v3 &a, real32 b)
{
    a = b * a;
    
    return(a);
}

inline v3 operator-(v3 a)
{
    v3 result;
    
    result.X = -a.X;
    result.Y = -a.Y;
    result.Z = -a.Z;
    
    return(result);
}

inline v3 operator+(v3 a, v3 b)
{
    v3 result;
    
    result.X = a.X + b.X;
    result.Y = a.Y + b.Y;
    result.Z = a.Z + b.Z;
    
    return(result);
}

inline v3 &operator+=(v3 &a, v3 b)
{
    a = a + b;
    
    return(a);
}

inline v3 operator-(v3 a, v3 b)
{
    v3 result;
    
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    result.Z = a.Z - b.Z;
    
    return(result);
}

inline v3 &operator-=(v3 &a, v3 b)
{
    a = a - b;
    
    return(a);
}

inline v3 Hadamard(v3 a, v3 b)
{
    v3 result = { a.X*b.X, a.Y*b.Y, a.Z*b.Z };
    
    return(result);
}

inline real32 Inner(v3 a, v3 b)
{
    real32 result = a.X*b.X + a.Y*b.Y + a.Z*b.Z;
    
    return(result);
}

inline real32 LengthSq(v3 a)
{
    real32 result = Inner(a, a);
    
    return(result);
}

inline real32 Length(v3 a)
{
    real32 result = SquareRoot(LengthSq(a));
    
    return(result);
}

//
// NOTE (joshua): Rectangle2
//

inline v2 GetMinCorner(rectangle2 rect)
{
    v2 result = rect.Min;
    
    return(result);
}

inline v2 GetMaxCorner(rectangle2 rect)
{
    v2 result = rect.Max;
    
    return(result);
}

inline v2 GetCenter(rectangle2 rect)
{
    v2 result = 0.5f*(rect.Min + rect.Max);
    
    return(result);
}

inline rectangle2 RectMinMax(v2 min, v2 max)
{
    rectangle2 result;
    
    result.Min = min;
    result.Max = max;
    
    return(result);
}

inline rectangle2 RectMinDim(v2 min, v2 dim)
{
    rectangle2 result;
    
    result.Min = min;
    result.Max = min + dim;
    
    return(result);
}

inline rectangle2 RectCenterHalfDim(v2 center, v2 halfDim)
{
    rectangle2 result;
    
    result.Min = center - halfDim;
    result.Max = center + halfDim;
    
    return(result);
}

inline rectangle2 AddRadiusTo(rectangle2 a, v2 radius)
{
    rectangle2 result;
    
    result.Min = a.Min - radius;
    result.Max = a.Max + radius;
    
    return(result);
}

inline rectangle2 RectCenterDim(v2 center, v2 dim)
{
    rectangle2 result = RectCenterHalfDim(center, 0.5f*dim);
    
    return(result);
}

inline bool32 IsInRectangle(rectangle2 a, v2 test)
{
    bool32 result = ((test.X >= a.Min.X) &&
                     (test.Y >= a.Min.Y) &&
                     (test.X < a.Max.X) &&
                     (test.Y < a.Max.Y));
                     
    return(result);
}

//
// NOTE (joshua): Rectangle3
//

inline v3 GetMinCorner(rectangle3 rect)
{
    v3 result = rect.Min;
    
    return(result);
}

inline v3 GetMaxCorner(rectangle3 rect)
{
    v3 result = rect.Max;
    
    return(result);
}

inline v3 GetCenter(rectangle3 rect)
{
    v3 result = 0.5f*(rect.Min + rect.Max);
    
    return(result);
}

inline rectangle3 RectMinMax(v3 min, v3 max)
{
    rectangle3 result;
    
    result.Min = min;
    result.Max = max;
    
    return(result);
}

inline rectangle3 RectMinDim(v3 min, v3 dim)
{
    rectangle3 result;
    
    result.Min = min;
    result.Max = min + dim;
    
    return(result);
}

inline rectangle3 RectCenterHalfDim(v3 center, v3 halfDim)
{
    rectangle3 result;
    
    result.Min = center - halfDim;
    result.Max = center + halfDim;
    
    return(result);    
}

inline rectangle3 AddRadiusTo(rectangle3 a, v3 radius)
{
    rectangle3 result;
    
    result.Min = a.Min - radius;
    result.Max = a.Max + radius;
    
    return(result);
}

inline rectangle3 RectCenterDim(v3 center, v3 dim)
{
    rectangle3 result = RectCenterHalfDim(center, 0.5f*dim);
    
    return(result);
}

inline bool32 IsInRectangle(rectangle3 a, v3 test)
{
    bool32 result = ((test.X >= a.Min.X) &&
                     (test.Y >= a.Min.Y) &&
                     (test.Z >= a.Min.Z) &&
                     (test.X < a.Max.X) &&
                     (test.Y < a.Max.Y) &&
                     (test.Z < a.Max.Z));
                     
    return(result);
}

inline bool32 RectanglesIntersect(rectangle3 a, rectangle3 b)
{
    bool32 result = !((b.Max.X < a.Min.X) || (b.Min.X > a.Max.X) ||
                      (b.Max.Y < a.Min.Y) || (b.Min.Y > a.Max.Y) ||
                      (b.Max.Z < a.Min.Z) || (b.Min.Z > a.Max.Z));
                      
    return(result);
}

#define JB_MATH_H
#endif