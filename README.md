# BBEngine

Building Block Engine ("BBE") is my playground engine.

The goal of this playground engine is to custom create as much system as I can, this includes creating my own containers, file readers / writers, allocators, multithreading system, physics system and so on. It will also use a custom created DX11 renderer which I will be creating alongside the engine itself.

![Screenshot 2025-02-14 164351](https://github.com/user-attachments/assets/c06e191b-0cd4-4e23-ac66-8c1fc3ae64d6)

## Engine
### Allocators
After having some working graphics in my engine, I decided to do research in allocators before going to deep into the actual systems of my engine. This so that I can start out working with allocators from the beginning instead of changing it all later down the line. I researched multiple sources but most note worthy was [this article](https://www.gingerbill.org/series/memory-allocation-strategies/) about all the allocators I used in my engine. After creating them with the tutorial, ofcourse in my own way, I also added Virtual Memory at the advice of a friend of mine.

[Linear/Arena](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/ArenaAllocator.cpp), [Stack](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/StackAllocator.cpp), [Free list](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/FreeListAllocator.cpp), [Pool](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/PoolAllocator.cpp), [Buddy](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/Allocators/BuddyAllocator.cpp)

### Containers
Current containers in the engine:
[LinkedList](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Include/Containers/LinkedList.h),[Pool](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Include/Containers/Pool.h),[Queue](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Include/Containers/Queue.h),

### File support
Current files it supports is:
[Wav files](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/FileLoaders/Sound/WAVLoader.cpp), [BMP files](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/FileLoaders/Image/BMPLoader.cpp), [JSON files](https://github.com/Reemhi2122/BBEngine/blob/main/BBEngine/BBEngine/Source/FileLoaders/Data/JsonParser.cpp)

## Graphics
The graphics library is currently based on DX11 and is very much still in progress.
