/**
 * PercussionEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::PercussionEvent::PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                                                  double velocity)
    : TrackEvent(timeSinceLastEvent)
    , m_instrument(instrument)
    , m_velocity(VelocityStorage::assertFromNormalizedDouble(velocity)) {}

inline bw_music::PercussionEvent::PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                                                  VelocityStorage velocity)
    : TrackEvent(timeSinceLastEvent)
    , m_instrument(instrument)
    , m_velocity(velocity) {}

inline void bw_music::PercussionEvent::setInstrument(babelwires::ShortId instrument) { m_instrument = instrument; }

inline babelwires::ShortId bw_music::PercussionEvent::getInstrument() const { return m_instrument; }

inline void bw_music::PercussionEvent::setVelocity(double velocity) {
    m_velocity = VelocityStorage::assertFromNormalizedDouble(velocity);
}

inline double bw_music::PercussionEvent::getVelocity() const { return m_velocity.getNormalizedDouble(); }

inline void bw_music::PercussionEvent::setVelocityStorage(VelocityStorage velocity) { m_velocity = velocity; }

inline bw_music::VelocityStorage bw_music::PercussionEvent::getVelocityStorage() const { return m_velocity; }

inline bw_music::PercussionOnEvent::PercussionOnEvent(ModelDuration timeSinceLastEvent,
                                                      babelwires::ShortId instrument, double velocity)
    : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

inline bw_music::PercussionOnEvent::PercussionOnEvent(ModelDuration timeSinceLastEvent,
                                                      babelwires::ShortId instrument, VelocityStorage velocity)
    : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

inline bw_music::PercussionOffEvent::PercussionOffEvent(ModelDuration timeSinceLastEvent,
                                                        babelwires::ShortId instrument, double velocity)
    : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

inline bw_music::PercussionOffEvent::PercussionOffEvent(ModelDuration timeSinceLastEvent,
                                                        babelwires::ShortId instrument, VelocityStorage velocity)
    : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}