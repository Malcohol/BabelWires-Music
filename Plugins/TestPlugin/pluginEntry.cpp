/**
 * Runtime plugin entry point for the TestPlugin plugin wrapper.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <TestPlugin/libRegistration.hpp>

#define PLUGIN_REGISTRATION_FUNCTION bw_music_testplugin::registerLib
#define PLUGIN_UUID "6d83035e-3530-48d5-a893-fcf1a865a590"
#include <BaseLib/PluginSupport/Detail/pluginEntryBoilerplate.inc>

