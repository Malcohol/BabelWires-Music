/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/instanceWithChannelOptionals.hpp>
#include <Plugins/Smf/Plugin/smfCommon.hpp>

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace smf {

    class RecordOfMidiTracks : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("recordOfTracks", "Tracks", "44244cf8-8a05-4071-91a3-af77514ee03b", 1);

        RecordOfMidiTracks(const babelwires::TypeSystem& typeSystem);

        static babelwires::ShortId getTrackIdFromChannel(unsigned int channel);

        /// It's easier to work with a custom instance than the one the standard DSL would produce.
        /// The parent class provides index based access to the 16 optional tracks.
        template <typename VALUE_TREE_NODE>
        class InstanceImpl : public InstanceWithChannelOptionals<VALUE_TREE_NODE, RecordOfMidiTracks> {
          public:
            InstanceImpl(VALUE_TREE_NODE& valueFeature)
                : InstanceWithChannelOptionals<VALUE_TREE_NODE, RecordOfMidiTracks>(valueFeature) {}
        };
        using Instance = InstanceImpl<babelwires::ValueTreeNode>;
        using ConstInstance = InstanceImpl<const babelwires::ValueTreeNode>;
    };
} // namespace smf
