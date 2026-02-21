/**
 * A TapeFile holds a decoded version of the audio storage for a sequence.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/common.hpp>
#include <BaseLib/Identifiers/identifier.hpp>
#include <BaseLib/Result/result.hpp>

#include <memory>
#include <string>

namespace babelwires {
    struct AudioSource;
    struct AudioDest;
    class DataSource;
    class DataSink;
} // namespace babelwires

namespace seq2tape {

    /// Holds a decoded version of the audio storage for a sequence.
    /// This usually contains one data file, but sometimes more.
    /// This allows a sequence to correspond to more than one loading event.
    class TapeFile {
      public:
        /// Construct an empty TapeFile for the given format.
        TapeFile(babelwires::LongId formatIdentifier);

        static babelwires::ResultT<TapeFile> load(babelwires::DataSource& dataSource);

        /// Write the PmcTapFile as bytes to the sink.
        void write(babelwires::DataSink& dataSink) const;

        babelwires::LongId getFormatIdentifier() const;

        std::string getName() const;
        void setName(std::string name);

        std::string getCopyright() const;
        void setCopyright(std::string copyright);

        /// The unit of loading, i.e. a device can load a data file independently.
        typedef std::vector<babelwires::Byte> DataFile;

        int getNumDataFiles() const;
        const DataFile& getDataFile(int i) const;
        void addDataFile(std::unique_ptr<DataFile> dataFile);

      private:
        /// Load the TapeFile from the data stream.
        TapeFile() = default;

      protected:
        babelwires::LongId m_formatIdentifier;
        std::string m_name;
        std::string m_copyright;

        std::vector<std::unique_ptr<DataFile>> m_dataFiles;
    };

} // namespace seq2tape
