#include <gtest/gtest.h>

#include <Seq2tapeLib/Audio/audioSource.hpp>
#include <Seq2tapeLib/Audio/FileAudio/fileAudioDest.hpp>

#include <Seq2tapeLib/waveReader.hpp>

#include <algorithm>
#include <filesystem>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath>

#define DEBUG_WRITE_WAVES_TO_FILES 0

#if DEBUG_WRITE_WAVES_TO_FILES
#define DEBUG_WRITE_TO_FILE(SCENARIO)                                                                                  \
    writeSourceToTempFile(::testing::UnitTest::GetInstance()->current_test_info()->name(), SCENARIO);
#else
#define DEBUG_WRITE_TO_FILE(SCENARIO)
#endif

namespace {
    constexpr babelwires::Duration c_pi = 3.14159265358979323846;

#if DEBUG_WRITE_WAVES_TO_FILES
    constexpr babelwires::Duration c_defaultFreq = 44100.0;
#else
    constexpr babelwires::Duration c_defaultFreq = 22050.0;
#endif

    /// Return the time period since the last "tick" at the given tick count.
    using ClockFunc = std::function<babelwires::Duration(int)>;

    /// Return the sample given a proportion throught the wave between 0 and 1.
    using WaveShapeFunc = std::function<babelwires::AudioSample(babelwires::Duration proportion)>;

    /// Get a bias value at an absolute time.
    using BiasFunc = std::function<babelwires::AudioSample(babelwires::Duration)>;

    /// Get the volume at an absolute time. This is a multiplier which scales the samples.
    using VolumeFunc = std::function<babelwires::AudioSample(babelwires::Duration)>;

    /// Get the noise at an absolute time. This is added to the smaple.
    /// (It's used exactly like bias, but would be expected to have a frequency higher than the
    /// waveforms, rather than lower.)
    using NoiseFunc = std::function<babelwires::AudioSample(babelwires::Duration)>;

    struct TestScenario {
        seq2tape::WaveReader::Polarity m_polarity = seq2tape::WaveReader::Polarity::negativeThenPositive;

        std::vector<babelwires::Duration> m_waveLengths = {0.00045, 0.0009};

        babelwires::Duration m_frequency = c_defaultFreq;

        ClockFunc m_clockFunc = [](int) { return 1 / c_defaultFreq; };

        WaveShapeFunc m_waveShapeFunc = [](babelwires::Duration p) {
            return static_cast<babelwires::AudioSample>(sin(p * 2.0 * c_pi));
        };

        BiasFunc m_biasFunc = [](babelwires::Duration) { return 0.0f; };

        VolumeFunc m_volumeFunc = [](babelwires::Duration) { return 1.0f; };

        NoiseFunc m_noiseFunc = [](babelwires::Duration) { return 0.0f; };

        int m_numBlocks = 3;

        unsigned int m_randomSeed = 0x24eb87ae;
    };

    struct TestWaveSequence {
        TestWaveSequence(const TestScenario& scenario)
            : m_generator(scenario.m_randomSeed)
            , m_waveTypesDist(0, static_cast<unsigned int>(scenario.m_waveLengths.size() - 1)) {
            std::uniform_int_distribution<unsigned int> blockLengthDist(10, 1000);
            for (int i = 0; i < scenario.m_numBlocks; ++i) {
                m_blockLengths.emplace_back(blockLengthDist(m_generator));
            }
        }

        enum SpecialWaveTypes {
            WAVE_TYPE_EOF = -2,
            // A pilot signal of waveType 0.
            PILOT_SEQUENCE = -3,
            SILENCE = -4,
            // We assume that some wave follows a block to ensure an unambiguous reading of the final block wave.
            POST_BLOCK_WAVE = -5
        };

        int getNextWaveType() {
            ++m_indexInBlock;
            if (m_indexInBlock == 0) {
                return POST_BLOCK_WAVE;
            } else if (m_indexInBlock == 1) {
                return SILENCE;
            }
            else if (m_blockIndex == m_blockLengths.size()) {
                return WAVE_TYPE_EOF;
            }
            else if (m_indexInBlock == 2) {
                return PILOT_SEQUENCE;
            }
            else if (m_indexInBlock == 3) {
                // Since pilot signals are expect to use wave type 0, the wave which follows cannot be
                // type 0.
                int nonZeroType = 0;
                while (nonZeroType == 0) {
                    nonZeroType = m_waveTypesDist(m_generator);
                }
                return nonZeroType;
            }
            if (m_indexInBlock == m_blockLengths[m_blockIndex]) {
                m_indexInBlock = -1;
                ++m_blockIndex;
            }
            return m_waveTypesDist(m_generator);
        }

