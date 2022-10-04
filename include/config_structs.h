
struct wifi {
    String ssid;
    String password;
    String hostname;
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
    String token;
};

struct config {
    struct wifi wifi;
    struct api api;
    struct microcontroller microcontroller;
    struct transmitter transmitter;
};