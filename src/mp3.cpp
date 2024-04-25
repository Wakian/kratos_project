#include "mp3.h"
// #include "timer.h"
// #include "consts.h"
// #include <EEPROM.h>

bool MP3::is_bell_playing = false;

MP3::MP3() {
    Serial2.begin(9600);
    volume = 15;
    if (volume > 30) {
        volume = 0;
    }

    handle = new JQ6500_Serial(Serial2);
    handle->reset();
    handle->setLoopMode(MP3_LOOP_ONE_STOP);
    handle->setVolume(volume);

    is_bell_playing = false;
    button_clicked = false;

    // pinMode(BUTTON, INPUT_PULLUP);
}

uint8_t MP3::getVolume() {
    return volume;
}

void MP3::setVolume(uint8_t new_volume) {
    if (new_volume > 30) {
        return;
    }

    volume = new_volume;
    handle->setVolume(new_volume);
    
}

void MP3::play() {
    if(handle->getStatus() != MP3_STATUS_PLAYING){
        handle->play();
    }
    
        
}

// void MP3::buttonListerner(){
//     if (!digitalRead(BUTTON) && !button_clicked) {
//         playBell();
//         button_clicked = true;
//     } else if (digitalRead(BUTTON) && button_clicked) {
//         button_clicked = false;
//     }
// }