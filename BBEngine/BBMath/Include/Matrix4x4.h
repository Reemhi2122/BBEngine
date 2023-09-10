#pragma once

#define MATRIX4X4SIZE 16

namespace BBMath {
	class Matrix4x4
	{
	public:
		Matrix4x4() noexcept;
		
		Matrix4x4(
			float, float, float, float, 
			float, float, float, float, 
			float, float, float, float, 
			float, float, float, float) noexcept;

		~Matrix4x4() = default;

		Matrix4x4& operator=(const Matrix4x4& rhs) noexcept;
		bool operator==(const Matrix4x4& rhs) noexcept;

		Matrix4x4 operator*(const Matrix4x4& rhs);


	public:
		union {
			float m[MATRIX4X4SIZE];
			float mt[4][4];

			struct {
				float x[4];
				float y[4];
				float z[4];
				float w[4];
			};
		};
	};
}