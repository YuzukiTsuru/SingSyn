#include <iostream>
#include "Sinsy.h"

namespace {
    const char *DEFAULT_LANGS = "j";
};

void usage() {
    std::cout << "The HMM-Based Singing Voice Syntheis System \"Sinsy\"" << std::endl;
    std::cout << "Version 0.92 (http://sinsy.sourceforge.net/)" << std::endl;
    std::cout << "Copyright (C) 2009-2015 Nagoya Institute of Technology" << std::endl;
    std::cout << "All rights reserved." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "The HMM-Based Speech Synthesis Engine \"hts_engine API\"" << std::endl;
    std::cout << "Version 1.10 (http://hts-engine.sourceforge.net/)" << std::endl;
    std::cout << "Copyright (C) 2001-2015 Nagoya Institute of Technology" << std::endl;
    std::cout << "              2001-2008 Tokyo Institute of Technology" << std::endl;
    std::cout << "All rights reserved." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "sinsy - The HMM-based singing voice synthesis system \"Sinsy\"" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  usage:" << std::endl;
    std::cout << "    sinsy [ options ] [ infile ]" << std::endl;
    std::cout << "  options:                                           [def]" << std::endl;
    std::cout << "    -w langs    : languages                          [  j]" << std::endl;
    std::cout << "                  j: Japanese                             " << std::endl;
    std::cout << "                  (Currently, you can set only Japanese)  " << std::endl;
    std::cout << "    -x dir      : dictionary directory               [N/A]" << std::endl;
    std::cout << "    -m htsvoice : HTS voice file                     [N/A]" << std::endl;
    std::cout << "    -o file     : filename of output wav audio       [N/A]" << std::endl;
    std::cout << "  infile:" << std::endl;
    std::cout << "    MusicXML file" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        return -1;
    }

    std::string xml;
    std::string voice;
    std::string config;
    std::string wav;
    std::string languages(DEFAULT_LANGS);

    int i(1);
    for (; i < argc; ++i) {
        if ('-' != argv[i][0]) {
            if (xml.empty()) {
                xml = argv[i];
            } else {
                std::cout << "[ERROR] invalid option : '" << argv[i][1] << "'" << std::endl;
                usage();
                return -1;
            }
        } else {
            switch (argv[i][1]) {
                case 'w' :
                    languages = argv[++i];
                    break;
                case 'x' :
                    config = argv[++i];
                    break;
                case 'm' :
                    voice = argv[++i];
                    break;
                case 'o' :
                    wav = argv[++i];
                    break;
                case 'h' :
                    usage();
                    return 0;
                default :
                    std::cout << "[ERROR] invalid option : '-" << argv[i][1] << "'" << std::endl;
                    usage();
                    return -1;
            }
        }
    }

    if (xml.empty() || voice.empty()) {
        usage();
        return -1;
    }

    sinsy::Sinsy sinsy;

    std::vector<std::string> voices;
    voices.push_back(voice);

    if (!sinsy.setLanguages(languages, config)) {
        std::cout << "[ERROR] failed to set languages : " << languages << ", config dir : " << config << std::endl;
        return -1;
    }

    if (!sinsy.loadVoices(voices)) {
        std::cout << "[ERROR] failed to load voices : " << voice << std::endl;
        return -1;
    }

    if (!sinsy.loadScoreFromMusicXML(xml)) {
        std::cout << "[ERROR] failed to load score from MusicXML file : " << xml << std::endl;
        return -1;
    }

    sinsy::SynthCondition condition;

    if (wav.empty()) {
        condition.setPlayFlag();
    } else {
        condition.setSaveFilePath(wav);
    }

    sinsy.synthesize(condition);

    return 0;
}
