#include <gtest/gtest.h>
#include "System/BBString.h"

TEST(Systems, BBString)
{
	BBE::BBString str("Testing");
	BBE::BBString str1("Testing");

	str.Append(str1);

	

}