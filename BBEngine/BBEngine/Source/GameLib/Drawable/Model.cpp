#include "GameLib/Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"

Model::Model(IGraphics& a_Gfx, const char* a_Name, BBE::GLTFFile* a_File, uint32_t a_VertexShader, uint32_t a_PixelShader)
{
	strcpy(m_Name, a_Name);

	m_Nodes = reinterpret_cast<ModelNode*>(malloc(a_File->nodeAmount * sizeof(ModelNode)));
	memset(m_Nodes, 0, a_File->nodeAmount * sizeof(ModelNode));

	m_nodeCount = a_File->nodeAmount;
	for (size_t nodeIndex = 0; nodeIndex < a_File->nodeAmount; nodeIndex++)
	{
		BBE::Node& curNode = a_File->nodes[nodeIndex];

		strcpy(m_Nodes[nodeIndex].name, curNode.name);

		m_Nodes[nodeIndex].parent = &m_Nodes[curNode.Parent];

		m_Nodes[nodeIndex].objectTransform = { 
			curNode.translation, 
			curNode.rotation, 
			curNode.scale 
		};

		m_Nodes[nodeIndex].primitiveCount = curNode.mesh.primitiveCount;
		m_Nodes[nodeIndex].primitives = reinterpret_cast<ModelNode::ModelPrimitive*>(malloc(m_Nodes[nodeIndex].primitiveCount * sizeof(ModelNode::ModelPrimitive)));

		for (size_t primitiveIndex = 0; primitiveIndex < curNode.mesh.primitiveCount; primitiveIndex++)
		{
			BBE::Mesh::Primative& gltfPrimitive = curNode.mesh.primative[primitiveIndex];
			BBE::Vertex* vertices = reinterpret_cast<BBE::Vertex*>(malloc(gltfPrimitive.vertexCount * sizeof(BBE::Vertex)));

			ModelNode::ModelPrimitive& modelPrimitive = m_Nodes[nodeIndex].primitives[primitiveIndex];
			modelPrimitive = {};

			if (vertices == nullptr)
				return;

			for (size_t i = 0; i < gltfPrimitive.vertexCount; i++)
			{
				vertices[i].pos = gltfPrimitive.vertices[i];
				vertices[i].texCoords = gltfPrimitive.texCoords[i];
				vertices[i].normals = gltfPrimitive.normals[i];
			}

			if (gltfPrimitive.material.pbrMetallicRoughness.baseColorTexture.path)
			{
				char texturePath[_MAX_PATH] = "";
				strcat(texturePath, a_File->gltfPath);
				strcat(texturePath, gltfPrimitive.material.pbrMetallicRoughness.baseColorTexture.path);

				modelPrimitive.m_Texture = new DX12Texture();
				modelPrimitive.m_Texture->Create(a_Gfx, texturePath);

				//modelPrimitive.m_Sampler = new Sampler(a_Gfx);
			}

			if (gltfPrimitive.material.pbrMetallicRoughness.baseColorTexture.path != nullptr)
			{
				Vector4 FactorData = gltfPrimitive.material.pbrMetallicRoughness.baseColorFactor;
				modelPrimitive.m_MaterialConstant.baseColor = Vector4(1, 1, 1, 1);
				modelPrimitive.m_MaterialConstant.hasBaseColorTexture = 1;
			}
			else
			{
				Vector4 FactorData = gltfPrimitive.material.pbrMetallicRoughness.baseColorFactor;
				modelPrimitive.m_MaterialConstant.baseColor = FactorData;
				modelPrimitive.m_MaterialConstant.hasBaseColorTexture = 0;
			}

			//if (gltfPrimitive.material.extensions.hasKhrMaterialVolume)
			//{
			//	modelPrimitive.m_MaterialConstant.hasKhrVolume = 1;
			//	modelPrimitive.m_MaterialConstant.khrThicknessFactor = gltfPrimitive.material.extensions.khrMaterialVolume.thicknessFactor;
			//	modelPrimitive.m_MaterialConstant.khrAttenuationDistance = gltfPrimitive.material.extensions.khrMaterialVolume.attenuationDistance;
			//	modelPrimitive.m_MaterialConstant.khrAttenuationColor = gltfPrimitive.material.extensions.khrMaterialVolume.attenuationColor;

			//	if (gltfPrimitive.material.extensions.khrMaterialVolume.thicknessTexture.path)
			//	{
			//		char texturePath[_MAX_PATH] = "";
			//		strcat(texturePath, a_File->gltfPath);
			//		strcat(texturePath, gltfPrimitive.material.extensions.khrMaterialVolume.thicknessTexture.path);

			//		modelPrimitive.m_MaterialConstant.hasKhrVolumeTexture = 1;
			//		modelPrimitive.m_KhrVolumeTexture = new DX12Texture();
			//		modelPrimitive.m_KhrVolumeTexture->Create(a_Gfx, texturePath, 5);
			//	}
			//}

			modelPrimitive.vBuffer = new DX12VertexBuffer();
			modelPrimitive.vBuffer->Create(a_Gfx, vertices, sizeof(BBE::Vertex), gltfPrimitive.vertexCount);

			modelPrimitive.m_IndexBuffer = new DX12IndexBuffer();
			modelPrimitive.m_IndexBuffer->Create(a_Gfx, gltfPrimitive.indices, gltfPrimitive.indicesAmount, gltfPrimitive.indicesDataSize);
			modelPrimitive.m_Blend = gltfPrimitive.material.alphaMode;
		}
	}

	//m_ModelPixelBuffer = new RootConstantBuffer<MaterialConstant>();
	//m_ModelPixelBuffer->Create(a_Gfx, 2, 1);

	m_VertexShader = m_CurVertexShader = a_VertexShader;
	m_PixelShader = m_CurPixelShader = a_PixelShader;

	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord",		0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "InstanceTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,							D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	//m_InputLayout = new InputLayout(a_Gfx, ied, a_Gfx.GetVertexShaderByteCode(m_CurVertexShader).Get());
	//AddBind(m_InputLayout);

	//m_Topology = new Topology(a_Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//AddBind(m_Topology);
}

