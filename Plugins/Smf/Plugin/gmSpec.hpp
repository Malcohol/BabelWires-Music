/**
 * An enum which defines the available MIDI specifications.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Enum/enumWithCppEnum.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Instance/enumTypeInstance.hpp>
#include <BabelWiresLib/Instance/instance.hpp>

#define GM_SPEC_VALUES(X)                                                                                              \
    X(NONE, "No Specification", "0d8e86c7-3d0c-4a6c-8fb0-3aaa0410dfc1")                                                \
    X(GM, "General MIDI", "241ae73e-f1a2-490a-8341-1456b3dcc1fa")                                                      \
    X(GS, "Roland GS", "a51d7cf7-cc3d-4f1b-a83d-7706931becae")                                                         \
    X(XG, "Yamaha XG", "6be100b8-c581-4613-8cd3-c3f44c40f98d")                                                         \
    X(GM2, "General MIDI 2", "11fb43e0-f04c-44fc-bb84-6ebec830321d")

namespace smf {
    /// Carries the enum of GM Spec values.
    class GMSpecType : public babelwires::EnumType {
      public:
        REGISTERED_TYPE("GMSpec", "GM Specification", "4dc2566d-1be8-468b-9aa0-2f4d63344a13", 1);
        GMSpecType();

        ENUM_DEFINE_CPP_ENUM(GM_SPEC_VALUES);
    };
} // namespace smf
