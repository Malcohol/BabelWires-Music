/**
 * An enum which defines the available MIDI specifications.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(smf::GMSpecType, GM_SPEC_VALUES);

smf::GMSpecType::GMSpecType()
    : EnumType(getThisIdentifier(), getStaticValueSet(), 1) {}
