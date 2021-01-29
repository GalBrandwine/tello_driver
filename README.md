# tello_driver (cpp)

A **cpp** tello_driver for the DJI tello drone, that truly supports the **unofficial** SDK.
Meant to be included as a library, to other projects. See *[examples](examples)*.

## Implemented Capabilities from TelloPy

Not all TelloPy cabapilities were implemented.

### Message receiving

- [x] Parsing LOG_HEADER_MSG  
- [x] Parsing LOG_DATA_MSG  
- [ ] Parsing LOG_CONFIG_MSG. **Note: Didnt find any reason to implement this**.
- [x] Parsing WIFI_MSG
- [x] Parsing ALT_LIMIT_MSG
- [x] Parsing ATT_LIMIT_MSG
- [x] Parsing LOW_BAT_THRESHOLD_MSG.
- [ ] Parsing LIGHT_MSG. **Note: Not enough doccumentation. didnt understand what is this message**.
- [x] Parsing FLIGHT_MSG
- [x] Parsing TIME_CMD. **Note could not reproduce**.
- [ ] Capturing all ACK messages:
- [ ] Parsing TELLO_CMD_FILE_SIZE
- [ ] Parsing TELLO_CMD_FILE_DATA

### Command sending

- [x] Send conn_req **(Port 9617)**
- [x] takeoff
- [ ] throw_and_go
- [x] land
- [ ] palm_land
- [ ] get_alt_limit **Tested**
- [ ] set_alt_limit **Tested**
- [ ] get_att_limit **Tested**
- [ ] set_att_limit **Tested**
- [ ] get_low_bat_threshold **Tested**
- [ ] set_low_bat_threshold **Tested**
- [ ] __send_time_command
- [ ] __send_start_video
- [ ] __send_video_mode
- [ ] set_video_mode (With zoom)
- [ ] start_video
- [ ] set_exposure
- [ ] __send_exposure
- [ ] set_video_encoder_rate
- [ ] __send_video_encoder_rate
- [ ] take_picture
- [x] up
- [x] down
- [x] forward
- [x] backward
- [x] right
- [x] left
- [x] clockwise
- [x] counter_clockwise
- [ ] flip_forward
- [ ] flip_back
- [ ] flip_right
- [ ] flip_left
- [ ] flip_forwardleft
- [ ] flip_backleft
- [ ] flip_forwardright
- [ ] flip_backright
- [ ] __fix_range
- [ ] set_throttle
- [ ] set_yaw
- [ ] set_pitch
- [ ] set_roll
- [ ] toggle_fast_mode
- [ ] manual_takeoff
- [x] __send_stick_command
- [x] __send_ack_log
- [x] send_packet
- [x] send_packet_data

### Video receiving

- [ ] Receive video

### Packet parsing

- [ ] GetTime
- [x] AddTime
- [x] Fixup
- [x] GetBuffer
- [x] GetBufferSize
- [x] GetData // Copy the data
- [x] AddByte
- [x] AddInt
- [x] AddFloat
- [x] addInt16 // Member

## Installation

```shell
git clone https://github.com/GalBrandwine/tello_driver.git
cd tello_driver
mkdir build && cd build
cmake ..  && make -j # Or, for debugging: cmake .. -DCMAKE_BUILD_TYPE=Debug && make -j
```

## Running the examples

```shell
cd build/examples
./simple_connection
```

### Projects that help me bring up this monstrosity :)

- [tello_ros project](https://github.com/clydemcqueen/tello_ros).
- [TelloPy](https://github.com/hanyazou/TelloPy)
