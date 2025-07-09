#include <d3d12.h>
#include <stack>

struct DescriptorFreeList
{
public:
	DescriptorFreeList() = default;
	~DescriptorFreeList() = default;

	bool Create(ID3D12Device* a_Device, D3D12_DESCRIPTOR_HEAP_DESC* a_Desc);
	bool Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* a_CPUHandle_out, D3D12_GPU_DESCRIPTOR_HANDLE* a_GPUHandle_out);
	bool Free(D3D12_CPU_DESCRIPTOR_HANDLE* a_CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE* a_GPUHandle);

	ID3D12DescriptorHeap* GetHeap() const { return m_Heap; };
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleStart() const { return m_StartCPUHandle; };
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleStart() const { return m_StartGPUHandle; };

private:
	ID3D12DescriptorHeap* m_Heap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_StartCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_StartGPUHandle;

	uint32_t m_IncrementSize;
	uint32_t m_DescriptorCapacity;

	std::stack<uint32_t> m_AvailableIndices;
};