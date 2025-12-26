/**
 * A type used to represents generic accompaniment data.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/genericAccompaniment.hpp>

#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/Types/Record/recordTypeConstructor.hpp>
#include <MusicLib/chord.hpp>

namespace {
    std::vector<babelwires::TypeRef> getTypesForOptionalFields() {
        std::vector<babelwires::TypeRef> types;
        types.reserve(static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES));
        for (unsigned int i = 0; i < static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES); ++i) {
            types.emplace_back(babelwires::TypeVariableTypeConstructor::makeTypeRef());
        }
        return types;
    }

    std::vector<babelwires::ValueHolder> getValuesForOptionalFields() {
        const auto& fieldIds = bw_music::ChordType::getStaticValueSet();
        std::vector<babelwires::ValueHolder> values;
        values.reserve(static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES));
        for (unsigned int i = 0; i < static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES); ++i) {
            values.emplace_back(
                babelwires::FieldIdValue(fieldIds[i], babelwires::RecordType::Optionality::optionalDefaultInactive));
        }
        return values;
    }
} // namespace

babelwires::TypeRef bw_music::getGenericAccompanimentTypeRef() {
    return babelwires::TypeRef(
        babelwires::RecordTypeConstructor::getThisIdentifier(),
        babelwires::TypeConstructorArguments{getTypesForOptionalFields(), getValuesForOptionalFields()});
}
