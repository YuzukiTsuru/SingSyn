cmake_minimum_required(VERSION 2.8)
project(libSingSyn)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(hts_engine_API)

include_directories(
        converter/
        hts_engine_API/
        japanese/
        label/
        score/
        temporary/
        util/
        xml/
)

add_library(libSingSyn
        # converter
        converter/ConfGroup.cpp
        converter/ConfManager.cpp
        converter/Converter.cpp
        converter/PhonemeInfo.cpp
        converter/UnknownConf.cpp
        converter/util_converter.cpp

        # hts_engine_API
        hts_engine_API/HtsEngine.cpp
        hts_engine_API/SynthConditionImpl.cpp

        # japanese
        japanese/JConf.cpp

        # label
        label/LabelData.cpp
        label/LabelMaker.cpp
        label/LabelMeasure.cpp
        label/LabelPosition.cpp
        label/LabelStream.cpp
        label/LabelStrings.cpp
        label/NoteGroup.cpp
        label/NoteLabeler.cpp
        label/PhonemeLabeler.cpp
        label/SyllableLabeler.cpp

        # Score
        score/Beat.cpp
        score/Dynamics.cpp
        score/Key.cpp
        score/Mode.cpp
        score/Note.cpp
        score/Pitch.cpp
        score/ScorePosition.cpp
        score/Slur.cpp
        score/Syllabic.cpp
        score/util_score.cpp

        # temporary
        temporary/ScoreDoctor.cpp
        temporary/TempScore.cpp

        # util
        util/Configurations.cpp
        util/DiphthongConverter.cpp
        util/InputFile.cpp
        util/IStringable.cpp
        util/MacronTable.cpp
        util/MultibyteCharRange.cpp
        util/OutputFile.cpp
        util/PhonemeTable.cpp
        util/StringTokenizer.cpp
        util/util_string.cpp

        # xml
        xml/XmlData.cpp
        xml/XmlParser.cpp
        xml/XmlReader.cpp
        xml/XmlWriter.cpp

        # Sinsy
        Sinsy.cpp
        )

target_link_libraries(libSingSyn hts_engine)