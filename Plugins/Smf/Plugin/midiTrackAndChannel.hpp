/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/instanceWithChannelOptionals.hpp>
#include <Plugins/Smf/Plugin/smfCommon.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <Plugins/Smf/Plugin/midiChannel.hpp>

namespace smf {

    /// The tracks of format 1 files usually carry events for one channel. However:
    /// (a) more than one track can have data for the same channel.
    /// (b) a track can carry events for multiple channels.
    /// See discussion here: https://www.midi.org/forum/8793-smf-format-1-tracks-and-channels
    /// Because of (a), I chose track number rather than channel number to give identity to the
    /// TrackFeatures. That isn't a complete solution because of (b). However, since (b) is
    /// very rare, I believe the following to be an acceptable compromise:
    /// We pick the channel in each track which has the most events, and give it an identity
    /// which just depends on the track number. This should be pretty stable.
    /// All other channels in the track are given an identity which depends on the track number
    /// and channel. This is less stable, but OK. Problems will arise in the very rare situation
    /// where a track had a similar number of events for two channels, and on reload, the
    /// the smaller became the larger.
    class MidiTrackAndChannel : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("TrackChannel", "Track and Channel", "5e9b395c-ec13-4bdb-9b2b-b060ba7fb707", 1);
        MidiTrackAndChannel();

        static babelwires::ShortId getTrackIdFromChannel(unsigned int channel);

        /// It's easier to work with a custom instance than the one the standard DSL would produce.
        /// The parent class provides index based access to the 16 optional tracks.
        template <typename VALUE_TREE_NODE>
        class InstanceImpl : public InstanceWithChannelOptionals<VALUE_TREE_NODE, MidiTrackAndChannel> {
          public:
            InstanceImpl(VALUE_TREE_NODE& valueFeature)
                : InstanceWithChannelOptionals<VALUE_TREE_NODE, MidiTrackAndChannel>(valueFeature) {}

            DECLARE_INSTANCE_FIELD(Chan, MidiChannel)
            DECLARE_INSTANCE_FIELD(Track, bw_music::TrackType);
        };
        using Instance = InstanceImpl<babelwires::ValueTreeNode>;
        using ConstInstance = InstanceImpl<const babelwires::ValueTreeNode>;
    };
} // namespace smf
