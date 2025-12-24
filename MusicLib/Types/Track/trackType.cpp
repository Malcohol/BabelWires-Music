/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/trackType.hpp>

#include <MusicLib/Types/Track/track.hpp>

bw_music::TrackType::TrackType(ModelDuration defaultDuration)
    : m_defaultDuration(defaultDuration) {}

babelwires::NewValueHolder bw_music::TrackType::createValue(const babelwires::TypeSystem& typeSystem) const {
    return babelwires::ValueHolder::makeValue<Track>(m_defaultDuration);
}

bool bw_music::TrackType::visitValue(const babelwires::TypeSystem& typeSystem, const babelwires::Value& v, ChildValueVisitor& visitor) const {
    return v.as<Track>();
}

std::string bw_music::TrackType::getFlavour() const {
    return "track";
}

std::optional<babelwires::SubtypeOrder>
bw_music::TrackType::compareSubtypeHelper(const babelwires::TypeSystem& typeSystem,
                                                     const babelwires::Type& other) const {
    const babelwires::Type* const otherTrackType = other.as<TrackType>();
    if (!otherTrackType) {
        return {};
    }
    return babelwires::SubtypeOrder::IsEquivalent;
}

std::string bw_music::TrackType::valueToString(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& v) const { 
    const Track& track = v->is<Track>();
    return track.getDuration().toString();
}
