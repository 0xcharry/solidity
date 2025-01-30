#include <test/libsolidity/util/compiler/InternalCompiler.h>

#include <range/v3/algorithm.hpp>

using namespace solidity;
using namespace solidity::evmasm;
using namespace solidity::frontend;
using namespace solidity::frontend::test;

using solidity::frontend::CompilerStack;

namespace {
    /// Configures the @param _stack with the @param _input given.
    void configureStack(CompilerStack& _stack, CompilerInput const& _input)
    {
        _stack.setSources(_input.sourceCode);
        _stack.setLibraries(_input.libraryAddresses);

        if (_input.evmVersion.has_value())
            _stack.setEVMVersion(_input.evmVersion.value());
        if (_input.optimise.has_value())
            _stack.setOptimiserSettings(_input.optimise.value());
        if (_input.optimiserSettings.has_value())
            _stack.setOptimiserSettings(_input.optimiserSettings.value());
        if (_input.revertStrings.has_value())
            _stack.setRevertStringBehaviour(_input.revertStrings.value());
        if (_input.metadataFormat.has_value())
            _stack.setMetadataFormat(_input.metadataFormat.value());
        if (_input.metadataHash.has_value())
            _stack.setMetadataHash(_input.metadataHash.value());
        if (_input.viaIR.has_value())
            _stack.setViaIR(_input.viaIR.value());
        _stack.setEOFVersion(_input.eofVersion);
    }

    /// @returns a formatted output of all errors that occurred during
	/// compilation.
    std::string formattedStackErrors(CompilerStack const& _stack)
    {
        auto toFormatted = [&](auto const& _error) {
            return SourceReferenceFormatter::formatErrorInformation(*_error, _stack, true, false);
        };

        return ranges::fold_left(
            _stack.errors() | ranges::views::transform(toFormatted) | ranges::to<std::vector>(),
            std::string{},
            [](std::string _acc, std::string _error) { return _acc.append(_error); }
        );
    }
}

CompilerOutput InternalCompiler::compile(CompilerInput const& _input)
{
    CompilerStack stack;

    // Configure, compile & build output
    configureStack(stack, _input);
    bool success = stack.compile();

    auto toCompiledContract = [&](auto const* _contract) {
        return CompiledContract::fromStack(
            stack,
            _contract->fullyQualifiedName()
        );
    };

    auto toSourceUnit = [&](auto const& _sourceName) {
        auto contractDefinitions = stack.contractDefinitions(_sourceName);
        auto sourceContracts = contractDefinitions
            | ranges::views::transform(toCompiledContract)
            | ranges::to<std::vector>();
        
        return std::make_pair(_sourceName, sourceContracts);
    };

    auto sourceNames = stack.sourceNames();
    auto contracts = sourceNames
        | ranges::views::transform(toSourceUnit)
        | ranges::to<SourceUnits>();

    return CompilerOutput{
        std::move(contracts),
        success,
        stack.errors(),
        formattedStackErrors(stack)
    };
}


