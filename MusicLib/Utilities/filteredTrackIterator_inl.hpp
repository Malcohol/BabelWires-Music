/**
 * A FilteredTrackIterator provides a way of iterate over a track, skipping events that are not of interest.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

/// The iterator must be initialized after construction by called initBegin or initEnd.
template <typename EVENT>
bw_music::FilteredTrackIterator<EVENT>::FilteredTrackIterator(const Track& track)
    : m_iterator(track.begin())
    , m_end(track.end()) {}

/// Set the iterator to the beginning of the track.
/// Note: Subclasses may have an overridden isEventOfInterest and the subclass' v-table and invariants will
/// not be fully set up when the FilteredTrackIterator constructor is called.
template <typename EVENT> void bw_music::FilteredTrackIterator<EVENT>::initBegin() {
    m_isInitialized = true;
    advance();
}

/// Set the iterator to the end of the track.
template <typename EVENT> void bw_music::FilteredTrackIterator<EVENT>::initEnd() {
    m_isInitialized = true;
    m_iterator = m_end;
}

template <typename EVENT> bw_music::FilteredTrackIterator<EVENT>& bw_music::FilteredTrackIterator<EVENT>::operator++() {
    assert(m_isInitialized && "The iterator is not initialized. Call initBegin or initEnd.");
    ++m_iterator;
    advance();
    return *this;
}

template <typename EVENT> const EVENT& bw_music::FilteredTrackIterator<EVENT>::operator*() const {
    assert(m_isInitialized && "The iterator is not initialized. Call initBegin or initEnd.");
    assert(m_iterator->tryAs<EVENT>() && "m_iterator should never stop on an event of the wrong type.");
    return m_event.hasEvent() ? *m_event : static_cast<const EVENT&>(*m_iterator);
}

template <typename EVENT> const EVENT* bw_music::FilteredTrackIterator<EVENT>::operator->() const {
    assert(m_isInitialized && "The iterator is not initialized. Call initBegin or initEnd.");
    assert(m_iterator->tryAs<EVENT>() && "m_iterator should never stop on an event of the wrong type.");
    return m_event.hasEvent() ? &*m_event : static_cast<const EVENT*>(&*m_iterator);
}

template <typename EVENT>
bool bw_music::FilteredTrackIterator<EVENT>::operator==(const FilteredTrackIterator& other) const {
    assert(m_isInitialized && "The iterator is not initialized. Call initBegin or initEnd.");
    assert(m_end == other.m_end);
    return m_iterator == other.m_iterator;
}

template <typename EVENT>
bool bw_music::FilteredTrackIterator<EVENT>::operator!=(const FilteredTrackIterator& other) const {
    return !(*this == other);
}

template <typename EVENT> void bw_music::FilteredTrackIterator<EVENT>::advance() {
    m_event.reset();
    ModelDuration timeSinceLastEvent = 0;
    while (m_iterator != m_end) {
        timeSinceLastEvent += m_iterator->getTimeSinceLastEvent();
        if (isEventOfInterest(*m_iterator)) {
            // Only copy the event into the StreamEventHolder if necessary.
            if (timeSinceLastEvent != m_iterator->getTimeSinceLastEvent()) {
                assert(m_iterator->tryAs<EVENT>() &&
                       "m_iterator should never stop on an event of the wrong type.");
                m_event = static_cast<const EVENT&>(*m_iterator);
                m_event->setTimeSinceLastEvent(timeSinceLastEvent);
            }
            break;
        }
        ++m_iterator;
    }
}

template <typename EVENT>
babelwires::Span<bw_music::FilteredTrackIterator<EVENT>> bw_music::iterateOver(const Track& track) {
    FilteredTrackIterator<EVENT> begin(track);
    begin.initBegin();
    FilteredTrackIterator<EVENT> end(track);
    end.initEnd();
    return {begin, end};
}
