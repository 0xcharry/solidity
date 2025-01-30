#include <test/libsolidity/util/compiler/Compiler.h>

#include <range/v3/view/concat.hpp>
#include <range/v3/algorithm.hpp>

#include <test/libsolidity/util/SoltestErrors.h>

using namespace solidity;
using namespace solidity::frontend::test;

EventSignature EventSignature::fromEvent(EventDefinition const& _event)
{
	auto externalSignature = _event.functionType(true)->externalSignature();
	bool isAnonymous = _event.isAnonymous();

	auto toString = [](auto const& _param) { return _param->type()->toString(true); };

	auto indexedTypes =
		_event.parameters() |
		ranges::views::filter([](auto const& _param) { return _param->isIndexed(); }) |
		ranges::views::transform(toString) |
		ranges::to<std::vector>();
	auto nonIndexedTypes =
		_event.parameters() |
		ranges::views::filter([](auto const& _param) { return !_param->isIndexed(); }) |
		ranges::views::transform(toString) |
		ranges::to<std::vector>();

	return EventSignature{
		externalSignature,
		indexedTypes,
		nonIndexedTypes,
		isAnonymous
	};
}

CompiledContract CompiledContract::fromStack(
    CompilerStack const& _stack,
	std::string const& _name
)
{
    /// Collect assembly items if available
    std::optional<evmasm::AssemblyItems> assemblyItems;
    std::optional<evmasm::AssemblyItems> runtimeAssemblyItems;
    if (
        auto const* items = _stack.assemblyItems(_name);
        items != nullptr
    )
        assemblyItems = *items;
    if (
        auto const* items = _stack.runtimeAssemblyItems(_name);
        items != nullptr
    )
        runtimeAssemblyItems = *items;

    /// Collect event signatures
    auto const& contract = _stack.contractDefinition(_name);
    auto const& events = contract.events();
    auto const& interfaceEvents = contract.usedInterfaceEvents();

    auto toSignature = [](EventDefinition const* _event) {
        soltestAssert(_event);
        return EventSignature::fromEvent(*_event);
    };

    auto eventSignatures = ranges::views::concat(events, interfaceEvents)
        | ranges::views::transform(toSignature)
        | ranges::to<std::vector>();

    return CompiledContract{
        _name,
        _stack.object(_name).bytecode,
        _stack.runtimeObject(_name).bytecode,
        !_stack.object(_name).linkReferences.empty(),
        _stack.cborMetadata(_name),
        assemblyItems,
        runtimeAssemblyItems,
        _stack.metadata(_name),
        _stack.contractABI(_name),
        _stack.interfaceSymbols(_name),
        eventSignatures
    };
}

CompiledContract const* CompilerOutput::contract(ContractName const& _name) const
{
    if (
    	auto const& source = m_sourceUnits.find(std::string{_name.source()});
     	source != m_sourceUnits.end()
    )
    {
        if (!_name.contract().empty())
        {
            for (auto const& contract: source->second)
                if (contract.name == _name)
                    return &contract;
        }
        else
        {
            if (!source->second.empty())
                return &source->second.back();
        }
    }

    return nullptr;
}

EventSignature const* CompilerOutput::matchEvent(util::h256 const& _hash) const
{
    for (auto const& contracts: m_sourceUnits | ranges::views::values)
        for (auto const& contract: contracts)
            for (auto const& event: contract.eventSignatures)
                if (!event.isAnonymous && keccak256(event.signature) == _hash)
                    return &event;

    return nullptr;
}

bool CompilerOutput::success() const
{
    return m_success;
}

std::optional<langutil::Error> CompilerOutput::findError(
    langutil::Error::Type _type
) const
{
    for (auto const& error: m_errors)
		if (error->type() == _type)
			return *error;

    return std::nullopt;
}

std::string_view CompilerOutput::errorInformation() const
{
    return m_errorInformation;
}
