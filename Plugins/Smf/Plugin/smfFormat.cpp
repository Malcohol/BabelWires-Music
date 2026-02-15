/**
 * Formats which support Standard MIDI Files.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>

#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <BaseLib/IO/fileDataSource.hpp>
#include <BaseLib/IO/fileDataSink.hpp>

namespace {

    const char s_formatIdentifier[] = "StandardMIDIFile";
    const char s_manufacturerName[] = "MIDI Manufacturers Association";
    const char s_productName[] = "Standard MIDI File";

} // namespace

smf::SmfSourceFormat::SmfSourceFormat()
    : SourceFileFormat(
          BW_LONG_ID(s_formatIdentifier, "Standard MIDI file (in)", "418b8238-c184-4885-a369-b24c4e0d06ec"), 1,
          Extensions{"mid", "smf"}) {}

babelwires::LongId smf::SmfSourceFormat::getThisIdentifier() {
    return s_formatIdentifier;
}

std::string smf::SmfSourceFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfSourceFormat::getProductName() const {
    return s_productName;
}

babelwires::ResultT<std::unique_ptr<babelwires::ValueTreeRoot>>
smf::SmfSourceFormat::loadFromFile(const std::filesystem::path& path, const babelwires::ProjectContext& projectContext,
                                   babelwires::UserLogger& userLogger) const {
    ASSIGN_OR_ERROR(auto dataSource, babelwires::FileDataSource::open(path));
    ON_ERROR(dataSource.close(babelwires::ErrorState::Error));
    ASSIGN_OR_ERROR(auto result, parseSmfSequence(dataSource, projectContext, userLogger));
    DO_OR_ERROR(dataSource.close());
    return std::move(result);
}

smf::SmfTargetFormat::SmfTargetFormat()
    : TargetFileFormat(BW_LONG_ID("SmfFile", "Standard MIDI file (out)", "f29cd3b0-8a46-4a21-bb7d-53acd6702944"), 1,
                       Extensions{"mid", "smf"}) {}

std::string smf::SmfTargetFormat::getManufacturerName() const {
    return s_manufacturerName;
}

std::string smf::SmfTargetFormat::getProductName() const {
    return s_productName;
}

std::unique_ptr<babelwires::ValueTreeRoot>
smf::SmfTargetFormat::createNewValue(const babelwires::ProjectContext& projectContext) const {
    return std::make_unique<babelwires::ValueTreeRoot>(projectContext.m_typeSystem,
                                                       babelwires::FileTypeT<SmfSequence>::getThisIdentifier());
}

babelwires::Result smf::SmfTargetFormat::writeToFile(const babelwires::ProjectContext& projectContext,
                                                      babelwires::UserLogger& userLogger,
                                                      const babelwires::ValueTreeRoot& contents,
                                                      const std::filesystem::path& path) const {
    ASSIGN_OR_ERROR(auto sink, babelwires::FileDataSink::open(path));
    ON_ERROR(sink.close(babelwires::ErrorState::Error));
    writeToSmf(projectContext, userLogger, contents, sink.stream());
    DO_OR_ERROR(sink.close());
    return {};
}
