/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::ExpressionTrackEvent::ExpressionTrackEvent(ModelDuration timeSinceLastEvent,
                                                            bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::ExpressionTrackEvent::ExpressionTrackEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : ExpressionTrackEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::ExpressionTrackEvent::getExpressionAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::ExpressionTrackEvent::getExpressionStorage() const { return m_value; }