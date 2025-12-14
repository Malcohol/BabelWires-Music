/**
 * Processor which adjusts notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/buildAccompanimentProcessor.hpp>

#include <MusicLib/Functions/fitToChordFunction.hpp>
#include <MusicLib/Types/chordTypeSet.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Utilities/applyToSubvalues.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/Types/Record/recordTypeConstructor.hpp>
#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>

bw_music::BuildAccompanimentProcessorInput::BuildAccompanimentProcessorInput()
    : GenericType(babelwires::RecordTypeConstructor::makeTypeRef(
                      getIdOfChordsArray(), ChordTypeSet::getThisType(), getIdOfInput(),
                      babelwires::TypeVariableTypeConstructor::makeTypeRef()),
                  1) {}

babelwires::ShortId bw_music::BuildAccompanimentProcessorInput::getIdOfChordsArray() {
    return BW_SHORT_ID("chords", "chord types", "946fd111-1e3a-40e5-ae53-c30e4a929a01");
}

babelwires::ShortId bw_music::BuildAccompanimentProcessorInput::getIdOfInput() {
    return BW_SHORT_ID("input", "input", "6c2d63fb-b1f0-4cad-a7e6-c1bca583c6f3");
}

babelwires::ShortId bw_music::BuildAccompanimentProcessorOutput::getIdOfResult() {
    return BW_SHORT_ID("output", "output", "91cdf142-e8e1-4b86-8811-43028e3b4154");
}

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

bw_music::BuildAccompanimentProcessorOutput::BuildAccompanimentProcessorOutput()
    : GenericType(
          babelwires::TypeRef(babelwires::RecordTypeConstructor::makeTypeRef(
              getIdOfResult(), babelwires::TypeRef(babelwires::RecordTypeConstructor::getThisIdentifier(),
                                                   babelwires::TypeConstructorArguments{
                                                       getTypesForOptionalFields(), getValuesForOptionalFields()}))),
          1) {}

bw_music::BuildAccompanimentProcessor::BuildAccompanimentProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, BuildAccompanimentProcessorInput::getThisType(),
                BuildAccompanimentProcessorOutput::getThisType()) {}

namespace {

    // apply the fitToChordFunction to all tracks in the value.
    babelwires::ValueHolder applyFitToChordFunction(const babelwires::TypeSystem& typeSystem,
                                                         const babelwires::Type& type,
                                                         const babelwires::ValueHolder& sourceValue,
                                                         const bw_music::Chord& chord) {
        babelwires::ValueHolder result = sourceValue;

        babelwires::applyToSubvaluesOfType<bw_music::TrackType>(typeSystem, type, result,
                                                      [&chord](const babelwires::Type& t, babelwires::Value& v) {
                                                          auto& track = v.is<bw_music::Track>();
                                                          track = bw_music::fitToChordFunction(track, chord);
                                                      });

        return result;
    }

} // namespace

void bw_music::BuildAccompanimentProcessor::processValue(babelwires::UserLogger& userLogger,
                                                         const babelwires::ValueTreeNode& input,
                                                         babelwires::ValueTreeNode& output) const {
    /*
TODO:
1. [X] Need to be able to query the assigned type of the input - Input is generic with one type variable.
2. [X] Need to be able to build record types dynamically.
- [X] Regular fields
- [X] Optional fields
3. [X] The output is a generic record type with optional track fields, corresponding to selected chord types.
4. [ ] Process the input to produce the output.

Alternative: The output could be a generic type where the output record as a whole is substituted.
* Advantage: Might be simpler
* Disadvantage: More new types created any time chords are added or removed.
*/

    const BuildAccompanimentProcessorInput& inputType = input.getType().is<BuildAccompanimentProcessorInput>();
    const BuildAccompanimentProcessorOutput& outputType = output.getType().is<BuildAccompanimentProcessorOutput>();

    const babelwires::ValueHolder& inputValue = input.getValue();
    const babelwires::TypeRef& assignedInputTypeRef = inputType.getTypeAssignment(inputValue, 0);

    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();

    const auto [inputChild, inputStep, inputChildType] = inputType.getChild(inputValue, 0);
    const auto& inputRecordType = inputChildType.resolve(typeSystem).is<babelwires::RecordType>();
    const auto [chordsArray, inputStep0, inputChildType0] = inputRecordType.getChild(*inputChild, 0);
    const auto& chordsArrayType = inputChildType0.resolve(typeSystem).is<ChordTypeSet>();
    const auto [inputStructure, inputStep1, inputChildType1] = inputRecordType.getChild(*inputChild, 1);

    babelwires::ValueHolder newOutputValue = output.getValue();
    outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {assignedInputTypeRef});
    const auto [outputChild, outputStep, outputChildType] = outputType.getChildNonConst(newOutputValue, 0);
    const auto& outputRecordType = outputChildType.resolve(typeSystem).is<babelwires::RecordType>();
    const auto& [resultChild, resultStep, resultChildType] = outputRecordType.getChildNonConst(*outputChild, 0);
    const auto& resultRecordType = resultChildType.resolve(typeSystem).is<babelwires::RecordType>();

    const auto& chordType = typeSystem.getRegisteredType(ChordType::getThisIdentifier()).is<ChordType>();

    std::map<babelwires::ShortId, bool> selectedChords;
    {
        const std::set<ChordType::Value> chordSet = chordsArrayType.getChordTypesFromValue(typeSystem, *chordsArray);
        for (const auto& chordValue : chordSet) {
            const babelwires::ShortId chordId = chordType.getIdentifierFromValue(chordValue);
            selectedChords.emplace(chordId, true);
        }
    }

    resultRecordType.selectOptionals(typeSystem, *resultChild, selectedChords);

    for (const auto& maplet : selectedChords) {
        auto [fieldValueHolder, fieldTypeRef] = resultRecordType.getChildByIdNonConst(*resultChild, maplet.first);
        const auto& fieldType = fieldTypeRef.resolve(typeSystem);
        // Accompaniment always generated with a C root.
        const bw_music::Chord chord = {bw_music::PitchClass::Value::C, chordType.getValueFromIdentifier(maplet.first)};

        fieldValueHolder = applyFitToChordFunction(typeSystem, fieldType, *inputStructure, chord);
    }

    output.setValue(newOutputValue);
}
