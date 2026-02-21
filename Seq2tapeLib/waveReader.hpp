/**
 * A WaveReader tries to read waves of specific lengths from an audio file.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Seq2tapeLib/sampleReader.hpp>

namespace seq2tape {
    /// Try to read waves of specific lengths from an audio file.
    /// A wave is a negative then positive pulse.
    /// Tries to adapt to variable speed.
    class WaveReader {
      public:
        /// We assume that a "wave" is pair of a positive and a negative pulse.
        /// This determines which is expected first.
        enum class Polarity { positiveThenNegative, negativeThenPositive };

        /// Construct a WaveReader which reads waves from an audioSource.
        /// The waveLengths are the expected durations of the wave.
        /// The reader will attempt to adapt the absolute durations while
        /// respecting the relative durations.
        WaveReader(babelwires::AudioSource& audioSource, Polarity polarity,
                   const std::vector<babelwires::Duration>& waveLengths);

        enum WaveType { WAVE_TYPE_UNKNOWN = -1, WAVE_TYPE_EOF = -2 };

        /// Get the index of the wave in the waveProportion vector passed to the constructor, or -1 if the
        /// next wave wasn't close enough to one of those waves.
        /// If a particular wave type is known to be expected, then we can adjust the
        /// referenceDuration to account for changes in tape speed.
        int getNextWave(int expectedWave = WAVE_TYPE_UNKNOWN);

        /// Returns the position of the last sample read in seconds.
        babelwires::Duration getPositionInStream() const;

        /// Try to read at least the minimum number of waves of the given waveType, and return the waveType of the wave
        /// that breaks the sequence. While reading, try to adapt to the speed of the recording by adjusting the
        /// m_referenceDuration.
        int seekPilot(int waveType, int minimumNumWaves);

      protected:
        /// Return the waveType which the duration matches if it matches one closely enough.
        int getWaveType(babelwires::Duration waveDuration) const;

        /// (Negative, Positive)
        using PulseDurations = std::tuple<babelwires::Duration, babelwires::Duration>;

        /// Return the durations of the two pulses (negative, positive) that make up the next wave,
        PulseDurations getNextWaveDuration();

        /// Get the next sample, accounting for bias and polarity.
        SampleReader::SampleResult getNextSample(babelwires::AudioSample& sample);

        /// Adjust the referenceDuration so it better matches the given waveType at the given duration.
        void updateReferenceDuration(int waveType, babelwires::Duration waveDuration);

        /// When a wave is successfully read, use the ratio of negative to positive pulse durations to
        /// influence the estimate of the bias.
        void updateBiasEstimate(babelwires::Duration negativeDuration, babelwires::Duration positiveDuration);

      private:
        SampleReader m_sampleReader;
        const babelwires::Duration m_samplePeriod;
        babelwires::Duration m_referenceDuration;
        /// Used to judge when a pulse is over, even if it didn't reach the switching threshold.
        babelwires::Duration m_postReturnTimeOut;
        /// positiveNegative = -1.0, negativePositive = 1.0
        babelwires::AudioSample m_polarityMultiplier;

        /// Proportions of the reference durations are not actually durations, but it's convenient to use the same type.
        using Proportion = babelwires::Duration;

        struct PerWaveInfo {
            Proportion m_proportionOfReferenceDuration;

            Proportion m_lowerBoundProportion;
            Proportion m_upperBoundProportion;

            int m_originalIndex;
        };

        std::vector<PerWaveInfo> m_perWaveInfo;
        std::vector<int> m_originalToSortedIndex;

        /// getNextWave read this much into the beginning of the next negative pulse.
        babelwires::Duration m_negativeDurationFromPreviousStep;

        /// The mean of all samples read so far. Used to remove extreme constant bias.
        babelwires::AudioSample m_signalMean = 0.0f;

        /// An estimate of the bias (after other factors have been accounted for).
        /// This tries to handle slowly varying bias.
        babelwires::AudioSample m_biasEstimate = 0.0f;
    };

} // namespace seq2tape
