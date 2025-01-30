#include <test/libsolidity/util/compiler/CompilerHost.h>

using namespace solidity;
using namespace solidity::frontend::test;

CompilerOutput const& CompilerHost::compile(CompilerInput const& _input)
{
    auto compile = [&](auto& compiler) { return compiler.compile(_input); };
    auto output = std::visit(compile, m_compiler);

    m_output.emplace(output);

    return this->output();
}

CompilerOutput const& CompilerHost::output() const
{
    solAssert(m_output.has_value(), "No output found. Please compile first.");
    return m_output.value();
}
