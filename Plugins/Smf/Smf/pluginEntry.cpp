/**
 * Runtime plugin entry point for the Smf plugin wrapper.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/libRegistration.hpp>

#define PLUGIN_REGISTRATION_FUNCTION smf::registerLib;
#include <BaseLib/PluginSupport/detail/pluginEntryBoilerplate.inc>

