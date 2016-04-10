# DIY Self Balancing Robot
#### Click for Video:
[![Click for YouTube Video](https://github.com/HackBo/Self-Balancing-Robot/blob/master/images/anim_robot.gif)](https://youtu.be/7tfVts636bs)

#### Initial technologies and hardware:

- [IMU6050](http://www.aliexpress.com/item/MPU-6050-3-Axis-gyroscope-acce-lerometer-module-3V-5V-compatible-For-Ar/1858984311.html)
- Motor Drivers [L298N](http://www.aliexpress.com/item/Free-Shipping-1PCS-New-Dual-H-Bridge-DC-Stepper-Motor-Drive-Controller-Board-Module-L298N-for/32556583041.html)
- [DC Motors](http://www.aliexpress.com/store/product/HK-POST-FREE-Wholesale-48-1-Plastic-DC-Drive-Gear-Motor-Tyre-Tire-Wheel-For/2035033_32603795906.html)
- [PlatformIO framework](http://platformio.org/) (Open source ecosystem for IoT development)

#### Alpha Test Boards:

- NodeMCU 1.0 (ESP-12E Module) (ESP8266)
- ArduinoProMini

## Firmware Installation

1. First install PlatformIO via pip:
 
   ``` 
   $ pip install -U platformio 
   ```
   or more info on oficial site: [http://docs.platformio.org/en/latest/installation.html](http://docs.platformio.org/en/latest/installation.html)

3. Install basic libraries:

   ``` 
   $ platformio lib install 2
   ```
   
3. Clone project and external libraries:

   ```
   $ git clone https://github.com/HackBo/Self-Balancing-Robot
   $ mkdir external_libraries
   $ cd external_libraries
   $ git clone git@github.com:jrowberg/i2cdevlib.git
   $ cd ..
   ```
   
4. Compile project and deploy
   ``` 
   $ platformio run
   ```
   
## Current main config:
```
    [env:pro16MHzatmega328]
      platform = atmelavr
      framework = arduino 
      board = pro16MHzatmega328
      targets = upload
```

[![Click for YouTube Video](https://github.com/HackBo/Self-Balancing-Robot/blob/master/images/photo_robot.jpg)](https://youtu.be/7tfVts636bs)


