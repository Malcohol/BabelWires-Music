#pragma once

#include <Domains/Music/MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Int/intType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music_testplugin {
    /// A type that contains some tracks and other fields for testing.
    class TestTrackContainer : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("TrackContainer", "Test Track Container", "2bfd356b-7974-4769-8a64-ef934c44a5a0", 1);
        TestTrackContainer();

        DECLARE_INSTANCE_BEGIN(TestTrackContainer)
        DECLARE_INSTANCE_FIELD(track1, bw_music::TrackType)
        DECLARE_INSTANCE_FIELD(track2, bw_music::TrackType)
        DECLARE_INSTANCE_FIELD(other, babelwires::IntType)
        DECLARE_INSTANCE_END()

        static babelwires::ShortId getTrack1Id();
        static babelwires::ShortId getTrack2Id();
        static babelwires::ShortId getOtherId();
    };
} // namespace bw_music_testplugin
