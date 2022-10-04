
struct wifi {
    char ssid[32];
    char password[64];
    char hostname[32];
    int port;
};

struct microcontroller {
    int serial_baudrate;
};

struct transmitter {
    int pin;
    int protocol;
    int pulse_length;
};

struct api {
    char token[64];
};

struct config {
    struct wifi wifi;
    struct api api;
    struct microcontroller microcontroller;
    struct transmitter transmitter;
};