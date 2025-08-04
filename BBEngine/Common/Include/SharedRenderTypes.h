#pragma once
#include "Vector3.h"
#include "BBMath.h"
#include <stdint.h>
#include <vector>

#define MAX_NAME 256
#define INVALID_INDEX -1

//Note(Stan):	Doubting if these are even supposed to be
//				in the SharedRenderTypes because they could
//				be part of the normal engine as well..
namespace BBE {

	enum class AlphaMode
	{
		MASK_MODE,
		BLEND_MODE,
		OPAQUE_MODE
	};

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
	struct TextureT 
	{
		const char* path = nullptr;
		uint32_t texCoordIndex;
	};

	struct Mesh 
	{
		char name[MAX_NAME];
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

			struct Material
			{
				struct 
				{
					TextureT	baseColorTexture;
					Vector4		baseColorFactor;
					TextureT	metallicRoughnessTexture;
					float		metallicFactor;
					float		roughnessFactor;
				} pbrMetallicRoughness;

				TextureT	normalTexture;
				float		normalTextureScale;

				TextureT	occlusionTexture;
				float		occlusionTextureStrength;

				TextureT	emissiveTexture;
				Vector3		emissiveFactor;

				AlphaMode alphaMode;
				const char* materialName;

				//Note(Stan): Hard coded for now, probably want a dynamic? array of extentions as these are rarely
				//				here and I don't want GLTF files to be that big, although we only safe one of these
				//				for each model.
				struct Extensions
				{
					struct KhrMaterialTransmission
					{
						float transmissionFactor;
						TextureT transmissionTexture;
					} khrMaterialTransmission;
					bool hasKhrMaterialTransmission = false;

					struct KhrMaterialvolume
					{
						float thicknessFactor;
						TextureT thicknessTexture;
						float attenuationDistance;
						Vector3	attenuationColor;
					} khrMaterialVolume;
					bool hasKhrMaterialVolume = false;;
				} extensions;

			} material;

			uint32_t		indicesAmount;
			uint8_t			indicesDataSize;
			uint8_t*		indices;
		}* primative;
	};

	//Note(Stan): Removed sampler for simplicty rn
	//struct {
	//	uint32_t magFilter;
	//	uint32_t minFilter;
	//	uint32_t wrapS;
	//	uint32_t warpT;
	//} sampler;

	struct GLTFNode
	{
		char name[MAX_NAME];

		//Note(Stan): Change this to BB specific container
		std::vector<GLTFNode*> Children;
		int Parent = INVALID_INDEX;
		int NodeIndex = INVALID_INDEX;

		Mesh mesh;
		Vector3 translation;
		Vector4 rotation;
		Vector3 scale;
	};

	struct GLTFFile {
		GLTFNode* nodes = nullptr;
		GLTFNode* rootNode = nullptr;
		uint32_t nodeAmount;
		uint32_t rootNodeAmount;
		char* gltfPath;
		
		uint32_t totalVertexCount;
		uint32_t PrimitiveCount;
	};
}