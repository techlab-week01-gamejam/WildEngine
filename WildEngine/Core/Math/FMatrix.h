#pragma once
#include <cstring>
#include "FVector.h"

struct FMatrix {
	float M[4][4];
	// 단위 행렬
	static const FMatrix Identity;

    // 기본 생성자 (0으로 초기화)
    FMatrix()
    {
        memset(M, 0, sizeof(M));
    }

    // 초기화 생성자
    FMatrix(const float InitM[4][4])
    {
        memcpy(M, InitM, sizeof(M));
    }

    void SetIdentity() {
        M[0][0] = 1; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
        M[1][0] = 0; M[1][1] = 1;  M[1][2] = 0;  M[1][3] = 0;
        M[2][0] = 0; M[2][1] = 0;  M[2][2] = 1;  M[2][3] = 0;
        M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
    }

    // 전치 행렬 (Transpose)
    FMatrix Transpose() const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                Result.M[i][j] = M[j][i];
        return Result;
    }

    // 행 벡터(Row Vector) 반환
    FVector GetRow(int Row) const
    {
        return FVector(M[Row][0], M[Row][1], M[Row][2]);
    }

    void SetRow(int Row, FVector Value) {
        M[Row][0] = Value.X;
        M[Row][1] = Value.Y;
        M[Row][2] = Value.Z;
    }

    // 열 벡터(Column Vector) 반환
    FVector GetColumn(int Col) const
    {
        return FVector(M[0][Col], M[1][Col], M[2][Col]);
    }

    void SetColumn(int Col, FVector Value) {
        M[0][Col] = Value.X;
        M[1][Col] = Value.Y;
        M[2][Col] = Value.Z;
    }
};
