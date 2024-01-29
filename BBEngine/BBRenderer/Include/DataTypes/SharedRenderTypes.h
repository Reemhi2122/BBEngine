#pragma once
#include "Vector3.h"
#include <stdint.h>

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

	//Note(Stan):	Doubting if these are even supposed to be
	//				in the SharedRenderTypes because they could
	//				be part of the normal engine as well..
	struct Mesh {
		const char*		name;
		uint32_t		vertAmount;
		
		union
		{
			void* data[4];
			struct
			{
				Vector3* vertices;
				UV* texCoords;
				Vector3* normals;
				Vector3* tangents; //vector4
			};
		} attributes;

		//Vector3*		vertices;
		//UV*				texCoords;
		//Vector3*		normals;
		
		uint32_t		indicesAmount;
		unsigned short* indices;
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
	};

}