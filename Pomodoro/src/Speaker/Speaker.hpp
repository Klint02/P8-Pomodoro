#ifndef SPEAKER_HEADER
#define SPEAKER_HEADER

#define buzzer 14


namespace speaker {
    enum tones {
        congratulations,
        tada,
        woopwoop

    };

    void generateTone(int compilation); 
}

#endif