#include <gtest/gtest.h>
#include "FileLoaders/Data/JsonParser.h"
#include "FileLoaders/Sound/WAVLoader.h"
#include "FileLoaders/Models/GLTFParser.h"

TEST(Fileloading, WAV)
{
    BBE::WAV WAVFile;
    WAVFile.LoadWav("Assets/Sound/music.wav");
    WAVFile.WriteWav("music_copy.wav");
}

TEST(Fileloading, Bitmap)
{

}

TEST(Fileloading, Json)
{
    BBE::JsonParser parser;
    parser.Parse("Assets/Data/data.json");
    parser.GetRootNode()["members"]->GetListBB()[0]->GetObjectBB()["name"]->SetStringBB("Stan Vogels");
    parser.GetRootNode()["members"]->GetListBB()[0]->GetObjectBB()["age"]->SetFloatBB(29);
    parser.WriteJson("writebackJson.json");
}

TEST(FileLoading, GLTF) 
{
    BBE::GLTFParser parser;
    BBE::GLTFFile* file = parser.Parse("Assets/Models/Cube/glTF/Cube.gltf");
}