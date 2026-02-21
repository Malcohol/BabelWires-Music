/**
 * Options for the seq2Tape program.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Result/result.hpp>

#include <memory>
#include <stdexcept>
#include <string>

struct ProgramOptions {
    static babelwires::ResultT<ProgramOptions> parse(int argc, char* argv[]);

    enum Mode { MODE_PRINT_HELP, MODE_CAPTURE, MODE_PLAYBACK, MODE_CONVERT, MODE_LIST_FORMATS, MODE_LIST_AUDIO };

    struct CaptureOptions {
        std::string m_outputFileName;
        /// System specific description of an input audio source.
        std::string m_inputCaptureSource;
        /// How many data files are expected.
        int m_numDataFiles;
        /// A name for the sequence, which gets embedded in the file.
        std::string m_sequenceName;
        /// A copyright string for the sequence, which gets embedded in the file.
        std::string m_copyright;
    };

    struct PlaybackOptions {
        std::string m_inputFileName;
        /// System specific description of an output audio destination.
        std::string m_outputPlaybackDest;
    };

    struct ConvertOptions {
        std::string m_inputFileName;
        std::string m_outputFileName;
        /// A name for the sequence, which gets embedded in the file.
        std::string m_sequenceName;
        /// A copyright string for the sequence, which gets embedded in the file.
        std::string m_copyright;
    };

    Mode m_mode;

    std::unique_ptr<CaptureOptions> m_captureOptions;
    std::unique_ptr<PlaybackOptions> m_playbackOptions;
    std::unique_ptr<ConvertOptions> m_convertOptions;
};

void writeUsage(const std::string& programName, bool playbackAvailable, bool captureAvailable, std::ostream& stream);
void writeHelp(const std::string& programName, bool playbackAvailable, bool captureAvailable, std::ostream& stream);
