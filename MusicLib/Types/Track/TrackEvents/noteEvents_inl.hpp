/**
 * NoteEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::NoteEvent::NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch)
    : TrackEvent(timeSinceLastEvent)
    , m_pitch(pitch) {}

inline void bw_music::NoteEvent::setPitch(Pitch pitch) { m_pitch = pitch; }

inline bw_music::Pitch bw_music::NoteEvent::getPitch() const { return m_pitch; }

inline void bw_music::NoteEventWithVelocity::setVelocityFromNormalizedValue(double velocity) {
    m_velocity = VelocityStorage::assertFromNormalizedDouble(velocity);
}

inline double bw_music::NoteEventWithVelocity::getVelocityAsNormalizedValue() const { return m_velocity.getNormalizedDouble(); }

inline void bw_music::NoteEventWithVelocity::setVelocityStorage(VelocityStorage velocity) { m_velocity = velocity; }

inline bw_music::VelocityStorage bw_music::NoteEventWithVelocity::getVelocityStorage() const { return m_velocity; }

inline bw_music::NoteEventWithVelocity::NoteEventWithVelocity(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
    : NoteEvent(timeSinceLastEvent, pitch)
    , m_velocity(VelocityStorage::assertFromNormalizedDouble(velocity)) {}

inline bw_music::NoteEventWithVelocity::NoteEventWithVelocity(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity)
    : NoteEvent(timeSinceLastEvent, pitch)
    , m_velocity(velocity) {}

inline bw_music::NoteOnEvent::NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
    : NoteEventWithVelocity(timeSinceLastEvent, pitch, velocity) {}

inline bw_music::NoteOnEvent::NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity)
    : NoteEventWithVelocity(timeSinceLastEvent, pitch, velocity) {}

inline bw_music::NoteOffEvent::NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
    : NoteEventWithVelocity(timeSinceLastEvent, pitch, velocity) {}

inline bw_music::NoteOffEvent::NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity)
    : NoteEventWithVelocity(timeSinceLastEvent, pitch, velocity) {}