
#include "mbed.h"
#include "LSM6DSLSensor.h"

#define PI 3.141592654 // global definition for PI

static DevI2C devI2c(PB_11,PB_10);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address


float computeAngle(int x, int y, int z, int type){ // Returning type float as char was not converting negative numbers correctly
    float res = 0.0; // return value
    float pitch_rad = 0.0; // temp variable for result of Pitch and Roll formulas. value will be in radians
    float roll_rad = 0.0; 

    int x_2 = pow(x, 2);
    int y_2 = pow(y, 2);
    int z_2 = pow(z, 2);   

    if(type == 0){
        pitch_rad = atan(x / ( // Pitch formula
                            sqrt( y_2 + z_2)
                        )
                    );
        res = pitch_rad * 180/PI; // Pitch in degrees
        // printf("Pitch: %3.2f\r\n", res); // Print information to terminal
    } else if (type == 1){
        roll_rad = atan(y / (  // Roll formula
                            sqrt( x_2 + z_2)
                        )
                    );
        res = roll_rad * 180/PI; // Roll in degrees
        // printf("Roll: %3.2f\r\n", res); // Print information to terminal
    }

    return res; // return specified value in degrees
}

int main() {
    uint8_t id;
    int32_t axes[3];
    float pitch = 0.0;
    float roll = 0.0;
    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while(true) {

        acc_gyro.get_x_axes(axes);
        pitch = computeAngle(axes[0], axes[1], axes[2], 0); // calling computeAngle, passing 0 as type to get Pitch
        roll = computeAngle(axes[0], axes[1], axes[2], 1);  // calling computeAngle, passing 1 as type to get Roll
        printf("LSM6DSL: %6d, %6d, %6d, %3.2f, %3.2f\r\n", axes[0], axes[1], axes[2], pitch, roll); // Print information to terminal

        thread_sleep_for(2000);

    }
}