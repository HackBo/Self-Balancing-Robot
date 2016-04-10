# DIY Self Balancing Robot

Self Balancing Robot DIY Test.

### initial technologies and hardware:

- [IMU6050](http://www.aliexpress.com/item/MPU-6050-3-Axis-gyroscope-acce-lerometer-module-3V-5V-compatible-For-Ar/1858984311.html?spm=2114.30010308.3.1.tzbiPF&ws_ab_test=searchweb201556_9,searchweb201602_4_301_10034_507_10032_508_10020_10017_10005_10006_10021_10022_401_10018_10019,searchweb201603_7&btsid=1335747b-7b51-44ff-9882-e08fa010ac81)
- Motor Drivers [L298N](http://www.aliexpress.com/item/Free-Shipping-1PCS-New-Dual-H-Bridge-DC-Stepper-Motor-Drive-Controller-Board-Module-L298N-for/32556583041.html?spm=2114.30010308.3.1.5qNg6m&ws_ab_test=searchweb201556_9,searchweb201602_4_301_10034_507_10032_508_10020_10017_10005_10006_10021_10022_401_10018_10019,searchweb201603_7&btsid=8acba5b8-b4ac-45d7-8e2c-14282646cf26)
- [PlatformIO framework](http://platformio.org/) (open source ecosystem for IoT development)

### Alpha Test Boards:
- nodemcuv2 env NodeMCU 1.0 (ESP-12E Module) (ESP8266 80MHz, 4096Kb, 80Kb)
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
      platform = espressif
      framework = arduino 
      board = pro16MHzatmega328
      targets = upload
```

