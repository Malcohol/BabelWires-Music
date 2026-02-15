/**
 * Formats which support Standard MIDI Files.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/FileFormat/sourceFileFormat.hpp>
#include <BabelWiresLib/FileFormat/targetFileFormat.hpp>

namespace smf {
    /// Format for loading Standard MIDI Files..
    class SmfSourceFormat : public babelwires::SourceFileFormat {
      public:
        SmfSourceFormat();
        static babelwires::LongId getThisIdentifier();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual babelwires::ResultT<std::unique_ptr<babelwires::ValueTreeRoot>>
        loadFromFile(const std::filesystem::path& path, const babelwires::ProjectContext& projectContext,
                     babelwires::UserLogger& userLogger) const override;
    };

    /// Format for creating Standard MIDI Files.
    class SmfTargetFormat : public babelwires::TargetFileFormat {
      public:
        SmfTargetFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::ValueTreeRoot>
        createNewValue(const babelwires::ProjectContext& projectContext) const override;
        virtual babelwires::Result writeToFile(const babelwires::ProjectContext& projectContext,
                             babelwires::UserLogger& userLogger,
                             const babelwires::ValueTreeRoot& contents,
                             const std::filesystem::path& path) const override;
    };
} // namespace smf
