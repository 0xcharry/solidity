/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <liblangutil/SourceReferenceFormatter.h>

#include <test/libsolidity/util/compiler/Compiler.h>

using namespace solidity::langutil;

namespace solidity::frontend::test
{
/**
 * Abstraction of the `CompilerStack`.
 */
struct InternalCompiler
{
	/// Compiles using a `CompilerStack` internally.
	/// @param _input the compiler input with sources, options set.
	/// @returns the aggregated compiler output.
	CompilerOutput compile(CompilerInput const& _input);
};

}
