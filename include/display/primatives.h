#ifndef __PRIMATIVES_H__
#define __PRIMATIVES_H__

#include "texture.h"

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

struct PrimLine;
struct PrimSquare;
struct PrimCircle;
struct PrimTri;
struct PrimEllipse;
struct PrimRoundedSquare;

union DisplayPrimativeDescription {
    PrimLine lineOpt;
    PrimSquare squareOpt;
    PrimCircle circleOpt;
    PrimTri triOpt;
    PrimEllipse elipseOpt;
    PrimSquare roundSquareOpt;
};

struct DisplayPrimative {
   public:
    DisplayPrimativesType type;
    DisplayPrimativeOptions options;
    DisplayPrimativeDescription desc;

    static DisplayPrimative line(DisplayPrimativeOptions options, PrimLine lineDesc) { return DisplayPrimative(PRIM_LINE, options, lineDesc); }
    static DisplayPrimative square(DisplayPrimativeOptions options, PrimSquare squareDesc) { return DisplayPrimative(PRIM_SQUARE, options, squareDesc); }
    static DisplayPrimative circle(DisplayPrimativeOptions options, PrimCircle circleDesc) { return DisplayPrimative(PRIM_CIRCLE, options, circleDesc); }
    static DisplayPrimative tri(DisplayPrimativeOptions options, PrimTri triDesc) { return DisplayPrimative(PRIM_TRI, options, triDesc); }
    static DisplayPrimative ellipse(DisplayPrimativeOptions options, PrimEllipse ellipseDesc) { return DisplayPrimative(PRIM_ELLIPSE, options, ellipseDesc); }
    static DisplayPrimative roundedSquare(DisplayPrimativeOptions options, PrimSquare roundSquareDesc) { return DisplayPrimative(PRIM_ROUNDED_SQUARE, options, roundSquareDesc); }

   private:
    DisplayPrimative(DisplayPrimativesType primType, DisplayPrimativeOptions primOptions, DisplayPrimativeDescription primDesc) : type(primType), options(primOptions), desc(primDesc) {}
}

#endif  // __PRIMATIVES_H__