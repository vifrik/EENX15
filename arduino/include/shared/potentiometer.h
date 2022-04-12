#include "Wire.h"

class Potentiometer {
    public:
        uint32_t read() {
            return analogRead(A0);
        }
};