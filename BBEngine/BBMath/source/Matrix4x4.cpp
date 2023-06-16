#include "Matrix4x4.h"

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
		for (size_t i = 0; i < MATRIX4X4SIZE; i++) {
			if (this->m[i] != a_RHS.m[i]) {
				return false;
			}
		}
		return true;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& a_RHS)
	{
		Matrix4x4 res;
		int matrixSize = 4;

		for (size_t i = 0; i < matrixSize; i++)
		{
			for (size_t y = 0; y < matrixSize; y++)
			{
				float value = 0;
				for (size_t x = 0; x < matrixSize; x++)
					value += m[(i * matrixSize) + x] * a_RHS.m[(x * matrixSize) + y];

				res.mt[i][y] = value;
			}
		}

		return res;
	}
}