        std::vector<int> m_blockLengths;
        int m_blockIndex = 0;
        int m_indexInBlock = -1;
        std::default_random_engine m_generator;
        std::uniform_int_distribution<unsigned int> m_waveTypesDist;
    };

    struct SourceSampleProvider {
        SourceSampleProvider(const TestScenario& scenario)
            : m_testSequence(scenario)
            , m_scenario(scenario) {}

        enum SampleResult { SAMPLE_EOF, SAMPLE_OK };

        SampleResult getNextSample(babelwires::AudioSample& sample) {
            ++m_sampleCount;
            const babelwires::Duration samplePeriod = m_scenario.m_clockFunc(m_sampleCount);
            m_amountThroughWave += samplePeriod;

            assert(m_state != AT_END);
            if (m_state == IN_SILENCE) {
                if (m_amountThroughWave > m_silenceLength) {
                    m_amountThroughWave -= m_silenceLength;
                    nextState();
                }
            } else if (m_state == IN_BLOCK_WAVE) {
                if (m_amountThroughWave > m_scenario.m_waveLengths[m_waveType]) {
                    m_amountThroughWave -= m_scenario.m_waveLengths[m_waveType];
                    if (m_waveRepeat == 0) {
                        nextState();
                    } else {
                        --m_waveRepeat;
                    }
                }
            }

            if (m_state == AT_END) {
                return SAMPLE_EOF;
            } else {
                const babelwires::Duration proportionThroughWave =
                    m_amountThroughWave / m_scenario.m_waveLengths[m_waveType];
                const babelwires::AudioSample baseSample =
                    (m_state == IN_SILENCE) ? 0.0f : m_scenario.m_waveShapeFunc(proportionThroughWave);
                const babelwires::Duration absoluteTime = m_sampleCount * (1.0 / m_scenario.m_frequency);
                const babelwires::AudioSample bias = m_scenario.m_biasFunc(absoluteTime);
                const babelwires::AudioSample noise = m_scenario.m_noiseFunc(absoluteTime);
                const babelwires::AudioSample volume = m_scenario.m_volumeFunc(absoluteTime);
                const babelwires::AudioSample unclampedValue = (volume * baseSample) + bias + noise;
                sample = std::clamp(unclampedValue, -1.0f, 1.0f);
                if (m_scenario.m_polarity == seq2tape::WaveReader::Polarity::negativeThenPositive) {
                    sample *= -1.0f;
                }
                /*
                if (m_state == IN_SILENCE)
                {
                    sample = 0.0;
                }
                */
            }

            return SAMPLE_OK;
        }

        void nextState() {
            m_waveType = m_testSequence.getNextWaveType();
            switch (m_waveType) {
                case TestWaveSequence::WAVE_TYPE_EOF:
                    m_state = AT_END;
                    break;
                case TestWaveSequence::PILOT_SEQUENCE:
                    m_state = IN_BLOCK_WAVE;
                    m_waveRepeat = 1000;
                    m_waveType = 0;
                    break;
                case TestWaveSequence::POST_BLOCK_WAVE:
                    m_state = IN_BLOCK_WAVE;
                    m_waveRepeat = 0;
                    m_waveType = 1;
                    break;
                case TestWaveSequence::SILENCE:
                    m_state = IN_SILENCE;
                    m_waveType = 0;
                    break;
                default:
                    m_state = IN_BLOCK_WAVE;
                    break;
            }
        }

        TestWaveSequence m_testSequence;
        const TestScenario& m_scenario;

        int m_sampleCount = 0;

        enum State { IN_SILENCE, IN_PILOT, IN_BLOCK_WAVE, AT_END } m_state = IN_SILENCE;

        int m_waveRepeat = 0;
        babelwires::Duration m_silenceLength = 0.3f;
        int m_waveType = 0;
        babelwires::Duration m_amountThroughWave = 0.0;
    };

    /// Adapts a SourceSampleProvider to an audio source.
    struct TestAudioSource : babelwires::AudioSource {
        TestAudioSource(const TestScenario& testScenario)
            : m_sourceSampleProvider(testScenario) {}

        // TODO Multichannel
        int getNumChannels() const override { return 1; }

        babelwires::Duration getFrequency() const override { return m_sourceSampleProvider.m_scenario.m_frequency; }

