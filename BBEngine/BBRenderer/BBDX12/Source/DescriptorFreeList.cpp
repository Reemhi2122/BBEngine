#include "DescriptorFreeList.h"

bool DescriptorFreeList::Create(ID3D12Device* a_Device, D3D12_DESCRIPTOR_HEAP_DESC* a_Desc)
{
	HRESULT hres;
	hres = a_Device->CreateDescriptorHeap(a_Desc, IID_PPV_ARGS(&m_Heap));
	if (FAILED(hres))
	{
		printf("[GFX][DescriptorFreeList]: Failed to create Descriptor Heap!");
		return false;
	}

	m_StartGPUHandle = m_Heap->GetGPUDescriptorHandleForHeapStart();
	m_StartCPUHandle = m_Heap->GetCPUDescriptorHandleForHeapStart();
	
	m_DescriptorCapacity = a_Desc->NumDescriptors;
	m_IncrementSize = a_Device->GetDescriptorHandleIncrementSize(a_Desc->Type);
	
	for (int32_t i = a_Desc->NumDescriptors - 1; i >= 0; i--)
	{
		m_AvailableIndices.push(i);
	}

	return true;
}

bool DescriptorFreeList::Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* a_CPUHandle_out, D3D12_GPU_DESCRIPTOR_HANDLE* a_GPUHandle_out)
{
	if (m_AvailableIndices.empty())
	{
		printf("[GFX][DescriptorFreeList]: Failed to create Descriptor Heap!");
		return false;
	}

	uint32_t curIndex = m_AvailableIndices.top();
	m_AvailableIndices.pop();

	a_CPUHandle_out->ptr = m_StartCPUHandle.ptr + (curIndex * m_IncrementSize);
	a_GPUHandle_out->ptr = m_StartGPUHandle.ptr + (curIndex * m_IncrementSize);
	
	return true;
}

bool DescriptorFreeList::Free(D3D12_CPU_DESCRIPTOR_HANDLE a_CPUHandle, D3D12_GPU_DESCRIPTOR_HANDLE a_GPUHandle)
{
	uint32_t cpuIndex = (a_CPUHandle.ptr - m_StartCPUHandle.ptr) / m_IncrementSize;
	uint32_t gpuIndex = (a_GPUHandle.ptr - m_StartGPUHandle.ptr) / m_IncrementSize;

	if (cpuIndex != gpuIndex)
	{
		printf("[GFX][DescriptorFreeList]: Failed to get correct GPU & CPU indices!");
		return false;
	}

	m_AvailableIndices.push(cpuIndex);

	return true;
}
