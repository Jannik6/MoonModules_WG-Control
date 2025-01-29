#pragma once

#include "wled.h"

// Define a unique ID for this usermod
#define USERMOD_ID_DIP_SWITCH 0x0100

class DIPSwitchUsermod : public Usermod {
  private:
    // Default pins to connect to multiplexer
    int S0 = 27;
    int S1 = 26;
    int S2 = 25;
    int S3 = 33;
    int SIG_PIN = 12;

    uint16_t DMX_start_Address = 1;   // Default DMX Address
    unsigned long lastTime = 0;
    bool initDone = false;
    int next_input = 0;
    int dmx_mode = 1;                 // Switch between selecting DMX-Address or Preset-Selection
    int8_t lastPreset = 0;

    // Helper function to read DIP switch values using the multiplexer
    void updateDMX_start_Address() {
      digitalWrite(S0, next_input & 0x01);
      digitalWrite(S1, (next_input >> 1) & 0x01);
      digitalWrite(S2, (next_input >> 2) & 0x01);
      digitalWrite(S3, (next_input >> 3) & 0x01);

      delayMicroseconds(5); // Short delay to stabilize signal

      int state = digitalRead(SIG_PIN);      
      
      #ifdef DIPSWITCHUSERMOD_INVERT
        if (next_input == 0) {
          dmx_mode = state;
        } else {
          if (dmx_mode == 0){
            lastPreset = currentPreset;
            if (state == LOW) {
              currentPreset |= (1 << (9 - next_input));
            } else {
              currentPreset &= ~(1 << (9 - next_input));
            }
            if (lastPreset != currentPreset){
              applyPreset(currentPreset);
              colorUpdated(CALL_MODE_FX_CHANGED);
            } 
              
          } else {
            if (state == LOW) {
              DMXAddress |= (1 << (9 - next_input));
            } else {
              DMXAddress &= ~(1 << (9 - next_input));
            }
          }
        }
      #else
        if (next_input == 9) {
            dmx_mode = state;
          } else {
            if (dmx_mode == 0){
              int old = currentPreset;
              if (state == LOW) {
                currentPreset |= (1 << (next_input));
              } else {
                currentPreset &= ~(1 << (next_input));
              }
              if (old != currentPreset) colorUpdated(CALL_MODE_FX_CHANGED);
            } else {
              if (state == LOW) {
                DMXAddress |= (1 << (next_input));
              } else {
                DMXAddress &= ~(1 << (next_input));
              }
            }
          }
      #endif
      next_input++;
      if (next_input > 9) next_input = 0;
    }

  public:
    // Constructor with usermod name and enabled flag
    DIPSwitchUsermod(const char *name, bool enabled): Usermod(name, enabled) {}

    void setup() {
      pinMode(S0, OUTPUT);
      pinMode(S1, OUTPUT);
      pinMode(S2, OUTPUT);
      pinMode(S3, OUTPUT);
      pinMode(SIG_PIN, INPUT_PULLUP); // Enable internal pullup
      DMXAddress = 0;
      updateDMX_start_Address();
      initDone = true;
    }

    // Loop: periodically check for changes in DIP switch values
    void loop() {
      if (!enabled || millis() - lastTime < 1000) return;

      updateDMX_start_Address();
      lastTime = millis();
    }

    // Read configuration (pin assignments)
    bool readFromConfig(JsonObject& root) {
      JsonObject top = root[FPSTR(_name)];
      if (top.isNull()) return false;

      bool configComplete = true;
      configComplete &= getJsonValue(top["enabled"], enabled, false);
      configComplete &= getJsonValue(top["S0"], S0, 27);
      configComplete &= getJsonValue(top["S1"], S1, 26);
      configComplete &= getJsonValue(top["S2"], S2, 12);
      configComplete &= getJsonValue(top["S3"], S3, 33);
      configComplete &= getJsonValue(top["SIG_PIN"], SIG_PIN, 25);

      return configComplete;
    }

    // Save configuration
    void addToConfig(JsonObject& root) {
      JsonObject top = root.createNestedObject(FPSTR(_name));
      top["enabled"] = true;
      top["S0"] = S0;
      top["S1"] = S1;
      top["S2"] = S2;
      top["S3"] = S3;
      top["SIG_PIN"] = SIG_PIN;
    }

    // Provide usermod ID
    uint16_t getId() {
      return USERMOD_ID_DIP_SWITCH;
    }
};
