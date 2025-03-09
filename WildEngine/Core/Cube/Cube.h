#pragma once
#include "../../Engine/Renderer/URenderer.h"

FVertexSimple cube_vertices[] =
{
    { -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f },

    { -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f },

    {  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f },

    {  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },

    { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f },

    { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f },

    {  0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f },

    {  0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 0.5f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f, 1.0f },

    { -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f },

    { -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f },

    { -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f, 1.0f },

    { -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f, 1.0f },
    { -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }
};