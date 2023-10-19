#include <gtest/gtest.h>
#include "FileLoaders/Data/JsonParser.h"

TEST(Fileloading, WAV)
{

}

TEST(Fileloading, Bitmap)
{

}

TEST(Fileloading, Json)
{
    BBE::JsonParser parser;
    parser.Parse("assets/Data/sample.json");
    parser.WriteJson("writebackJson.json");

    //bool test = parser.GetRootNode()["truebool"]->GetBoolBB()
}