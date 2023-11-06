#include <gtest/gtest.h>
#include "FileLoaders/Data/JsonParser.h"
#include "FileLoaders/Sound/WAVLoader.h"

TEST(Fileloading, WAV)
{
    BBE::WAV WAVFile;
    WAVFile.LoadWav("assets/Sound/music.wav");
    WAVFile.WriteWav("music_copy.wav");
}

TEST(Fileloading, Bitmap)
{

}

TEST(Fileloading, Json)
{
    BBE::JsonParser parser;
    parser.Parse("assets/Data/sample.json");
    parser.GetRootNode()["children"]->GetListBB()[4]->GetObjectBB()["name"]->SetStringBB("Stan Vogels");
    parser.GetRootNode()["children"]->GetListBB()[4]->GetObjectBB()["age"]->SetFloatBB(29);
    parser.WriteJson("writebackJson.json");
}