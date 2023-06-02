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
				float r[4];
				float g[4];
				float b[4];
				float a[4];
			};
		};
	};
}