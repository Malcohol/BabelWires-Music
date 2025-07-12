/**
 * TrackType is the type of Tracks.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/type.hpp>

#include <MusicLib/musicTypes.hpp>

namespace bw_music {
    /// TrackType is the type of Tracks.
    class TrackType : public babelwires::Type {
      public:
        TrackType(ModelDuration defaultDuration = 0);

        babelwires::NewValueHolder createValue(const babelwires::TypeSystem& typeSystem) const override;

        bool isValidValue(const babelwires::TypeSystem& typeSystem, const babelwires::Value& v) const override;

        std::string getFlavour() const override;

        std::optional<babelwires::SubtypeOrder> compareSubtypeHelper(const babelwires::TypeSystem& typeSystem, const babelwires::Type& other) const override;

        std::string valueToString(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& v) const override;

      private:
        ModelDuration m_defaultDuration;
    };

    /// The standard TrackType which creates empty Tracks with 0 duration.
    class DefaultTrackType : public TrackType {
      public:
        REGISTERED_TYPE("track", "Track", "346ec14c-25dd-43fc-a942-d24722be6802", 1);
    };
} // namespace bw_music
