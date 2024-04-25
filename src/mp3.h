#pragma once
#include <Arduino.h>
#include "JQ6500_Serial.h"

class MP3 {
    private:
        uint8_t volume;
        JQ6500_Serial* handle;
        bool button_clicked;
        uint8_t current_bell;
        static bool is_bell_playing;

        static void bellDelayTimeout(void* args);

    public:
        /// @brief Constructors 
        MP3();

        /// @brief set the volume of the mp3
        /// @param new_volume new volume for mp3 [0-30]
        void setVolume(uint8_t new_volume);

        /// @brief returns the current volume. 
        uint8_t getVolume(void);

        /// @brief return the current bell. 
        // uint8_t getBell(void);

        /// @brief set the bell of the mp3
        /// @param bell new bell for mp3 [1-BELL_QUANTITY]
        // void setBell(uint8_t bell);

        /// @brief play current sound
        // void playBell(void);

        void play(void);
        
        /// @brief put on loop function. Listen to the button pin  
        // void buttonListerner(void);
};

