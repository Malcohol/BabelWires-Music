/**
 * Function which maps notes events using a percussion map.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

#include <BabelWiresLib/TypeSystem/typeConstructor.hpp>
#include <BabelWiresLib/Types/Sum/sumType.hpp>

namespace babelwires {
    class MapValue;
    class TypeSystem;
} // namespace babelwires

namespace bw_music {

    /// A 0-ary type constructor which constructs a sum type of all registered percussion types.
    /// A type constructor is used, since when the SumType itself is built, we know all the percussion types should
    /// be registered already.
    class PercussionMapType : public babelwires::TypeConstructor {
      public:
        TYPE_CONSTRUCTOR("PercMaps", "Percussion Map Types", "c6d3d105-5823-4764-a4bb-5a9b12abab03", 1);

        TypeConstructorResult
        constructType(const babelwires::TypeSystem& typeSystem, babelwires::TypeRef newTypeRef,
                      const std::vector<const babelwires::Type*>& typeArguments,
                      const std::vector<babelwires::EditableValueHolder>& valueArguments) const override;
    };

    babelwires::TypeRef getPercussionMapType();

    ///
    Track mapPercussionFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                const babelwires::MapValue& percussionMapValue);
} // namespace bw_music