        unsigned long getMoreAudioData(babelwires::AudioSample* buffer, unsigned long bufSize) override {
            if (m_atEnd) {
                return 0;
            }
            for (unsigned long i = 0; i < bufSize; ++i) {
                babelwires::AudioSample sample = 0.0f;
                if (m_sourceSampleProvider.getNextSample(sample) == SourceSampleProvider::SAMPLE_EOF) {
                    m_atEnd = true;
                    return i;
                }
                buffer[i] = sample;
            }
            return bufSize;
        }

        SourceSampleProvider m_sourceSampleProvider;
        bool m_atEnd = false;
    };

    void writeSourceToTempFile(std::string testName, const TestScenario& scenario) {
        babelwires::FileAudioDestRegistry reg;
        std::filesystem::path outputFileName;
        outputFileName = std::filesystem::temp_directory_path() / (testName + ".wav");
        std::cerr << "Test wavefile written to " << outputFileName << std::endl;
        const auto audioDest = reg.createFileAudioDest(babelwires::pathToString(outputFileName).c_str(), 1);
        ASSERT_TRUE(audioDest);
        const babelwires::Duration targetFrequency = (*audioDest)->getFrequency();
        // If the frequencies did not agree, we'd have to resample, which would mean the output files did not accurately reflect the data being tested.
        ASSERT_EQ(targetFrequency, c_defaultFreq) << "Can only write test files with available frequency";
        constexpr std::size_t bufferSize = 2048;
        babelwires::AudioSample buffer[bufferSize];
        TestAudioSource audioSource(scenario);
        unsigned long samplesRead = 0;
        unsigned long samplesWritten = 0;
        do {
            samplesRead = audioSource.getMoreAudioData(buffer, bufferSize);
            samplesWritten = (*audioDest)->writeMoreAudioData(buffer, samplesRead);
            ASSERT_EQ(samplesWritten, samplesRead);
        } while (samplesRead == bufferSize);
    }

    /// Confirm that the sequence described by the scenario can be read from the audio source.
    void testSequenceReadSuccessfully(const TestScenario& scenario) {
        TestAudioSource source(scenario);

        seq2tape::WaveReader waveReader(source, scenario.m_polarity, scenario.m_waveLengths);

        TestWaveSequence expectedSequence(scenario);

        int expectedWaveType;
        int waveCount = 0;
        do {
            expectedWaveType = expectedSequence.getNextWaveType();

            if (expectedWaveType == TestWaveSequence::SILENCE) {
                continue;
            } else if (expectedWaveType == TestWaveSequence::POST_BLOCK_WAVE) {
                continue;
            } else if (expectedWaveType == TestWaveSequence::PILOT_SEQUENCE) {
                const int postPilotWaveRead = waveReader.seekPilot(0, 100);
                const int postPilotWaveExpected = expectedSequence.getNextWaveType();
                ASSERT_EQ(postPilotWaveRead, postPilotWaveExpected);
            } else if (expectedWaveType != TestWaveSequence::WAVE_TYPE_EOF) {
                // Assume every 10th wave is of an agreed type. We can then tell the reader to expect it,
                // allowing the reader to adapt to varying speed.
                const int agreedWave = (++waveCount % 10) ? seq2tape::WaveReader::WAVE_TYPE_UNKNOWN : expectedWaveType;
                const int waveRead = waveReader.getNextWave(agreedWave);
                ASSERT_EQ(waveRead, expectedWaveType);
            }
        } while (expectedWaveType != TestWaveSequence::WAVE_TYPE_EOF);

        // Once we've read all the expected waves, we don't care what else the sequence does.
    }
} // namespace

