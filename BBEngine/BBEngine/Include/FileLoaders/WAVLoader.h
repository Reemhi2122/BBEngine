#pragma once
#include <cstdint>
#include "Utility/BBMemory.h"

namespace BBE {

    struct RIFF {
        uint8_t			Type[4]{ 0u };
        uint32_t		ChunkSize{ 0u };
        uint8_t			WAVE[4]{ 0u };
    };

    struct FMT {
        uint8_t			fmt[4]{ 0u };
        uint32_t        Subchunk1Size{ 0u };
        uint16_t        AudioFormat{ 0u };
        uint16_t        NumOfChan{ 0u };
        uint32_t        SamplesPerSec{ 0u };
        uint32_t        bytesPerSec{ 0u };
        uint16_t        blockAlign{ 0u };
        uint16_t        bitsPerSample{ 0u };
    };

    struct Data {
        uint8_t         Subchunk2ID[4]{ 0u };
        uint32_t        Subchunk2Size{ 0u };
    };

    class WAV {
    public:
        WAV();
        WAV(const char* a_FileName);
        ~WAV() = default;

        void LoadWav(const char* a_FilePath); //Note(Stan): This is currently only loading the RIFF, FMT and Data. It will skip all the other chunks.
        void WriteWav(const char* a_FilePath); //Note(Stan): See no reason to implement this yet, don't think I would save WAV Files

    private:
        Allocators::ArenaAllocator m_BufferAlloc;
        Allocators::ArenaAllocator m_WavBufferAlloc;

        RIFF* m_RiffHeader;
        FMT* m_FmtHeader;
        Data* m_DataHeader;

        unsigned char* m_Data;
    };

}