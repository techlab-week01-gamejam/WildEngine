#pragma once

struct FVertexType
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

enum class PrimitiveType
{
    Cube,
    Sphere,
    Triangle
};

enum class EGizmoType 
{
    Translation,
    Roation,
    Scale
};