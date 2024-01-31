#pragma once
#include "Vector3.h"
#include <stdint.h>

//Note(Stan):	Doubting if these are even supposed to be
//				in the SharedRenderTypes because they could
//				be part of the normal engine as well..
namespace BBE {

	struct UV
	{
		float u, v;
	};

	struct Vertex
	{
		Vector3 pos;
		UV		texCoords;
		//Vector3 normals;
	};

	struct Mesh {
		const char*	name;
		uint32_t primitiveCount;
		struct Primative
		{
			union
			{
				void* data[4];
				struct
				{
					Vector3*	vertices;
					UV*			texCoords;
					Vector3*	normals;
					Vector3*	tangents; //vector4
				};
			} attributes;

			union
			{
				uint32_t data[4];
				struct
				{
					uint32_t	vertexCount;
					uint32_t	texCoordCount;
					uint32_t	normalCount;
					uint32_t	tangentCount;
				};
			} counts;

			char* baseTexturePath;

			uint32_t		indicesAmount;
			unsigned short* indices;
		}* primative;
	};

	struct Node
	{
		Mesh mesh;
		Vector3 translation;
		//quads
		Vector3 Scale;
		//Matrix
	};

	struct GLTFFile {
		Node* nodes;
		uint32_t nodeAmount;
		char* gltfPath;
	};

}