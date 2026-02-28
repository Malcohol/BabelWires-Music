/**
 * A TypeConstructor which constructs a track type with an initial duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/musicTypes.hpp>

#include <BabelWiresLib/TypeSystem/typeConstructor.hpp>

namespace bw_music {
    /// Construct a track type with an initial duration.
    class TrackTypeConstructor : public babelwires::TypeConstructor {
      public:
        /// Note that the we don't represent the default in the name.
        TYPE_CONSTRUCTOR("Track", "Track", "bba2f18c-9ff1-406c-b39c-47a2e4047caa", 1);

        babelwires::ResultT<babelwires::TypePtr>
        constructType(const babelwires::TypeSystem& typeSystem, babelwires::TypeExp newTypeExp,
                      const babelwires::TypeConstructorArguments& arguments,
                      const std::vector<babelwires::TypePtr>& resolvedTypeArguments) const override;

      private:
        /// Throws a TypeSystem exception if the arguments are not of the expect type.
        static babelwires::ResultT<ModelDuration> extractValueArguments(const std::vector<babelwires::ValueHolder>& valueArguments);
    };
} // namespace bw_music