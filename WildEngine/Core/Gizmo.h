#pragma once
#include <cmath>
#include "Types/CommonTypes.h"
#include "Types/Types.h"


#define CYLINDER_SEGMENTS 12  // 원기둥을 만들 때 사용할 세그먼트 개수
#define M_PI 3.14159265358979323846

static TArray<FVertexType> gizmo_translation_vertices;
static TArray<uint16_t> gizmo_translation_indices;

// 이동 Gizmo 초기화 함수 (X, Y, Z축 어디든 사용 가능)
static void InitializeGizmoArrow(float length, float radius, float coneHeight, float coneRadius, float r, float g, float b) {
    gizmo_translation_vertices.clear();
    gizmo_translation_indices.clear();

    int baseIndex = 0;

    // 원기둥(Cylinder) 정점 추가
    for (int i = 0; i < CYLINDER_SEGMENTS; i++) {
        float angle = (2.0f * M_PI * i) / CYLINDER_SEGMENTS;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        // 몸통의 시작점
        gizmo_translation_vertices.push_back({ 0.0f, x, y, r, g, b, 1.0f });
        // 몸통의 끝점 (길이만큼 이동)
        gizmo_translation_vertices.push_back({ length, x, y, r, g, b, 1.0f });
    }

    // 원기둥 삼각형 인덱스 추가
    for (int i = 0; i < CYLINDER_SEGMENTS; i++) {
        int next = (i + 1) % CYLINDER_SEGMENTS;
        gizmo_translation_indices.push_back(baseIndex + i * 2);
        gizmo_translation_indices.push_back(baseIndex + i * 2 + 1);
        gizmo_translation_indices.push_back(baseIndex + next * 2 + 1);

        gizmo_translation_indices.push_back(baseIndex + i * 2);
        gizmo_translation_indices.push_back(baseIndex + next * 2 + 1);
        gizmo_translation_indices.push_back(baseIndex + next * 2);
    }

    baseIndex = gizmo_translation_vertices.size();

    // 원뿔(Cone) 정점 추가
    gizmo_translation_vertices.push_back({ length + coneHeight, 0.0f, 0.0f, r, g, b, 1.0f }); // 뿔의 꼭짓점

    for (int i = 0; i < CYLINDER_SEGMENTS; i++) {
        float angle = (2.0f * M_PI * i) / CYLINDER_SEGMENTS;
        float x = cos(angle) * coneRadius;
        float y = sin(angle) * coneRadius;

        gizmo_translation_vertices.push_back({ length, x, y, r, g, b, 1.0f });
    }

    // 원뿔 삼각형 인덱스 추가
    int coneTipIndex = baseIndex;
    for (int i = 0; i < CYLINDER_SEGMENTS; i++) {
        int next = (i + 1) % CYLINDER_SEGMENTS;
        gizmo_translation_indices.push_back(coneTipIndex);
        gizmo_translation_indices.push_back(baseIndex + i + 1);
        gizmo_translation_indices.push_back(baseIndex + next + 1);
    }
}