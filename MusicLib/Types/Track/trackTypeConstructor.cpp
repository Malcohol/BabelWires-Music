/**
 * A TypeConstructor which constructs a track type with an initial duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/trackTypeConstructor.hpp>

#include <BabelWiresLib/TypeSystem/typeSystemException.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

bw_music::ModelDuration bw_music::TrackTypeConstructor::extractValueArguments(
    const std::vector<babelwires::EditableValueHolder>& valueArguments) {
    if (valueArguments.size() != 1) {
        throw babelwires::TypeSystemException()
            << "TrackTypeConstructor expects 1 value arguments but got " << valueArguments.size();
    }

    if (const babelwires::RationalValue* rationalValue = valueArguments[0]->as<babelwires::RationalValue>()) {
        return rationalValue->get();
    } else {
        throw babelwires::TypeSystemException() << "Argument 0 given to TrackTypeConstructor was not a RationalValue";
    }
}

babelwires::TypeConstructor::TypeConstructorResult bw_music::TrackTypeConstructor::constructType(const babelwires::TypeSystem& typeSystem, 
    babelwires::TypeRef newTypeRef, const std::vector<const babelwires::Type*>& typeArguments,
    const std::vector<babelwires::EditableValueHolder>& valueArguments) const {
    if (typeArguments.size() != 0) {
        throw babelwires::TypeSystemException()
            << "TrackTypeConstructor does not expect type arguments but got " << typeArguments.size();
    }
    ModelDuration initialDuration = extractValueArguments(valueArguments);
    return std::make_unique<babelwires::ConstructedType<TrackType>>(std::move(newTypeRef), initialDuration);
}

