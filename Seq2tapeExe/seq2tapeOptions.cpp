/**
 * Options for the seq2Tape program.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/Audio/FileAudio/fileAudioDest.hpp>
#include <Seq2tapeExe/seq2tapeOptions.hpp>

namespace {
    const char s_helpString[] = "help";
    const char s_convertString[] = "convert";
    const char s_playString[] = "play";
    const char s_captureString[] = "capture";
    const char s_formatsString[] = "formats";
    const char s_audioString[] = "audio";
} // namespace

babelwires::ResultT<ProgramOptions> ProgramOptions::parse(int argc, char* argv[]) {
    ProgramOptions options;
    options.m_mode = MODE_PRINT_HELP;
    int option = 0;

    if (argc < 2) {
        return babelwires::Error() << "No mode specified";
    }

    const std::string modeArg = argv[1];

    if ((modeArg == s_helpString) || (modeArg == "-h") || (modeArg == "--help")) {
        options.m_mode = MODE_PRINT_HELP;
    } else if (modeArg == s_captureString) {
        options.m_mode = MODE_CAPTURE;
        options.m_captureOptions = std::make_unique<CaptureOptions>();
        options.m_captureOptions->m_inputCaptureSource = "default";
        options.m_captureOptions->m_numDataFiles = 1;

        int i = 2;
        while (i < argc - 1) {
            std::string nextArg = argv[i];
            if (nextArg == "-s") {
                options.m_captureOptions->m_inputCaptureSource = argv[i + 1];
                i += 2;
            } else if (nextArg == "-f") {
                options.m_captureOptions->m_numDataFiles = atoi(argv[i + 1]);
                i += 2;
            } else if (nextArg == "-n") {
                options.m_captureOptions->m_sequenceName = argv[i + 1];
                i += 2;
            } else if (nextArg == "-c") {
                options.m_captureOptions->m_copyright = argv[i + 1];
                i += 2;
            } else {
                return babelwires::Error() << "Unexpected arguments provided to capture mode";
            }
        }
        if (i == argc) {
            return babelwires::Error() << "Capture mode requires a filename argument";
        }
        options.m_captureOptions->m_outputFileName = argv[argc - 1];
    } else if (modeArg == s_playString) {
        options.m_mode = MODE_PLAYBACK;
        options.m_playbackOptions = std::make_unique<PlaybackOptions>();
        options.m_playbackOptions->m_outputPlaybackDest = "default";
        int i = 2;
        while (i < argc - 1) {
            std::string nextArg = argv[i];
            if (nextArg == "-d") {
                options.m_playbackOptions->m_outputPlaybackDest = argv[i + 1];
                i += 2;
            } else {
                return babelwires::Error() << "Unexpected arguments to playback mode";
            }
        }
        if (i == argc) {
            return babelwires::Error() << "Playback mode requires a filename argument";
        }
        options.m_playbackOptions->m_inputFileName = argv[argc - 1];
    } else if (modeArg == s_convertString) {
        options.m_mode = MODE_CONVERT;
        options.m_convertOptions = std::make_unique<ConvertOptions>();
        int i = 2;
        while (i < argc - 2) {
            std::string nextArg = argv[i];
            if (nextArg == "-n") {
                options.m_convertOptions->m_sequenceName = argv[i + 1];
                i += 2;
            } else if (nextArg == "-c") {
                options.m_convertOptions->m_copyright = argv[i + 1];
                i += 2;
            } else {
                return babelwires::Error() << "Unexpected arguments provided to convert mode";
            }
        }
        if (i == argc - 1) {
            return babelwires::Error() << "Convert mode expects two filename arguments";
        }
        options.m_convertOptions->m_inputFileName = argv[i];
        options.m_convertOptions->m_outputFileName = argv[i + 1];
    } else if (modeArg == s_formatsString) {
        options.m_mode = MODE_LIST_FORMATS;
    } else if (modeArg == s_audioString) {
        options.m_mode = MODE_LIST_AUDIO;
    } else {
        return babelwires::Error() << "Unrecognized mode \"" << modeArg << "\" provided";
    }
    return options;
}

void writeUsage(const std::string& programName, bool playbackAvailable, bool captureAvailable, std::ostream& stream) {
    stream << "Usage:" << std::endl;
    stream << programName << " " << s_convertString << " [-n name] [-c copyright] <input file> <output file>"
           << std::endl;
    if (playbackAvailable) {
        stream << programName << " " << s_playString << " [-d <audio destination>] <input file>" << std::endl;
    }
    if (captureAvailable) {
        stream << programName << " " << s_captureString
               << " [-s <audio source>] [-f <num data files>] [-n name] [-c copyright] <output file>" << std::endl;
    }
    stream << programName << " " << s_formatsString << " " << std::endl;
    if (playbackAvailable || captureAvailable) {
        stream << programName << " " << s_audioString << " " << std::endl;
    }
    stream << programName << " " << s_helpString << " " << std::endl;
}

void writeHelp(const std::string& programName, bool playbackAvailable, bool captureAvailable, std::ostream& stream) {
    stream << programName
           << " - Converts music sequencer data between file formats suitable for computer storage and audio data "
              "format suitable for cassette tapes."
           << std::endl;
    if (!captureAvailable) {
        stream << "Warning: No source audio interfaces were registered, so audio capture is unavailable.";
    }
    if (!playbackAvailable) {
        stream << "Warning: No destination audio interface were registered, so audio playback is unavailable.";
    }
    writeUsage(programName, playbackAvailable, captureAvailable, stream);
}
