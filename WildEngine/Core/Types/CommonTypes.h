#pragma once

struct FVertexType
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

enum class EPrimitiveType
{
    Cube,
    Sphere,
    Triangle
};

enum class EGizmoType
{
    Translation,
    Rotation,
    Scale
};