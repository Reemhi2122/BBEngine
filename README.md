# BBEngine
Building Block Engine ("BBE") is my playground engine.

## Engine
### Allocators
After having some working graphics in my engine, I decided to do research in allocators before going to deep into the actual systems of my engine. This so that I can start out working with allocators from the beginning instead of changing it all later down the line. I researched multiple sources but most note worthy was [this article](https://www.gingerbill.org/series/memory-allocation-strategies/) about all the allocators I used in my engine. After creating them with the tutorial, ofcourse in my own way, I also added Virtual Memory at the advice of a friend of mine.

[Linear/Arena](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/ArenaAllocator.cpp), [Stack](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/StackAllocator.cpp), [Free list](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/FreeListAllocator.cpp), [Pool](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/PoolAllocator.cpp), [Buddy](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/BuddyAllocator.cpp)

### File support
Current files it supports is:
[Wav files](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/FileLoaders/WAVLoader.cpp), [BMP files](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/FileLoaders/BMPLoader.cpp).

## Graphics
The graphics library is currently based on DX11 and is very much still in progress.
