#if !defined(JB_PLATFORM_H)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;
typedef float real32;
typedef double real64;

#define internal static
#define global_variable static

#if DATA_SNIFFER_DEBUG
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define pi32 3.14159265359f

#define InvalidCodePath Assert(!"InvalidCodePath");

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define JB_PLATFORM_H
#endif