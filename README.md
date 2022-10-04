# ESP32 433API
This projekt links an API to a 433MHz transmitter. This is based on the [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview).

## Hardware
- ESP32
- 433MHz transmitter

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
        "ssid": "SSID",
        "password": "PASSWORD",
        "hostname": "433mhz-api-bridge",
        "port": 80
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