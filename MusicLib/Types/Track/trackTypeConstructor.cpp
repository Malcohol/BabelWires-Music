/**
 * A TypeConstructor which constructs a track type with an initial duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/trackTypeConstructor.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <BaseLib/Result/error.hpp>
#include <BaseLib/Result/resultDSL.hpp>

babelwires::ResultT<bw_music::ModelDuration> bw_music::TrackTypeConstructor::extractValueArguments(
    const std::vector<babelwires::ValueHolder>& valueArguments) {
    if (valueArguments.size() != 1) {
        return babelwires::Error()
            << "TrackTypeConstructor expects 1 value arguments but got " << valueArguments.size();
    }

    if (const babelwires::RationalValue* rationalValue = valueArguments[0]->tryAs<babelwires::RationalValue>()) {
        return rationalValue->get();
    } else {
        return babelwires::Error() << "Argument 0 given to TrackTypeConstructor was not a RationalValue";
    }
}

babelwires::ResultT<babelwires::TypePtr>
bw_music::TrackTypeConstructor::constructType(const babelwires::TypeSystem& typeSystem, babelwires::TypeExp newTypeExp,
                                              const babelwires::TypeConstructorArguments& arguments,
                                              const std::vector<babelwires::TypePtr>& resolvedTypeArguments) const {
    if (arguments.getTypeArguments().size() != 0) {
        return babelwires::Error() << "TrackTypeConstructor does not expect type arguments but got "
                                  << arguments.getTypeArguments().size();
    }
    ASSIGN_OR_ERROR(auto initialDuration, extractValueArguments(arguments.getValueArguments()));
    return babelwires::makeType<TrackType>(std::move(newTypeExp), initialDuration);
}
