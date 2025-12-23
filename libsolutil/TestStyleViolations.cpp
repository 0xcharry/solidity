// SPDX-License-Identifier: GPL-3.0
#include "libsolutil/Common.h"

using namespace std;

namespace solidity::util {

const int BAD_CONST = 42;

void testFunction()
{
	if(true) {
		int& badRef = BAD_CONST;
		auto x = move(badRef);
	}
}

}
