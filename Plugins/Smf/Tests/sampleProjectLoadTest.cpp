#include <gtest/gtest.h>

#include <Smf/libRegistration.hpp>

#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/Project/Nodes/SourceFileNode/sourceFileNodeData.hpp>
#include <BabelWiresLib/Project/Nodes/TargetFileNode/targetFileNodeData.hpp>
#include <BabelWiresLib/Serialization/projectSerialization.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

namespace {
    const char s_sampleProjectFileName[] = "DucksInARow.babelwires";
    const char s_sampleSourceFileName[] = "DucksInARow.mid";
    const char s_sampleTargetFileName[] = "DucksInARow_Round.mid";

    std::filesystem::path getDucksInARowProjectPath() {
        return std::filesystem::weakly_canonical(
            std::filesystem::path(BW_MIDI_TO_MIDI_SAMPLE_PROJECT_DIR) / s_sampleProjectFileName);
    }
}

TEST(SampleProjectLoadTest, midiToMidiSampleProjectResolvesSiblingMidiFiles) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    const std::filesystem::path projectPath = getDucksInARowProjectPath();
    ASSERT_TRUE(std::filesystem::exists(projectPath));

    const auto projectDataResult =
        babelwires::ProjectSerialization::loadFromFile(projectPath, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(projectDataResult) << projectDataResult.error().toString();

    bool foundSource = false;
    bool foundTarget = false;
    for (const auto& node : projectDataResult->m_nodes) {
        if (const auto* sourceFileNode = node->tryAs<babelwires::SourceFileNodeData>()) {
            EXPECT_EQ(sourceFileNode->m_filePath, projectPath.parent_path() / s_sampleSourceFileName);
            foundSource = true;
        }
        if (const auto* targetFileNode = node->tryAs<babelwires::TargetFileNodeData>()) {
            EXPECT_EQ(targetFileNode->m_filePath, projectPath.parent_path() / s_sampleTargetFileName);
            foundTarget = true;
        }
    }

    EXPECT_TRUE(foundSource);
    EXPECT_TRUE(foundTarget);
}
