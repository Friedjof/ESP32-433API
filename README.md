# ESP32 433API
This projekt links an API to a 433MHz transmitter. This is based on the [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview).

## Hardware
- ESP32
- 433MHz transmitter

## Endpoints
### `/`
- GET: returns the WiFi status
### `/styles`
- GET: returns the styles for the web interface
### `/logs`
- GET: returns the logs
   - `?key=your-api-key`: returns the logs
   - `?clear`: clears the logs
### `/config`
- GET: set or return the config
   - `?ssid=your-ssid&password=your-password`: set the WiFi config
   - `?show`: returns the config
### `/transmit`
- GET: transmit a code
   - `?key=your-api-key`: authorization
   - `&msg=your-message`: the message to transmit
   - `type=your-type`: the type of the message (`hex` & `bin` are supported)

## Software
- [PlatformIO](https://platformio.org/)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- […]

## Development
### Setup
Visual Studio Code with PlatformIO
  - install PlatformIO

Move directory data-sample to data
```bash
mv data-sample data
```

Edit data/config.json
```json
{
    "wifi": {
        "ssid": "myssid",
        "password": "mypassword",
        "hostname": "your-hostname",
        "port": 80
    },
    "microcontroller": {
        "serial_baudrate": 9600
    },
    "transmitter": {
        "pin": 27,
        "protocol": 1,
        "pulse_length": 500
    },
    "api": {
        "key": "your-api-key"
    }
}
```

### Build filesystem and upload
```bash
pio run -t buildfs && pio run -t uploadfs
```

### Build and upload
```bash
pio run -t upload
```

### Monitor
```bash
pio device monitor
```

## License
[MIT](LICENSE)

## Author
[Friedjof Noweck](https://github.com/Friedjof)