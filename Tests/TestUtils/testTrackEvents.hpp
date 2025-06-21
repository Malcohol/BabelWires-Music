#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace testUtils {

    struct TestTrackEvent : bw_music::TrackEvent {
        STREAM_EVENT(TestTrackEvent);
        TestTrackEvent(bw_music::ModelDuration d, int value = 1)
            : TrackEvent(d)
            , m_value(value) {}
        void createEndEvent(bw_music::TrackEventHolder& dest, bw_music::ModelDuration timeSinceLastEvent) const override {}

        int m_value;
    };

    struct TestTrackEvent2 : bw_music::TrackEvent {
        STREAM_EVENT(TestTrackEvent2);
        TestTrackEvent2(bw_music::ModelDuration d, float value)
            : TrackEvent(d)
            , m_value(value) {}
        void createEndEvent(bw_music::TrackEventHolder& dest, bw_music::ModelDuration timeSinceLastEvent) const override {}

        float m_value;
    };

    struct alignas(16) BigTestTrackEvent : bw_music::TrackEvent {
        STREAM_EVENT(BigTestTrackEvent);
        BigTestTrackEvent(bw_music::ModelDuration d)
            : TrackEvent(d) {}
        void createEndEvent(bw_music::TrackEventHolder& dest, bw_music::ModelDuration timeSinceLastEvent) const override {}

        std::uint64_t m_big[5] = {};
    };

    struct Payload {
        Payload(int& payloadDestructionCounter)
            : m_payloadDestructionCounter(payloadDestructionCounter) {}

        ~Payload() { ++m_payloadDestructionCounter; }

        int& m_payloadDestructionCounter;
    };

    struct TestTrackEventWithPayload : bw_music::TrackEvent {
        STREAM_EVENT(TestTrackEventWithPayload);

        TestTrackEventWithPayload(bw_music::ModelDuration d, int& payloadDestructionCounter)
            : TrackEvent(d)
            , m_payload(std::make_unique<Payload>(payloadDestructionCounter)) {}

        TestTrackEventWithPayload(TestTrackEventWithPayload&& other)
            : TrackEvent(other) {
            m_payload = std::move(other.m_payload);
        }

        TestTrackEventWithPayload(const TestTrackEventWithPayload& other)
            : bw_music::TrackEvent(other)
            , m_payload(std::make_unique<Payload>(*other.m_payload)) {}

        ~TestTrackEventWithPayload() { m_payload.reset(); }

        void createEndEvent(bw_music::TrackEventHolder& dest, bw_music::ModelDuration timeSinceLastEvent) const override {}

        std::unique_ptr<Payload> m_payload;
    };

} // namespace
