/**
 * Runtime plugin entry point for the Smf plugin wrapper.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/libRegistration.hpp>

#include <BaseLib/BuildCompatibility/buildFingerprint.hpp>
#include <BaseLib/PluginSupport/pluginDescriptor.hpp>
#include <BaseLib/Version/codebaseVersion_gen.hpp>

namespace {

    std::size_t smfGetBuildFingerprint(char* buffer, std::size_t bufferSize) {
        return babelwires::writeBuildFingerprint(buffer, bufferSize);
    }

    void smfRegisterPlugin(babelwires::Context& context) {
        smf::registerLib(context);
    }

} // namespace

extern "C" BABELWIRES_PLUGIN_EXPORT void babelwires_getPluginDescriptor(babelwires::PluginDescriptor* out) {
    out->m_codebaseVersion = {babelwires::detail::c_codebaseVersionMajor,
                              babelwires::detail::c_codebaseVersionMinor,
                              babelwires::detail::c_codebaseVersionPatch};
    out->getBuildFingerprint = &smfGetBuildFingerprint;
    out->registerPlugin = &smfRegisterPlugin;
}
