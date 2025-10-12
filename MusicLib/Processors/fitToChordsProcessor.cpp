/**
 * Processor which adjusts notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/fitToChordsProcessor.hpp>

#include <MusicLib/Functions/fitToChordFunction.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/Types/Record/recordTypeConstructor.hpp>

bw_music::FitToChordsProcessorInput::FitToChordsProcessorInput()
    : GenericType(babelwires::RecordTypeConstructor::makeTypeRef(
                      getIdOfChordsArray(),
                      babelwires::ArrayTypeConstructor::makeTypeRef(
                          ChordType::getThisType(), 1, static_cast<unsigned int>(ChordType::Value::NUM_VALUES), 1),
                      getIdOfInput(), babelwires::TypeVariableTypeConstructor::makeTypeRef()),
                  1) {}

babelwires::ShortId bw_music::FitToChordsProcessorInput::getIdOfChordsArray() {
    return BW_SHORT_ID("chords", "chords", "946fd111-1e3a-40e5-ae53-c30e4a929a01");
}

babelwires::ShortId bw_music::FitToChordsProcessorInput::getIdOfInput() {
    return BW_SHORT_ID("input", "input", "6c2d63fb-b1f0-4cad-a7e6-c1bca583c6f3");
}

babelwires::ShortId bw_music::FitToChordsProcessorOutput::getIdOfResult() {
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

    std::vector<babelwires::EditableValueHolder> getValuesForOptionalFields() {
        const auto& fieldIds = bw_music::ChordType::getStaticValueSet();
        std::vector<babelwires::EditableValueHolder> values;
        values.reserve(static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES));
        for (unsigned int i = 0; i < static_cast<unsigned int>(bw_music::ChordType::Value::NUM_VALUES); ++i) {
            values.emplace_back(
                babelwires::FieldIdValue(fieldIds[i], babelwires::RecordType::Optionality::optionalDefaultInactive));
        }
        return values;
    }
} // namespace

bw_music::FitToChordsProcessorOutput::FitToChordsProcessorOutput()
    : GenericType(
          babelwires::TypeRef(babelwires::RecordTypeConstructor::makeTypeRef(
              getIdOfResult(), babelwires::TypeRef(babelwires::RecordTypeConstructor::getThisIdentifier(),
                                                   babelwires::TypeConstructorArguments{
                                                       getTypesForOptionalFields(), getValuesForOptionalFields()}))),
          1) {}

bw_music::FitToChordsProcessor::FitToChordsProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, FitToChordsProcessorInput::getThisType(), FitToChordsProcessorOutput::getThisType()) {}

void bw_music::FitToChordsProcessor::processValue(babelwires::UserLogger& userLogger,
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

    const FitToChordsProcessorInput& inputType = input.getType().is<FitToChordsProcessorInput>();
    const FitToChordsProcessorOutput& outputType = output.getType().is<FitToChordsProcessorOutput>();

    const babelwires::ValueHolder& inputValue = input.getValue();
    const babelwires::TypeRef& assignedInputTypeRef = inputType.getTypeAssignment(inputValue, 0);

    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();

    babelwires::ValueHolder newOutputValue = output.getValue();
    outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {assignedInputTypeRef});
    const auto [outputChild, outputStep, outputChildType] = outputType.getChildNonConst(newOutputValue, 0);
    const auto& outputRecordType = outputChildType.resolve(typeSystem).is<babelwires::RecordType>();
    const auto& [resultChild, resultStep, resultChildType] = outputRecordType.getChildNonConst(*outputChild, 0);
    const auto& resultRecordType = resultChildType.resolve(typeSystem).is<babelwires::RecordType>();

    const auto [inputChild, inputStep, inputChildType] = inputType.getChild(inputValue, 0);
    const auto& inputRecordType = inputChildType.resolve(typeSystem).is<babelwires::RecordType>();
    const auto [chordsArray, inputStep0, inputChildType0] = inputRecordType.getChild(*inputChild, 0);
    const auto& chordsArrayType = inputChildType0.resolve(typeSystem).is<babelwires::ArrayType>();
    const auto [inputStructure, inputStep1, inputChildType1] = inputRecordType.getChild(*inputChild, 1);

    const auto& chordType = typeSystem.getRegisteredType(ChordType::getThisIdentifier()).is<ChordType>();
    std::vector<babelwires::ShortId> selectedChords;
    for (unsigned int i = 0; i < chordsArrayType.getNumChildren(*chordsArray); ++i) {
        const auto [chordValueHolder, chordStep, chordChildType] = chordsArrayType.getChild(*chordsArray, i);
        assert(chordChildType == ChordType::getThisType());
        const babelwires::ShortId chordId = (*chordValueHolder)->is<babelwires::EnumValue>().get();

        assert(std::find(resultRecordType.getOptionalFieldIds().begin(), resultRecordType.getOptionalFieldIds().end(),
                         chordId) != resultRecordType.getOptionalFieldIds().end());
        if (std::find(selectedChords.begin(), selectedChords.end(), chordId) != selectedChords.end()) {
            continue;
        }
        selectedChords.emplace_back(chordId);
    }
    resultRecordType.ensureActivated(typeSystem, *resultChild, selectedChords);

    output.setValue(newOutputValue);
}
