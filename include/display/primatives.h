#ifndef __PRIMATIVES_H__
#define __PRIMATIVES_H__

#include "texture.h"
#include "vec.h"

enum DisplayPrimativesType {
    PRIM_LINE,
    PRIM_SQUARE,
    PRIM_CIRCLE,
    PRIM_TRI,
    PRIM_ELLIPSE,
    PRIM_ROUNDED_SQUARE,
    PRIM_COUNT
};

struct DisplayPrimativeOptions {
    Color color;
    Color outlineColor;
    float outlineWidth;
};

struct PrimLine {
    Vec2 start, end;
};

struct PrimSquare {
    Vec2 topLeft, bottomRight;
    float rotation;
};

struct PrimCircle {
    Vec2 center;
    float radius;
};

struct PrimTri {
    Vec2 a, b, c;
};

struct PrimEllipse {
    Vec2 center;
    float a, b;
    float rotation;
};

struct PrimRoundedSquare {
    Vec2 topLeft, bottomRight;
    float rotation;
    float radius;
};

union DisplayPrimativeDescription {
    PrimLine lineOpt;
    PrimSquare squareOpt;
    PrimCircle circleOpt;
    PrimTri triOpt;
    PrimEllipse elipseOpt;
    PrimRoundedSquare roundSquareOpt;
};

struct DisplayPrimative {
   public:
    DisplayPrimativesType type;
    DisplayPrimativeOptions options;
    DisplayPrimativeDescription desc;

    static DisplayPrimative line(DisplayPrimativeOptions options, PrimLine lineDesc) {
        return DisplayPrimative(PRIM_LINE, options, (DisplayPrimativeDescription) {
            .lineOpt = lineDesc
        });
    }

    static DisplayPrimative square(DisplayPrimativeOptions options, PrimSquare squareDesc) { 
        return DisplayPrimative(PRIM_SQUARE, options, (DisplayPrimativeDescription){
            .squareOpt = squareDesc
        });
    }



   private:
    DisplayPrimative(DisplayPrimativesType primType, DisplayPrimativeOptions primOptions, DisplayPrimativeDescription primDesc) : type(primType), options(primOptions), desc(primDesc) {}
};

#endif  // __PRIMATIVES_H__