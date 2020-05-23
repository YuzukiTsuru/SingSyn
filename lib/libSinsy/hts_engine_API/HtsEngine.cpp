#include <cmath>
#include <cstring>
#include <vector>
#include <limits>
#include <cstdlib>
#include <climits>
#include "HtsEngine.h"
#include "LabelStrings.h"
#include "SynthConditionImpl.h"

namespace sinsy {

/*!
 constructor
 */
    HtsEngine::HtsEngine() {
        init();
    }

/*!
 destructor
 */
    HtsEngine::~HtsEngine() {
        clear();
    }

/*!
 initialize
 */
    void HtsEngine::init() {
        HTS_Engine_initialize(&engine);
        fperiod = 0;
    }

/*!
 clear
 */
    void HtsEngine::clear() {
        HTS_Engine_clear(&engine);
    }

/*!
 reset
 */
    void HtsEngine::reset() {
        clear();
        init();
    }

/*!
 load voices
*/
    bool HtsEngine::load(const std::vector<std::string> &voices) {
        size_t i;
        char **fn_voices = nullptr;

        // check
        if (voices.empty()) {
            return false;
        }

        // get HTS voice file names
        fn_voices = (char **) malloc(voices.size() * sizeof(char *));
        if (nullptr == fn_voices) {
            throw std::bad_alloc();
        }
        for (i = 0; i < voices.size(); i++) {
            fn_voices[i] = strdup(voices[i].c_str());
        }

        // load HTS voices
        if (HTS_Engine_load(&engine, fn_voices, voices.size()) != TRUE) {
            HTS_Engine_clear(&engine);
            for (i = 0; i < voices.size(); i++) {
                free(fn_voices[i]);
            }
            free(fn_voices);
            return false;
        }

        for (i = 0; i < voices.size(); i++) {
            free(fn_voices[i]);
        }
        free(fn_voices);

        // save default frame period
        fperiod = HTS_Engine_get_fperiod(&engine);

        // set audio buffer size (100ms)
        HTS_Engine_set_audio_buff_size(&engine, (size_t) ((double) HTS_Engine_get_sampling_frequency(&engine) * 0.100));

        // phoneme alignment
        HTS_Engine_set_phoneme_alignment_flag(&engine, TRUE);

        return true;
    }

/*!
 synthesize
*/
    bool HtsEngine::synthesize(const LabelStrings &label, SynthConditionImpl &condition) {
        // check
        if (HTS_Engine_get_nvoices(&engine) == 0 || label.size() == 0) {
            return false;
        }

        bool playFlag = condition.playFlag;
        bool saveFlag = !condition.saveFilePath.empty();
        bool storeFlag = (nullptr != condition.waveformBuffer);

        // nothing to do
        if (!playFlag && !saveFlag && !storeFlag) {
            return true;
        }

        FILE *fp(nullptr);
        if (saveFlag) {
            fp = fopen(condition.saveFilePath.c_str(), "wb");
            if (nullptr == fp) {
                return false;
            }
        }

        size_t x = HTS_Engine_get_audio_buff_size(&engine);
        if (playFlag) {
            HTS_Engine_set_audio_buff_size(&engine, x); // reset audio device
        } else {
            HTS_Engine_set_audio_buff_size(&engine, 0);
        }

        int error = 0; // 0: no error 1: unknown error 2: bad alloc
        if (HTS_Engine_synthesize_from_strings(&engine, (char **) label.getData(), label.size()) != TRUE) {
            error = 1;
        }

        if (saveFlag) {
            if (0 == error)
                HTS_Engine_save_riff(&engine, fp);
            fclose(fp);
        }
        if (storeFlag && 0 == error) {
            if (condition.waveformBuffer) {
                size_t numSamples = HTS_Engine_get_nsamples(&engine);
                condition.waveformBuffer->resize(numSamples);
                for (size_t i = 0; i < numSamples; ++i)
                    (*condition.waveformBuffer)[i] = HTS_Engine_get_generated_speech(&engine, i);
            }
        }

        HTS_Engine_set_audio_buff_size(&engine, x);

        HTS_Engine_refresh(&engine);

        if (2 == error) {
            throw std::bad_alloc();
        }
        return (0 == error);
    }

/*!
 stop
*/
    void HtsEngine::stop() {
        HTS_Engine_set_stop_flag(&engine, true);
    }

/*!
 reset stop flag
*/
    void HtsEngine::resetStopFlag() {
        HTS_Engine_set_stop_flag(&engine, false);
    }

/*!
 set alpha
*/
    bool HtsEngine::setAlpha(double alpha) {
        if (0 == HTS_Engine_get_nvoices(&engine)) {
            return false;
        }

        HTS_Engine_set_alpha(&engine, alpha);
        return true;
    }

/*!
 set tone
*/
    bool HtsEngine::setTone(double tone) {
        if (0 == HTS_Engine_get_nvoices(&engine)) {
            return false;
        }

        HTS_Engine_add_half_tone(&engine, tone);
        return true;
    }

/*!
 set speed
*/
    bool HtsEngine::setSpeed(double speed) {
        if (0 == HTS_Engine_get_nvoices(&engine)) {
            return false;
        }
        if (0.0 == speed) {
            return false;
        }

        HTS_Engine_set_fperiod(&engine, static_cast<size_t>(fperiod / speed));
        return true;
    }

/*!
 set volume
*/
    bool HtsEngine::setVolume(double volume) {
        if (0 == HTS_Engine_get_nvoices(&engine)) {
            return false;
        }

        HTS_Engine_set_volume(&engine, volume);
        return true;
    }

/*!
 set interpolation weight
*/
    bool HtsEngine::setInterpolationWeight(size_t index, double weight) {
        if (0 == HTS_Engine_get_nvoices(&engine)) {
            return false;
        }
        if (HTS_Engine_get_nvoices(&engine) <= index) {
            return false;
        }

        HTS_Engine_set_duration_interpolation_weight(&engine, index, weight);
        for (size_t i = 0; i < HTS_Engine_get_nstream(&engine); i++) {
            HTS_Engine_set_parameter_interpolation_weight(&engine, index, i, weight);
            HTS_Engine_set_gv_interpolation_weight(&engine, index, i, weight);
        }

        return true;
    }

};  // namespace sinsy
