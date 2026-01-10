/**
 * Duration is a RationalType for holding a duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Rational/rationalType.hpp>

namespace bw_music {

    /// Duration is a RationalType for holding a duration.
    class Duration : public babelwires::RationalType {
      public:
        REGISTERED_TYPE("duration", "Duration", "d88c867e-f395-4f3c-bbe6-81c46314f3e5", 1);
        Duration() : babelwires::RationalType(getThisType(), {0, std::numeric_limits<babelwires::Rational::ComponentType>::max()}, 0) {}
    };

} // namespace bw_music
