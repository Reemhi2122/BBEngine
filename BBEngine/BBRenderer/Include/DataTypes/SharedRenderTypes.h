#pragma once
#include "Vector3.h"
#include <stdint.h>

namespace BBE {

	struct Vertex
	{
		struct
		{
			float x, y, z;
		} pos;

		struct
		{
			float u, v;
		} texCoord;
	};

	//Note(Stan):	Doubting if these are even supposed to be
	//				in the SharedRenderTypes because they could
	//				be part of the normal engine as well..
	struct Mesh {
		const char* name;
		
		uint32_t vertAmount;

		//Note(Stan):	
		//Vector3* vertices;
		//Vector3* texCoords;
		//Vector3* normals;
		Vertex* vertices;

		uint32_t indicesAmount;
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