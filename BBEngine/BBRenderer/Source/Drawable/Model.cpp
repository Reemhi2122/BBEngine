#include "Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"

Model::Model(Graphics& a_Gfx, BBE::GLTFFile* a_File, uint32_t a_VertexShader, uint32_t a_PixelShader)
{
	m_Nodes = reinterpret_cast<ModelNodes*>(malloc(a_File->nodeAmount * sizeof(ModelNodes)));
	memset(m_Nodes, 0, a_File->nodeAmount * sizeof(ModelNodes));

	m_nodeCount = a_File->nodeAmount;
	for (size_t nodeIndex = 0; nodeIndex < a_File->nodeAmount; nodeIndex++)
	{
		BBE::Node& curNode = a_File->nodes[nodeIndex];

		m_Nodes[nodeIndex].position = curNode.translation;
		m_Nodes[nodeIndex].rotation = curNode.rotation;
		m_Nodes[nodeIndex].scale	= curNode.scale;
			
		m_Nodes[nodeIndex].transformBuf = new TransformBuf(a_Gfx, 
			curNode.translation,
			curNode.rotation,
			curNode.scale);

		if (curNode.Parent) {
			m_Nodes[nodeIndex].transformBuf->SetParentTransform(
				curNode.Parent->translation,
				curNode.Parent->rotation,
				curNode.Parent->scale
			);
		}

		m_Nodes[nodeIndex].primitiveCount = curNode.mesh.primitiveCount;
		m_Nodes[nodeIndex].primitives = reinterpret_cast<ModelNodes::ModelPrimitive*>(malloc(m_Nodes[nodeIndex].primitiveCount * sizeof(ModelNodes::ModelPrimitive)));
			
		for (size_t primitiveIndex = 0; primitiveIndex < curNode.mesh.primitiveCount; primitiveIndex++)
		{
			BBE::Mesh::Primative& curPrim = curNode.mesh.primative[primitiveIndex];
			BBE::Vertex* vertices = reinterpret_cast<BBE::Vertex*>(malloc(curPrim.vertexCount * sizeof(BBE::Vertex)));

			if (vertices == nullptr)
				return;

			for (size_t i = 0; i < curPrim.vertexCount; i++)
			{
				vertices[i].pos = curPrim.vertices[i];
				vertices[i].texCoords = curPrim.texCoords[i];
				vertices[i].normals = curPrim.normals[i];
			}

			if (curPrim.Material.pbrMetallicRoughness.baseColorTexture.enabled)
			{
				char texturePath[64] = "";
				strcat(texturePath, a_File->gltfPath);
				strcat(texturePath, curPrim.Material.pbrMetallicRoughness.baseColorTexture.image.m_Path);

				m_Nodes[nodeIndex].primitives[primitiveIndex].m_Texture = new Texture(a_Gfx, texturePath);
				m_Nodes[nodeIndex].primitives[primitiveIndex].m_Sampler = new Sampler(a_Gfx);
			}
			else
			{
				m_Nodes[nodeIndex].primitives[primitiveIndex].m_Texture = nullptr;
				m_Nodes[nodeIndex].primitives[primitiveIndex].m_Sampler = nullptr;
			}
		
			m_Nodes[nodeIndex].primitives[primitiveIndex].vBuffer = new VertexBuffer(a_Gfx, vertices, curPrim.vertexCount);
			m_Nodes[nodeIndex].primitives[primitiveIndex].m_IndexBuffer = new IndexBuffer(a_Gfx, curPrim.indices, curPrim.indicesAmount, curPrim.indicesDataSize);
		}
	}

	m_VertexShader = m_CurVertexShader = a_VertexShader; 
	m_PixelShader = m_CurPixelShader = a_PixelShader;

	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		//{ "InstanceTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,							D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		//{ "InstanceTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	m_InputLayout = new InputLayout(a_Gfx, ied, a_Gfx.GetVertexShaderByteCode(m_CurVertexShader).Get());
	AddBind(m_InputLayout);

	m_Topology = new Topology(a_Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(m_Topology);
}

void Model::SetCurrentShader(uint32_t a_VertexShader, uint32_t a_PixelShader)
{
	m_CurVertexShader = a_VertexShader;
	m_CurPixelShader = a_PixelShader;
}

void Model::ResetShaders() {
	m_CurVertexShader = m_VertexShader;
	m_CurPixelShader = m_PixelShader;
}

void Model::AddToDraw(DirectX::XMMATRIX a_Transform)
{
	m_InstanceBuffer.push_back({ a_Transform });
}

void Model::Draw(Graphics& a_Gfx) noexcept {

	for (size_t curNode = 0; curNode < m_nodeCount; curNode++)
	{
		m_Nodes[curNode].transformBuf->Bind(a_Gfx, *m_CurTransform);
		for (size_t i = 0; i < m_Nodes[curNode].primitiveCount; i++)
		{
			if (m_Nodes[curNode].primitives[i].m_Texture != nullptr)
			{
				m_Nodes[curNode].primitives[i].m_Texture->Bind(a_Gfx);
				m_Nodes[curNode].primitives[i].m_Sampler->Bind(a_Gfx);
			}

			a_Gfx.BindShader(ShaderType::VertexShader, m_CurVertexShader);
			a_Gfx.BindShader(ShaderType::PixelShader, m_CurPixelShader);

			m_Nodes[curNode].primitives[i].vBuffer->Bind(a_Gfx);
			m_Nodes[curNode].primitives[i].m_IndexBuffer->Bind(a_Gfx);

			SetIndexBuffer(m_Nodes[curNode].primitives[i].m_IndexBuffer);
			Drawable::Draw(a_Gfx);
		}
	}
}

void Model::DrawInstanced(Graphics& a_Gfx, uint32_t a_InstanceCount) noexcept
{
	for (size_t curNode = 0; curNode < m_nodeCount; curNode++)
	{
		m_Nodes[curNode].transformBuf->Bind(a_Gfx);
		for (size_t i = 0; i < m_Nodes[curNode].primitiveCount; i++)
		{
			if (m_Nodes[curNode].primitives[i].m_Texture != nullptr)
			{
				m_Nodes[curNode].primitives[i].m_Texture->Bind(a_Gfx);
				m_Nodes[curNode].primitives[i].m_Sampler->Bind(a_Gfx);
			}

			a_Gfx.BindShader(ShaderType::VertexShader, m_CurVertexShader);
			a_Gfx.BindShader(ShaderType::PixelShader, m_CurPixelShader);

			m_Nodes[curNode].primitives[i].vBuffer->CreateInstanceBuffer(a_Gfx, m_InstanceBuffer.data(), sizeof(DirectX::XMMATRIX), m_InstanceBuffer.size());
			m_Nodes[curNode].primitives[i].vBuffer->Bind(a_Gfx);
			m_Nodes[curNode].primitives[i].m_IndexBuffer->Bind(a_Gfx);

			SetIndexBuffer(m_Nodes[curNode].primitives[i].m_IndexBuffer);
			Drawable::DrawInstanced(a_Gfx, m_InstanceBuffer.size());
		}
	}

	m_InstanceBuffer.clear();
}

void Model::Update(float a_DeltaTime) noexcept {}