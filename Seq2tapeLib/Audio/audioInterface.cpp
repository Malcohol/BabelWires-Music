/**
 * A system for registering AudioSources and AudioDests.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/Audio/audioInterface.hpp>

#include <Seq2tapeLib/Audio/audioDest.hpp>
#include <Seq2tapeLib/Audio/audioSource.hpp>
#include <BaseLib/Log/debugLogger.hpp>

#include <sstream>

babelwires::AudioInterface::AudioInterface(LongId identifier, VersionNumber version)
    : RegistryEntry(identifier, version) {}

babelwires::AudioInterfaceRegistry::AudioInterfaceRegistry()
    : Registry<AudioInterface>("Audio Interface Registry") {}

namespace {
    constexpr std::string_view delimiter = "::";

    std::string addPrefix(std::string_view prefix, std::string_view name) {
        std::ostringstream prefixedName;
        prefixedName << prefix << delimiter << name;
        return prefixedName.str();
    }

    std::tuple<std::string_view, std::string_view> splitAtColons(std::string_view name) {
        std::string_view prefix;
        std::string_view suffix = name;

        const std::size_t prefixLength = name.find(delimiter);
        if (prefixLength != std::string::npos) {
            prefix = std::string_view(name.data(), prefixLength);
            suffix = std::string_view(name.data() + prefixLength + 2, name.size() - prefixLength - 2);
        }
        return {prefix, suffix};
    }
} // namespace

std::vector<std::string> babelwires::AudioInterfaceRegistry::getDestinationNames() const {
    std::vector<std::string> names;
    for (auto it = begin(); it != end(); ++it) {
        auto interfaceDestinations = it->getDestinationNames();
        for (auto dit : interfaceDestinations) {
            names.emplace_back(addPrefix(it->getName(), dit));
        }
    }
    return names;
}

std::unique_ptr<babelwires::AudioDest>
babelwires::AudioInterfaceRegistry::getDestination(std::string_view destinationName) const {
    const auto [interfaceId, destName] = splitAtColons(destinationName);
    if (interfaceId.empty()) {
        logDebug() << "AudioInterfaceRegistry::getDestination called without prefix";
    }
    for (auto it = begin(); it != end(); ++it) {
        if (it->getIdentifier() != interfaceId) {
            continue;
        }
        auto interfaceDestinations = it->getDestinationNames();
        if (std::find(interfaceDestinations.begin(), interfaceDestinations.end(), destName) !=
            interfaceDestinations.end()) {
            return it->getDestination(destName);
        }
    }
    return nullptr;
}

std::vector<std::string> babelwires::AudioInterfaceRegistry::getSourceNames() const {
    std::vector<std::string> names;
    for (auto it = begin(); it != end(); ++it) {
        auto interfaceSourceNames = it->getSourceNames();
        for (auto sit : interfaceSourceNames) {
            names.emplace_back(addPrefix(it->getName(), sit));
        }
    }
    return names;
}

std::unique_ptr<babelwires::AudioSource>
babelwires::AudioInterfaceRegistry::getSource(std::string_view sourceName) const {
    const auto [interfaceId, srcName] = splitAtColons(sourceName);
    if (interfaceId.empty()) {
        logDebug() << "AudioInterfaceRegistry::getSource called without prefix";
    }
    for (auto it = begin(); it != end(); ++it) {
        if (it->getIdentifier() != interfaceId) {
            continue;
        }
        auto interfaceSourceNames = it->getSourceNames();
        if (std::find(interfaceSourceNames.begin(), interfaceSourceNames.end(), srcName) !=
            interfaceSourceNames.end()) {
            return it->getSource(srcName);
        }
    }
    return nullptr;
}
