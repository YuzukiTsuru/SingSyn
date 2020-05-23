cmake_minimum_required(VERSION 3.16)
project(hts_engine)

set(CMAKE_C_STANDARD 99)

add_library(hts_engine
        HTS_audio.c
        HTS_engine.c
        HTS_gstream.c
        HTS_label.c
        HTS_misc.c
        HTS_model.c
        HTS_pstream.c
        HTS_sstream.c
        HTS_vocoder.c
        )
