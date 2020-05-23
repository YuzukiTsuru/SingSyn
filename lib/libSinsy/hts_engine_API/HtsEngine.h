#ifndef SINSY_HTS_ENGINE_H_
#define SINSY_HTS_ENGINE_H_

#include <vector>
#include <string>
#include "util_log.h"
#include "HTS_engine.h"

namespace sinsy {
    class LabelStrings;

    class SynthConditionImpl;

    class HtsEngine {
    public:
        //! constructor
        HtsEngine();

        //! destructor
        virtual ~HtsEngine();

        //! reset
        void reset();

        //! load voices
        bool load(const std::vector<std::string> &);

        //! synthesize
        bool synthesize(const LabelStrings &label, SynthConditionImpl &condition);

        //! stop synthesizing
        void stop();

        //! reset stop flag
        void resetStopFlag();

        //! set alpha
        bool setAlpha(double);

        //! set tone
        bool setTone(double);

        //! set speed
        bool setSpeed(double);

        //! set volume
        bool setVolume(double);

        //! set interpolation weight
        bool setInterpolationWeight(size_t, double);

    private:
        //! copy constructor (donot use)
        HtsEngine(const HtsEngine &);

        //! assignment operator (donot use)
        HtsEngine &operator=(const HtsEngine &);

        //! initialize
        void init();

        //! clear
        void clear();

        //! hts_engine API
        HTS_Engine engine;

        //! default frame period
        size_t fperiod{};
    };

};

#endif // SINSY_HTS_ENGINE_H_
