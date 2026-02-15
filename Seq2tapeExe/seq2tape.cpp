/**
 * The seq2tape main function.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeExe/seq2tapeOptions.hpp>

#include <Seq2tapeExe/audioInit.hpp>

#include <Seq2tapeLib/Audio/FileAudio/fileAudioDest.hpp>
#include <Seq2tapeLib/Audio/FileAudio/fileAudioSource.hpp>
#include <Seq2tapeLib/Audio/audioInterface.hpp>
#include <Seq2tapeLib/seq2tapeContext.hpp>
#include <Seq2tapeLib/tapeFile.hpp>
#include <Seq2tapeLib/tapeFileFormat.hpp>

#include <BaseLib/IO/fileDataSource.hpp>
#include <BaseLib/IO/outFileStream.hpp>
#include <BaseLib/Identifiers/identifierRegistry.hpp>
#include <BaseLib/Log/unifiedLog.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

struct Context : seq2tape::Seq2TapeContext {
    babelwires::AudioInterfaceRegistry& m_audioInterfaceRegistry;
    babelwires::FileAudioDestRegistry& m_fileAudioDestRegistry;
};

babelwires::Result convertMode(const Context& context, const ProgramOptions::ConvertOptions& convertOptions) {
    if (auto inFormat = context.m_tapeFileRegistry.getEntryByFileName(convertOptions.m_inputFileName)) {
        auto infile = babelwires::FileDataSource::open(convertOptions.m_inputFileName);
        if (!infile) {
            return babelwires::Error() << "Cannot open input file " << infile.error().toString();
        }
        FINALLY_WITH_ERRORSTATE(infile->close(errorState));
        auto tapeFile = seq2tape::TapeFile::load(*infile);
        if (!tapeFile) {
            return babelwires::Error() << "Cannot parse input file: " << tapeFile.error().toString();
        }
        if (tapeFile->getFormatIdentifier() != inFormat->getIdentifier()) {
            return babelwires::Error() << "File extension does not match file contents";
        }

        if (auto outFormat = context.m_tapeFileRegistry.getEntryByFileName(convertOptions.m_outputFileName)) {
            // Converting between TapeFiles of the same format allows name and copyright to be overwritten.
            // No other cases make sense.
            if (outFormat != inFormat) {
                return babelwires::Error() << "You cannot convert between formats using seq2tape. Use BabelWires instead";
            }

            if (!convertOptions.m_sequenceName.empty()) {
                tapeFile->setName(convertOptions.m_sequenceName);
            }
            if (!convertOptions.m_copyright.empty()) {
                tapeFile->setCopyright(convertOptions.m_copyright);
            }

            try {
                babelwires::OutFileStream outfile(convertOptions.m_outputFileName.c_str(), std::ios_base::binary);
                tapeFile->write(outfile);
                outfile.close();
            } catch (const std::exception& e) {
                return babelwires::Error() << "Cannot write file. " << e.what();
            }
        } else {
            std::unique_ptr<babelwires::AudioDest> outfile =
                context.m_fileAudioDestRegistry.createFileAudioDest(convertOptions.m_outputFileName.c_str(), 1);
            const int numDataFiles = tapeFile->getNumDataFiles();
            if (numDataFiles == 0) {
                return babelwires::Error() << "Provided file has no contents";
            }
            if (!convertOptions.m_sequenceName.empty()) {
                return babelwires::Error() << "You cannot set name when writing an audio file";
            }
            if (!convertOptions.m_copyright.empty()) {
                return babelwires::Error() << "You cannot set copyright when writing an audio file";
            }

            inFormat->writeToAudio(tapeFile->getDataFile(0), *outfile);
            for (int i = 1; i < numDataFiles; ++i) {
                // TODO interfile gap.
                inFormat->writeToAudio(tapeFile->getDataFile(i), *outfile);
            }
        }
    } else if (auto outFormat = context.m_tapeFileRegistry.getEntryByFileName(convertOptions.m_outputFileName)) {
        // TODO Error handling.
        babelwires::FileAudioSource fileCallback(convertOptions.m_inputFileName.c_str());
        std::unique_ptr<seq2tape::TapeFile> tapeFile = std::make_unique<seq2tape::TapeFile>(outFormat->getIdentifier());
        if (convertOptions.m_sequenceName.empty()) {
            tapeFile->setName(convertOptions.m_outputFileName);
        } else {
            tapeFile->setName(convertOptions.m_sequenceName);
        }
        tapeFile->setCopyright(convertOptions.m_copyright);
        std::unique_ptr<seq2tape::TapeFile::DataFile> dataFile = outFormat->loadFromAudio(fileCallback);
        tapeFile->addDataFile(std::move(dataFile));
        babelwires::OutFileStream outfile(convertOptions.m_outputFileName.c_str(), std::ios_base::binary);
        tapeFile->write(outfile);
        outfile.close();
    } else {
        return babelwires::Error() << "Neither input nor output format are recognized seq2tape formats";
    }
    return {};
}

babelwires::Result playbackMode(const Context& context, const ProgramOptions::PlaybackOptions& playbackOptions) {
    auto inFormat = context.m_tapeFileRegistry.getEntryByFileName(playbackOptions.m_inputFileName);
    if (!inFormat) {
        return babelwires::Error() << "The input file is not a recognized seq2tape format";
    }
    auto infile = babelwires::FileDataSource::open(playbackOptions.m_inputFileName);
    THROW_ON_ERROR(infile, babelwires::IoException);
    FINALLY_WITH_ERRORSTATE(infile->close(errorState));
    const auto tapeFile = seq2tape::TapeFile::load(*infile);
    THROW_ON_ERROR(tapeFile, babelwires::IoException);
    if (tapeFile->getFormatIdentifier() != inFormat->getIdentifier()) {
        return babelwires::Error() << "File extension does not match file contents";
    }
    std::unique_ptr<babelwires::AudioDest> audioDest =
        context.m_audioInterfaceRegistry.getDestination(playbackOptions.m_outputPlaybackDest);
    const int numDataFiles = tapeFile->getNumDataFiles();
    if (numDataFiles == 0) {
        return babelwires::Error() << "Provided file has no contents";
    }
    if (!tapeFile->getName().empty()) {
        std::cout << "Name: " << tapeFile->getName() << ".\n";
    }
    if (!tapeFile->getCopyright().empty()) {
        std::cout << "Copyright: " << tapeFile->getCopyright() << ".\n";
    }
    std::cout << "Format: " << inFormat->getName() << ".\n";
    std::cout << "Playing file " << 1 << "/" << numDataFiles << ".\n";
    inFormat->writeToAudio(tapeFile->getDataFile(0), *audioDest);
    for (int i = 1; i < numDataFiles; ++i) {
        // TODO interfile gap.
        inFormat->writeToAudio(tapeFile->getDataFile(i), *audioDest);
        std::cout << "Playing file " << i + 1 << "/" << numDataFiles << ".\n";
    }
    return {};
}

babelwires::Result captureMode(const Context& context, const ProgramOptions::CaptureOptions& captureOptions) {
    auto outFormat = context.m_tapeFileRegistry.getEntryByFileName(captureOptions.m_outputFileName);
    if (!outFormat) {
        return babelwires::Error() << "The output file is not a recognized seq2tape format";
    }
    babelwires::OutFileStream outFile(captureOptions.m_outputFileName.c_str(), std::ios_base::binary);
    std::unique_ptr<babelwires::AudioSource> audioSource =
        context.m_audioInterfaceRegistry.getSource(captureOptions.m_inputCaptureSource);
    std::unique_ptr<seq2tape::TapeFile> tapeFile = std::make_unique<seq2tape::TapeFile>(outFormat->getIdentifier());
    if (captureOptions.m_sequenceName.empty()) {
        tapeFile->setName(captureOptions.m_outputFileName);
    } else {
        tapeFile->setName(captureOptions.m_sequenceName);
    }
    tapeFile->setCopyright(captureOptions.m_copyright);
    for (int i = 0; i < captureOptions.m_numDataFiles; ++i) {
        std::unique_ptr<seq2tape::TapeFile::DataFile> dataFile = outFormat->loadFromAudio(*audioSource);
        tapeFile->addDataFile(std::move(dataFile));
    }
    tapeFile->write(outFile);
    outFile.close();
    return {};
}

int main(int argc, char* argv[]) {
    babelwires::UnifiedLog log;
    babelwires::DebugLogger::swapGlobalDebugLogger(&log);
    babelwires::IdentifierRegistryScope identifierRegistry;

    seq2tape::TapeFileFormatRegistry tapeFileRegistry;
    babelwires::AudioInterfaceRegistry audioInterfaceRegistry;
    babelwires::FileAudioDestRegistry fileAudioDestRegistry;

    Context context{tapeFileRegistry, audioInterfaceRegistry, fileAudioDestRegistry};

    // Register plugins here.

    babelwires::init_audio(context.m_audioInterfaceRegistry);
    const bool playbackAvailable = !context.m_audioInterfaceRegistry.getDestinationNames().empty();
    const bool captureAvailable = !context.m_audioInterfaceRegistry.getSourceNames().empty();

    ProgramOptions options;
    auto result = options.parse(argc, argv);
    if (!result) {
        std::cerr << result.error().toString() << std::endl;
        writeUsage(argv[0], playbackAvailable, captureAvailable, std::cerr);
        return 1;
    }

    // TODO Register seq2tape plugins formats here.

    switch (options.m_mode) {
        case ProgramOptions::MODE_PRINT_HELP: {
            writeHelp(argv[0], playbackAvailable, captureAvailable, std::cout);
            break;
        }
        case ProgramOptions::MODE_CAPTURE: {
            if (!captureAvailable) {
                std::cerr
                    << "No source audio interfaces were registered, so audio capture is unavailable.";
                return EXIT_FAILURE;
            }
            const auto result = captureMode(context, *options.m_captureOptions);
            if (!result) {
                std::cerr << result.error().toString() << std::endl;
                return EXIT_FAILURE;
            }
            break;
        }
        case ProgramOptions::MODE_PLAYBACK: {
            if (!playbackAvailable) {
                std::cerr
                    << "No destination audio interfaces were registered, so audio playback is unavailable.";
                return EXIT_FAILURE;
            }
            const auto result = playbackMode(context, *options.m_playbackOptions);
            if (!result) {
                std::cerr << result.error().toString() << std::endl;
                return EXIT_FAILURE;
            }
            break;
        }
        case ProgramOptions::MODE_CONVERT: {
            const auto result = convertMode(context, *options.m_convertOptions);
            if (!result) {
                std::cerr << result.error().toString() << std::endl;
                return EXIT_FAILURE;
            }
            break;
        }
        case ProgramOptions::MODE_LIST_FORMATS: {
            std::cout << "Supported tape file formats:" << std::endl;
            for (const auto& e : context.m_tapeFileRegistry) {
                std::cout << "  " << e.getFileExtensions()[0] << " (" << e.getManufacturerName() << " "
                            << e.getProductName() << ")" << std::endl;
            }
            std::cout << "Supported audio file formats for writing:" << std::endl;
            for (const auto& e : context.m_fileAudioDestRegistry) {
                std::cout << "  " << e.getFileExtensions()[0] << " (" << e.getName() << ")" << std::endl;
            }
            std::cout << "Many additional audio file formats can be read." << std::endl;
            break;
        }
        case ProgramOptions::MODE_LIST_AUDIO: {
            std::cout << "Available audio destinations:" << std::endl;
            auto destinations = context.m_audioInterfaceRegistry.getDestinationNames();
            for (const auto& d : destinations) {
                std::cout << "  " << d << std::endl;
            }
            std::cout << "Available audio sources:" << std::endl;
            auto sources = context.m_audioInterfaceRegistry.getSourceNames();
            for (const auto& s : sources) {
                std::cout << "  " << s << std::endl;
            }
            break;
        }
        default:
            assert(false);
    }
    babelwires::shutdown_audio(context.m_audioInterfaceRegistry);
    return EXIT_SUCCESS;
}
