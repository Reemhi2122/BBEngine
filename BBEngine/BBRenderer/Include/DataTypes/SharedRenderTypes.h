#pragma once
#include "Vector3.h"
#include "Vector4.h"
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
		Vector3 normals;
	};

	//TODO(Stan):	Add support for MIME type images
	struct Texture {
		struct {
			char* m_Path;
		} image;

		float scale;
		float strenght;
		uint32_t texCoordIndex;
	};

	struct Mesh {
		const char*	name;
		uint32_t primitiveCount;
		
		struct Primative
		{
			//Note(Stan):	In the attribute data, you can have multiple
			//				texCoords for different textures.
			union
			{
				void* attributeData[4];
				struct
				{
					Vector3*	vertices;
					UV*			texCoords;
					Vector3*	normals;
					Vector4*	tangents;
				};
			};

			union
			{
				uint32_t countData[4];
				struct
				{
					uint32_t	vertexCount;
					uint32_t	texCoordCount;
					uint32_t	normalCount;
					uint32_t	tangentCount;
				};
			};

			struct {
				
				struct {
					Texture baseColorTexture;
					Vector3 baseColorFactor;
					Texture metallicRoughnessTexture;
					uint32_t metallicFactor;
					uint32_t roughnessFactor;
				} pbrMetallicRoughness ;

				Texture normalTexture;
				Texture occlusionTexture;
				Texture emissiveTexture;
				Vector3 emissiveFactor;

			} Material;

			uint32_t		indicesAmount;
			unsigned short* indices;
		}* primative;
	};

	//Removed sampler for simplicty rn
	//struct {
	//	uint32_t magFilter;
	//	uint32_t minFilter;
	//	uint32_t wrapS;
	//	uint32_t warpT;
	//} sampler;



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
		
		uint32_t totalVertexCount;
		uint32_t PrimitiveCount;
	};

}