TEST(WaveReader, defaultScenario) {
    TestScenario scenario;
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, threeWaveLengths) {
    TestScenario scenario;
    scenario.m_waveLengths = {0.003, 0.006, 0.009};
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, polarity) {
    TestScenario scenario;
    scenario.m_polarity = seq2tape::WaveReader::Polarity::positiveThenNegative;
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, squareWave) {
    TestScenario scenario;
    scenario.m_waveShapeFunc = [](babelwires::Duration p) { return p < 0.5 ? 1.0f : -1.0f; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, triangleWave) {
    TestScenario scenario;
    scenario.m_waveShapeFunc = [](babelwires::Duration p) -> babelwires::AudioSample {
        if (p < 0.25) {
            return static_cast<babelwires::AudioSample>(4.0f * p);
        } else if (p < 0.75) {
            return static_cast<babelwires::AudioSample>(1.0f - 4.0f * (p - 0.25));
        } else {
            return static_cast<babelwires::AudioSample>(-1.0f + 4.0f * (p - 0.75));
        }
    };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, slow) {
    TestScenario scenario;
    scenario.m_clockFunc = [&scenario](int) { return 0.75 / scenario.m_frequency; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, fast) {
    TestScenario scenario;
    scenario.m_clockFunc = [&scenario](int) { return 1.25 / scenario.m_frequency; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, varyingSpeed) {
    TestScenario scenario;
    scenario.m_clockFunc = [&scenario](int t) {
        // A cycle of 0.15 seconds.
        const double theta = c_pi * (static_cast<double>(t) / scenario.m_frequency) * (1 / 0.15);
        // Vary speed by +/- 25%.
        const babelwires::Duration s = 1.0 + (0.25 * sin(theta));
        return s / c_defaultFreq;
    };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, globalBias) {
    TestScenario scenario;
    scenario.m_biasFunc = [](babelwires::Duration) { return 0.2f; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, varyingBias) {
    TestScenario scenario;
    scenario.m_biasFunc = [](babelwires::Duration d) {
        // A cycle of 0.05 seconds.
        const double theta = c_pi * (d / 0.1f);
        // Vary bias by +/- 0.25.
        const babelwires::AudioSample b = static_cast<babelwires::AudioSample>(0.25f * sin(theta));
        return b;
    };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, lowVolume) {
    TestScenario scenario;
    scenario.m_volumeFunc = [](babelwires::Duration d) { return 0.3f; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, biasAndLowVolume) {
    TestScenario scenario;
    scenario.m_volumeFunc = [](babelwires::Duration d) { return 0.3f; };
    scenario.m_biasFunc = [](babelwires::Duration) { return 0.3f; };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, varyingBiasAndLowVolume) {
    TestScenario scenario;
    scenario.m_volumeFunc = [](babelwires::Duration d) { return 0.25f; };
    scenario.m_biasFunc = [](babelwires::Duration d) {
        // A cycle of 0.05 seconds.
        const double theta = c_pi * (d / 0.1f);
        // Vary bias by +/- 0.3.
        const babelwires::AudioSample b = static_cast<babelwires::AudioSample>(0.25f * sin(theta));
        return b;
    };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, noise) {
    TestScenario scenario;
    std::default_random_engine generator(12345);
    std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
    scenario.m_noiseFunc = [&generator, &dist](babelwires::Duration d) { return dist(generator); };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

// Test a combination of features, although not as extreme as they are tested individually.
TEST(WaveReader, combination) {
    TestScenario scenario;
    // Triangle
    scenario.m_waveShapeFunc = [](babelwires::Duration p) -> babelwires::AudioSample {
        if (p < 0.25) {
            return static_cast<babelwires::AudioSample>(4.0f * p);
        } else if (p < 0.75) {
            return static_cast<babelwires::AudioSample>(1.0f - 4.0f * (p - 0.25));
        } else {
            return static_cast<babelwires::AudioSample>(-1.0f + 4.0f * (p - 0.75));
        }
    };
    // Varying speed.
    scenario.m_clockFunc = [&scenario](int t) {
        // A cycle of 0.15 seconds.
        const double theta = c_pi * (static_cast<double>(t) / scenario.m_frequency) * (1 / 0.15);
        // Vary speed by +/- 15%.
        const babelwires::Duration s = 1.0 + (0.15 * sin(theta));
        return s / c_defaultFreq;
    };
    // Varying bias.
    scenario.m_biasFunc = [](babelwires::Duration d) {
        // A cycle of 0.05 seconds.
        const double theta = c_pi * (d / 0.05f);
        // Vary bias by +/- 0.15.
        const babelwires::AudioSample b = static_cast<babelwires::AudioSample>(0.15f * sin(theta));
        return b;
    };
    // Non-max volume
    scenario.m_volumeFunc = [](babelwires::Duration d) { return 0.8f; };
    // Noise
    std::default_random_engine generator(56789);
    std::uniform_real_distribution<float> dist(-0.025f, 0.025f);
    scenario.m_noiseFunc = [&generator, &dist](babelwires::Duration d) { return dist(generator); };
    DEBUG_WRITE_TO_FILE(scenario);
    testSequenceReadSuccessfully(scenario);
}

TEST(WaveReader, doNotCheckInWithFileWritingEnabled) {
    // This test exists to ensure that the debug macro is not accidently enabled.
    ASSERT_EQ(DEBUG_WRITE_WAVES_TO_FILES, 0);
}
