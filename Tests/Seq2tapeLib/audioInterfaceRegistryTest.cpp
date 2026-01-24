#include <gtest/gtest.h>

#include <Seq2tapeLib/Audio/audioDest.hpp>
#include <Seq2tapeLib/Audio/audioInterface.hpp>
#include <Seq2tapeLib/Audio/audioSource.hpp>
#include <BaseLib/Identifiers/identifierRegistry.hpp>

#include <Tests/TestUtils/equalSets.hpp>
#include <Tests/TestUtils/testIdentifiers.hpp>

#include <cassert>

namespace {
    /// Identifies itself via the getNumChannels method.
    struct TestAudioDest : babelwires::AudioDest {
        TestAudioDest(int numChannels)
            : m_numChannels(numChannels) {}

        int getNumChannels() const { return m_numChannels; }

        babelwires::Duration getFrequency() const { return 0.0; }

        unsigned long writeMoreAudioData(const babelwires::AudioSample* buffer, unsigned long bufSize) override {
            return 0;
        }

        int m_numChannels = 0;
    };

    /// Identifies itself via the getNumChannels method.
    struct TestAudioSource : babelwires::AudioSource {
        TestAudioSource(int numChannels)
            : m_numChannels(numChannels) {}

        int getNumChannels() const override { return m_numChannels; }

        babelwires::Duration getFrequency() const override { return 0.0; }

        unsigned long getMoreAudioData(babelwires::AudioSample* buffer, unsigned long bufSize) override { return 0; }

        int m_numChannels = 0;
    };

    /// Returns AudioDest and AudioSource which can be identifed by their baseNumChannels.
    struct TestAudioInterface : babelwires::AudioInterface {
        TestAudioInterface(std::string name, int baseNumChannels)
            : babelwires::AudioInterface(testUtils::getTestRegisteredLongIdentifier(name), 1)
            , m_name(name)
            , m_baseNumChannels(baseNumChannels) {}

        std::vector<std::string> getDestinationNames() const override {
            return std::vector<std::string>{m_name + "1", m_name + "2"};
        }

        std::unique_ptr<babelwires::AudioDest> getDestination(std::string_view destinationName) const {
            if ((m_name + "1") == destinationName) {
                return std::make_unique<TestAudioDest>(m_baseNumChannels + 1);
            } else if ((m_name + "2") == destinationName) {
                return std::make_unique<TestAudioDest>(m_baseNumChannels + 2);
            } else {
                assert(false);
                return nullptr;
            }
        }

        std::vector<std::string> getSourceNames() const {
            return std::vector<std::string>{m_name + "1", m_name + "2", m_name + "3"};
        }

        std::unique_ptr<babelwires::AudioSource> getSource(std::string_view sourceName) const {
            if ((m_name + "1") == sourceName) {
                return std::make_unique<TestAudioSource>(m_baseNumChannels + 1);
            } else if ((m_name + "2") == sourceName) {
                return std::make_unique<TestAudioSource>(m_baseNumChannels + 2);
            } else if ((m_name + "3") == sourceName) {
                return std::make_unique<TestAudioSource>(m_baseNumChannels + 3);
            } else {
                assert(false);
                return nullptr;
            }
        }

        std::string m_name;
        int m_baseNumChannels;
    };
} // namespace

TEST(AudioInterfaceRegistryTest, destinationsAndSources) {
    babelwires::IdentifierRegistryScope identifierRegistry;
    babelwires::AudioInterfaceRegistry reg;

    EXPECT_TRUE(reg.getDestinationNames().empty());
    EXPECT_TRUE(reg.getSourceNames().empty());

    reg.addEntry<TestAudioInterface>("foo", 10);

    using setOfStrings = std::vector<std::string>;

    EXPECT_TRUE(testUtils::areEqualSets(reg.getDestinationNames(), setOfStrings{"foo::foo1", "foo::foo2"}));
    {
        auto destFoo1 = reg.getDestination("foo::foo1");
        EXPECT_EQ(destFoo1->getNumChannels(), 11);
    }
    {
        auto destFoo2 = reg.getDestination("foo::foo2");
        EXPECT_EQ(destFoo2->getNumChannels(), 12);
    }
    EXPECT_TRUE(testUtils::areEqualSets(reg.getSourceNames(), setOfStrings{"foo::foo1", "foo::foo2", "foo::foo3"}));
    {
        auto sourceFoo1 = reg.getSource("foo::foo1");
        EXPECT_EQ(sourceFoo1->getNumChannels(), 11);
    }
    {
        auto sourceFoo2 = reg.getSource("foo::foo2");
        EXPECT_EQ(sourceFoo2->getNumChannels(), 12);
    }
    {
        auto sourceFoo3 = reg.getSource("foo::foo3");
        EXPECT_EQ(sourceFoo3->getNumChannels(), 13);
    }
    reg.addEntry<TestAudioInterface>("erm", 20);
    EXPECT_TRUE(testUtils::areEqualSets(reg.getDestinationNames(),
                                        setOfStrings{"foo::foo1", "foo::foo2", "erm::erm1", "erm::erm2"}));
    {
        auto destErm1 = reg.getDestination("erm::erm1");
        EXPECT_EQ(destErm1->getNumChannels(), 21);
    }
    {
        auto destFoo2 = reg.getDestination("foo::foo2");
        EXPECT_EQ(destFoo2->getNumChannels(), 12);
    }
    {
        auto destErm2 = reg.getDestination("erm::erm2");
        EXPECT_EQ(destErm2->getNumChannels(), 22);
    }
    EXPECT_TRUE(testUtils::areEqualSets(reg.getSourceNames(), setOfStrings{"foo::foo1", "foo::foo2", "foo::foo3",
                                                                           "erm::erm1", "erm::erm2", "erm::erm3"}));
    {
        auto sourceErm1 = reg.getSource("erm::erm1");
        EXPECT_EQ(sourceErm1->getNumChannels(), 21);
    }
    {
        auto sourceFoo3 = reg.getSource("foo::foo3");
        EXPECT_EQ(sourceFoo3->getNumChannels(), 13);
    }
    {
        auto sourceErm2 = reg.getSource("erm::erm2");
        EXPECT_EQ(sourceErm2->getNumChannels(), 22);
    }
    {
        auto sourceErm3 = reg.getSource("erm::erm3");
        EXPECT_EQ(sourceErm3->getNumChannels(), 23);
    }
}
