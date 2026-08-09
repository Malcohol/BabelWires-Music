/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::ExpressionEvent::ExpressionEvent(ModelDuration timeSinceLastEvent,
                                                            bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::ExpressionEvent::ExpressionEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : ExpressionEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::ExpressionEvent::getExpressionAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::ExpressionEvent::getExpressionStorage() const { return m_value; }