void Model::SetCurrentShader(uint32_t a_VertexShader, uint32_t a_PixelShader)
{
	m_CurVertexShader = a_VertexShader;
	m_CurPixelShader = a_PixelShader;
}

void Model::ResetShaders()
{
	m_CurVertexShader = m_VertexShader;
	m_CurPixelShader = m_PixelShader;
}

void Model::AddToDraw(DirectX::XMMATRIX a_Transform)
{
	m_InstanceBuffer.push_back({ a_Transform });
}

//Note(Stan): Look into unbinding of resources. Currently they are bound until something else is bound.
//				Only fixed for textures right now.
void Model::Draw(IGraphics& a_Gfx, uint32_t a_NodeIndex) noexcept
{
	//m_ModelPixelBuffer->Bind(a_Gfx);
	//for (size_t curNode = 0; curNode < m_nodeCount; curNode++)
	//{
		m_CurTransform->Bind(a_Gfx);
		for (size_t i = 0; i < m_Nodes[a_NodeIndex].primitiveCount; i++)
		{
			ModelNode::ModelPrimitive& curPrimitive = m_Nodes[a_NodeIndex].primitives[i];

			if (curPrimitive.m_Texture != nullptr)
			{
				curPrimitive.m_Texture->Bind(a_Gfx);
				//curPrimitive.m_Sampler->Bind(a_Gfx);
			}

			//if (curPrimitive.m_KhrVolumeTexture != nullptr)
			//{
			//	curPrimitive.m_KhrVolumeTexture->Bind(a_Gfx);
			//}

			//a_Gfx.SetBlendState(curPrimitive.m_Blend);

			//m_ModelPixelBuffer->Update(a_Gfx, curPrimitive.m_MaterialConstant);

			curPrimitive.vBuffer->Bind(a_Gfx);
			curPrimitive.m_IndexBuffer->Bind(a_Gfx);

			SetIndexBuffer(curPrimitive.m_IndexBuffer);
			Drawable::Draw(a_Gfx);


			if (curPrimitive.m_Texture != nullptr)
			{
				curPrimitive.m_Texture->UnBind(a_Gfx);
			}

			//a_Gfx.SetBlendState(BBE::AlphaMode::OPAQUE_MODE);
		}
	//}

	//m_ModelPixelBuffer->UnBind(a_Gfx);
}

void Model::DrawInstanced(IGraphics& a_Gfx, uint32_t a_InstanceCount) noexcept
{
	//for (size_t curNode = 0; curNode < m_nodeCount; curNode++)
	//{
	//	m_Nodes[curNode].transformBuf->Bind(a_Gfx);
	//	for (size_t i = 0; i < m_Nodes[curNode].primitiveCount; i++)
	//	{
	//		if (m_Nodes[curNode].primitives[i].m_Texture != nullptr)
	//		{
	//			m_Nodes[curNode].primitives[i].m_Texture->Bind(a_Gfx);
	//			m_Nodes[curNode].primitives[i].m_Sampler->Bind(a_Gfx);
	//		}

	//		a_Gfx.BindShader(ShaderType::VertexShader, m_CurVertexShader);
	//		a_Gfx.BindShader(ShaderType::PixelShader, m_CurPixelShader);

	//		m_Nodes[curNode].primitives[i].vBuffer->CreateInstanceBuffer(a_Gfx, m_InstanceBuffer.data(), sizeof(DirectX::XMMATRIX), m_InstanceBuffer.size());
	//		m_Nodes[curNode].primitives[i].vBuffer->Bind(a_Gfx);
	//		m_Nodes[curNode].primitives[i].m_IndexBuffer->Bind(a_Gfx);

	//		SetIndexBuffer(m_Nodes[curNode].primitives[i].m_IndexBuffer);
	//		Drawable::DrawInstanced(a_Gfx, m_InstanceBuffer.size());
	//	}
	//}

	//m_InstanceBuffer.clear();
}

void Model::Update(float a_DeltaTime) noexcept {}