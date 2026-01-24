#include <gtest/gtest.h>

#include <BaseLib/Identifiers/identifierRegistry.hpp>

int main(int argc, char** argv) {
    // Since these tests load "real" types which use the BW_SHORT_ID macros, they have to work within the same registry singleton.
    babelwires::IdentifierRegistryScope identifierRegistry;
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
