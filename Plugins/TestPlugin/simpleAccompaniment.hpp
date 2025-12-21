#pragma once

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>

namespace bw_music_testplugin {
    /// A type that contains some tracks and other fields for testing.
    class SimpleAccompaniment : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("SimpleAccomp", "Simple Accompaniment", "9a92a3e0-9613-46be-ad77-898896875ad4", 1);
        SimpleAccompaniment();

        babelwires::NewValueHolder createValue(const babelwires::TypeSystem& typeSystem) const override;

        static bw_music::Track getCMajorArpeggioTrack();

        DECLARE_INSTANCE_BEGIN(SimpleAccompaniment)
        DECLARE_INSTANCE_FIELD(M, TestTrackContainer)
        DECLARE_INSTANCE_FIELD(m, TestTrackContainer)
        DECLARE_INSTANCE_FIELD(dim, TestTrackContainer)
        DECLARE_INSTANCE_END()
    };
} // namespace bw_music_testplugin
