/*
   Arduino Sketch to detect pulses from two PulseSensors.

   Here is a link to the tutorial
   https://pulsesensor.com/pages/two-or-more-pulse-sensors

   Copyright World Famous Electronics LLC - see LICENSE
   Contributors:
     Joel Murphy, https://pulsesensor.com
     Yury Gitman, https://pulsesensor.com
     Bradford Needham, @bneedhamia, https://bluepapertech.com

   Licensed under the MIT License, a copy of which
   should have been included with this software.

   This software is not intended for medical use.
*/

/*
   Every Sketch that uses the PulseSensor Playground must
   define USE_ARDUINO_INTERRUPTS before including PulseSensorPlayground.h.
   Here, #define USE_ARDUINO_INTERRUPTS false tells the library to
   not use interrupts to read data from the PulseSensor.

   If you want to use interrupts, simply change the line below
   to read:
     #define USE_ARDUINO_INTERRUPTS true

   Set US_PS_INTERRUPTS to false if either
   1) Your Arduino platform's interrupts aren't yet supported
   by PulseSensor Playground, or
   2) You don't wish to use interrupts because of the side effects.

   NOTE: if US_PS_INTERRUPTS is false, your Sketch must
   call pulse.sawNewSample() at least once every 2 milliseconds
   to accurately read the PulseSensor signal.
*/
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>


/*
   The format of our output.

   Set this to PROCESSING_VISUALIZER if you're going to run
    the multi-sensor Processing Visualizer Sketch.
    See https://github.com/WorldFamousElectronics/PulseSensorAmped_2_Sensors

   Set this to SERIAL_PLOTTER if you're going to run
    the Arduino IDE's Serial Plotter.
*/
const int OUTPUT_TYPE = SERIAL_PLOTTER;

/*
   Number of PulseSensor devices we're reading from.
*/
const int PULSE_SENSOR_COUNT = 1;

/*
     PULSE_POWERx = the output pin that the red (power) pin of
      the first PulseSensor will be connected to. PulseSensor only
      draws about 4mA, so almost any micro can power it from a GPIO.
      If you don't want to use pins to power the PulseSensors, you can remove
      the code dealing with PULSE_POWER0 and PULSE_POWER1.
     PULSE_INPUTx = Analog Input. Connected to the pulse sensor
      purple (signal) wire.
     PULSE_BLINKx = digital Output. Connected to an LED (must have at least
      470 ohm resistor) that will flash on each detected pulse.
     PULSE_FADEx = digital Output. PWM pin onnected to an LED (must have
      at least 470 ohm resistor) that will smoothly fade with each pulse.

     NOTE: PULSE_FADEx must be pins that support PWM.
       If USE_INTERRUPTS is true, Do not use pin 9 or 10 for PULSE_FADEx
       because those pins' PWM interferes with the sample timer.
*/
const int PULSE_INPUT0 = 0;
const int PULSE_BLINK0 = LED_BUILTIN;
const int PULSE_FADE0 = 5;

const int PULSE_INPUT1 = 1;
const int PULSE_BLINK1 = 12;
const int PULSE_FADE1 = 11;

const int THRESHOLD = 400;   // Adjust this number to avoid noise when idle

/*
   samplesUntilReport = the number of samples remaining to read
   until we want to report a sample over the serial connection.

   We want to report a sample value over the serial port
   only once every 20 milliseconds (10 samples) to avoid
   doing Serial output faster than the Arduino can send.
*/
byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;

/*
   All the PulseSensor Playground functions.
   We tell it how many PulseSensors we're using.
*/
PulseSensorPlayground pulseSensor(PULSE_SENSOR_COUNT);

void setup() {
  /*
     Use 250000 baud because that's what the Processing Sketch expects to read,
     and because that speed provides about 25 bytes per millisecond,
     or 50 characters per PulseSensor sample period of 2 milliseconds.

     If we used a slower baud rate, we'd likely write bytes faster than
     they can be transmitted, which would mess up the timing
     of readSensor() calls, which would make the pulse measurement
     not work properly.
  */
  Serial.begin(9600);

  /*
     Configure the PulseSensor manager,
     telling it which PulseSensor (0 or 1)
     we're configuring.
  */

  pulseSensor.analogInput(PULSE_INPUT0);

  pulseSensor.analogInput(PULSE_INPUT1);


  pulseSensor.setSerial(Serial);
  pulseSensor.setThreshold(THRESHOLD);

  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
    /*
       PulseSensor initialization failed,
       likely because our Arduino platform interrupts
       aren't supported yet.

       If your Sketch hangs here, try changing USE_ARDUINO_INTERRUPTS to false.
    */
  }
}
String output_val;
char temp_str[25];
char temp_str2[25];
int my_BPM = 1;
char separator = ',';
char curr_char = 'K';

void loop() {

  if (pulseSensor.sawNewSample()) {

    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      /*
         If a beat has happened on a given PulseSensor
         since we last checked, write the per-beat information
         about that PulseSensor to Serial.
      */

      output_val = pulseSensor.getLatestSample();
      output_val.toCharArray(temp_str, sizeof(temp_str));
      char *p = temp_str;
      char *str;
      int count = 0;
      while ((str = strtok_r(p, ",", &p)) != NULL && count == 0) {
        my_BPM = atoi(str);
        count++;
      }

      Serial.println("test_val: " + my_BPM);
      if (my_BPM < 30 || my_BPM > 190) {
         delay(3000);
         output_val = pulseSensor.getLatestSample();
         output_val.toCharArray(temp_str2, sizeof(temp_str2));
         char *p2 = temp_str2;
         char *str2;
         int count2 = 0;
         while ((str2 = strtok_r(p2, ",", &p2)) != NULL && count2 == 0) {
          my_BPM = atoi(str2);
          count2++;
         }
        if (my_BPM < 40 || my_BPM > 190) {
          my_BPM = 0;
        }
         Serial.println(my_BPM);
      } else {
       Serial.println(my_BPM);
      }
    }
      delay(100);
  }
}

  /******
     Don't add code here, because it could slow the sampling
     from the PulseSensor.
  ******/

