/**
 * Runtime plugin entry point for the Smf plugin wrapper.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/libRegistration.hpp>

#define PLUGIN_REGISTRATION_FUNCTION smf::registerLib
#define PLUGIN_UUID "eb407280-a6ce-46d0-91cd-02a91354757a"
#include <BaseLib/PluginSupport/Detail/pluginEntryBoilerplate.inc>

