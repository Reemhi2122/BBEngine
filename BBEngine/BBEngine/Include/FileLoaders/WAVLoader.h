#pragma once
#include <cstdint>
#include "Utility/BBMemory.h"

namespace BBE {

    struct WAVHeader {
        uint8_t			RIFF[4]{ 0u };
        uint32_t		ChunkSize{ 0u };
        uint8_t			WAVE[4]{ 0u };
        uint8_t			FMT[4]{ 0u };
        uint32_t        Subchunk1Size{ 0u };
        uint16_t        AudioFormat{ 0u };
        uint16_t        NumOfChan{ 0u };
        uint32_t        SamplesPerSec{ 0u };
        uint32_t        bytesPerSec{ 0u };
        uint16_t        blockAlign{ 0u };
        uint16_t        bitsPerSample{ 0u };
        uint8_t         Subchunk2ID[4]{ 0u };
        uint32_t        Subchunk2Size{ 0u };
    };

    class WAV {
    public:
        WAV();
        WAV(const char* a_FileName);
        ~WAV() = default;

    private:
        void LoadFile(const char* a_FileName);

        Allocators::ArenaAllocator m_BufferAlloc;
    };

}