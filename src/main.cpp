#ifdef IMULIGHT

#include "src_imulight/main.cpp"

#endif

#ifdef ADA_IMU
#include "src_adaimu/main.cpp"
#endif

#ifdef IMU
#include "src_imu/main.cpp"
#endif

#ifdef BMP280LIGHT
#include "src_bmplight/main.cpp"
#endif

#ifdef ADA_BMP280
#include "src_adabmp/main.cpp"
#endif

#ifdef ADA_BMP_IMU
#include "src_ada_bmp_imu/main.cpp"
#endif

#ifdef RFM69
#include "src_rfm69/main.cpp"
#endif

#ifdef RFM69_STATION
#include "src_station/main.cpp"
#endif

#ifdef DFPLAYER
#include "src_dfplayer/main.cpp"
#endif

#ifdef SERVO
#include "src_servo/main.cpp"
#endif

#ifdef LEDS
#include "src_leds/main.cpp"
#endif
