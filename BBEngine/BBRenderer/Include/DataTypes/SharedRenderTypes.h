#pragma once

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
		Vertex* vertices;
		uint32_t vertAmount;
		unsigned short* indices;
		uint32_t indicesAmount;
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