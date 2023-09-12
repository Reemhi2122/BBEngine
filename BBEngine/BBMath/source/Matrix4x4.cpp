#include "Matrix4x4.h"
#include <immintrin.h>


namespace BBMath {

	Matrix4x4::Matrix4x4() noexcept {
		for (size_t i = 0; i < MATRIX4X4SIZE; i++)
			this->m[i] = 0;
	}

	Matrix4x4::Matrix4x4(
		float a_M0, float a_M1, float a_M2, float a_M3,
		float a_M4, float a_M5, float a_M6, float a_M7,
		float a_M8, float a_M9, float a_M10, float a_M11,
		float a_M12, float a_M13, float a_M14, float a_M15) noexcept
		: m{a_M0, a_M1, a_M2, a_M3,
			a_M4, a_M5, a_M6, a_M7,
			a_M8, a_M9, a_M10, a_M11,
			a_M12, a_M13, a_M14, a_M15}
	{}

	Matrix4x4& Matrix4x4::operator=(const Matrix4x4& a_RHS) noexcept {
		if (this == &a_RHS)
			return *this;

		for (size_t i = 0; i < MATRIX4X4SIZE; i++)
			this->m[i] = a_RHS.m[i];

		return *this;
	}

	bool Matrix4x4::operator==(const Matrix4x4& a_RHS) noexcept {
		const __m128 rhsRX = _mm_load_ps(a_RHS.x);
		const __m128 rhsRY = _mm_load_ps(a_RHS.y);
		const __m128 rhsRZ = _mm_load_ps(a_RHS.z);
		const __m128 rhsRW = _mm_load_ps(a_RHS.w);

		const __m128 lhsRX = _mm_load_ps(this->x);
		const __m128 lhsRY = _mm_load_ps(this->y);
		const __m128 lhsRZ = _mm_load_ps(this->z);
		const __m128 lhsRW = _mm_load_ps(this->w);

		__m128 rx = _mm_cmp_ps(rhsRX, lhsRX, _CMP_EQ_OS);
		__m128 ry = _mm_cmp_ps(rhsRY, lhsRY, _CMP_EQ_OS);
		__m128 rz = _mm_cmp_ps(rhsRZ, lhsRZ, _CMP_EQ_OS);
		__m128 rw = _mm_cmp_ps(rhsRW, lhsRW, _CMP_EQ_OS);
		
		return _mm_movemask_ps(rx) && _mm_movemask_ps(ry) && _mm_movemask_ps(rz) && _mm_movemask_ps(rw);
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& a_RHS)
	{
		Matrix4x4 res;
		int matrixSize = 4;

		const __m128 rhsRX = _mm_load_ps(a_RHS.x);
		const __m128 rhsRY = _mm_load_ps(a_RHS.y);
		const __m128 rhsRZ = _mm_load_ps(a_RHS.z);
		const __m128 rhsRW = _mm_load_ps(a_RHS.w);

		float* leftRowPointer = (float*)&this->x;
		float* resultRowPointer = res.x;

		for (int i = 0; i < matrixSize; i++, leftRowPointer += 4, resultRowPointer += 4) {
			__m128 lhsRX = _mm_set1_ps(leftRowPointer[0]);
			__m128 lhsRY = _mm_set1_ps(leftRowPointer[1]);
			__m128 lhsRZ = _mm_set1_ps(leftRowPointer[2]);
			__m128 lhsRW = _mm_set1_ps(leftRowPointer[3]);

			__m128 X = _mm_mul_ps(lhsRX, rhsRX);
			__m128 Y = _mm_mul_ps(lhsRY, rhsRY);
			__m128 Z = _mm_mul_ps(lhsRZ, rhsRZ);
			__m128 W = _mm_mul_ps(lhsRW, rhsRW);

			__m128 R = _mm_add_ps(X, _mm_add_ps(Y, _mm_add_ps(Z, W)));
			_mm_store_ps(resultRowPointer, R);
		}

		return res;
	}
}