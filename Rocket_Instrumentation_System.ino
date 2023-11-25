// Feather m0 adalogger
// ACCEL: SCL - SCL, SDA -SDA, GRD, Vin
// PRESSURE: SCL -> SCK, SDA -> SDI, GRD, Vin
// BUZZER: (+) -> pin 10, (- to ground)
// vdiv: +9v -> large R -> small R -> GRD.
// between resistors: connect to A3
// uses 20% of Arduino f. M0 storage when last checked.
// set to delete files on rerun. OFF
// wait for serial? OFF
// remove delete file? OFF
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
// pressure
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
//Main data variables.
const int chipSelect = 4; // chip select pin. soldered on feather, so cannot change.
float accelx;
float accely;
float accelz;
float pres;
float tempCels;
float pHeight;
float timePoint; // non zeroed time.
float counter; // loop counter
float timeold = 0; // stores time from prev loop.
File file; // creates file object 'file', allows read/write to file.
// =================================================================================
// buzzer script. currently set to pin 10.
float finterval = 20; // in seconds, starting time
float fintervalbetw = 15; // in seconds, time between interval
unsigned char frunscript = 0; // logical bit, script runs and sets to off afterwards. // if shorter than song time, will stack stopping accidental cutoff.
int findexer = 0; // array indexer, iterates though notes.
long ftune[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0}; // football tune. two digits represents a note.
int farraysize = 32;
int fnotelength = 50; // interval between note changes (ms, up to 32000, change to long for more.).
unsigned long fprevtime = 0; // do not change (ms)
int fLED_pin = 10;
void buzz() {
// timer between scripts
if ((finterval*1000) < millis()){
frunscript++;
finterval = finterval + fintervalbetw;
// play tune, if script is to run
}
if (frunscript > 0){
//Serial.println("past run");
if (findexer < farraysize){ // indexer starts at zero, so last one is at one less than the size of the array.
// add millis 'delay' timer script here. integrate into loop below?
//Serial.println("past array sizer");
if (millis()>fprevtime) {
fprevtime = millis() + fnotelength;
//Serial.println("past millis");
if (ftune[findexer] == 1){
digitalWrite(fLED_pin, HIGH);
Serial.println("oon");
//Serial.print("direct analog output of divider: "); Serial.println(measuredvbatraw);
}
else{
digitalWrite(fLED_pin, LOW);
Serial.println("oof");
}
findexer++;
}
}
else { // resets.
findexer = 0;
frunscript--;
Serial.print("set index and runscript to zero");
}
}
}
// battery measure script USES PIN 9 AND 13 (INTERNAL VDIVIDER, INTERNAL LED) [[3v7 batt]]
#define VBATPIN A7 // (pin 9)
float brawa; // raw analog reading of battery vdivider
int bpercent50 = 574; // analog battery percentage >50%
int bpercent25 = 551; // analog battery percentage >25%
int binterval = 10; // in seconds, time before first battery led script run.
int bintervalbetw = 10; // in seconds, time between battery led script playing.
unsigned char brunscript = 0; // logical bit, overrides timer to allow script to run.
int bindexer = 0; // array indexer, iterates though notes.
int bledtune50[] = {1, 0, 1, 0, 1, 0, 1, 0}; // >50%
int bledtune25[] = {1, 0, 1, 0, 1, 0, 0, 0}; // >25%
int bledtune00[] = {1, 0, 1, 0, 0, 0, 0, 0}; // <25%
int barraysize = 8;
int bnotelength = 200; // interval between note changes (ms, up to 32000, change to long for more.).
unsigned long bprevtime = 0; // do not change (ms)
int bLED_pin = 13; // internal LED.
void bat() {
brawa = analogRead(VBATPIN); // measures batt pin voltage
if ((binterval*1000) < millis()){ // timer between scripts
Serial.print(brawa); Serial.print(" "); Serial.println(brawa*6.6/1024);
brunscript++; // (adds one)
binterval = binterval + bintervalbetw;
}
// play tune, if script is to run
if (brunscript >0 ){
if (bindexer < barraysize){ // indexer starts at zero, so last one is at one less than the size of the array.
// add millis 'delay' timer script here. integrate into loop below?
if (millis()>bprevtime) {
bprevtime = millis() + bnotelength;
if (brawa > bpercent50 && bledtune50[bindexer] == 1){
digitalWrite(bLED_pin, HIGH);
Serial.println("high1");
//Serial.println("tune1, b50");
//Serial.print("direct analog output of divider: "); Serial.println(measuredvbatraw);
}
else if (brawa > bpercent25 && bledtune25[bindexer] == 1) {
digitalWrite(bLED_pin, HIGH);
Serial.println("high2");
}
else if (brawa <= bpercent25 && bledtune00[bindexer] == 1) {
digitalWrite(bLED_pin, HIGH);
Serial.println("high3");
}
else{
digitalWrite(bLED_pin, LOW);
Serial.println("low");
}
bindexer++;
}
}
else { // resets.
bindexer = 0;
brunscript--; // (takes off one)
//Serial.print("set index and runscript to zero"); Serial.println(runscript);
}
}
}
//9v battery level detector
// battery measure script USES PIN A3 AND 8 (INTERNAL LED)
#define V2BATPIN A3
float b2rawa; // raw analog reading of battery vdivider
int b2percent50 = 594; // analog battery percentage >50%
int b2percent25 = 524; // analog battery percentage >25%
int b2interval = 10; // in seconds, time before first battery led script run.
int b2intervalbetw = 10; // in seconds, time between battery led script playing.
unsigned char b2runscript = 0; // logical bit, overrides timer to allow script to run.
int b2indexer = 0; // array indexer, iterates though notes.
int b2ledtune50[] = {1, 0, 1, 0, 1, 0, 1, 0}; // >50%
int b2ledtune25[] = {1, 0, 1, 0, 1, 0, 0, 0}; // >25%
int b2ledtune00[] = {1, 0, 1, 0, 0, 0, 0, 0}; // <25% /// overlaps with 3v7 so could use same...
int b2arraysize = 8;
int b2notelength = 200; // interval between note changes (ms, up to 32000, change to long for more.).
unsigned long b2prevtime = 0; // do not change (ms)
int b2LED_pin = 8; // Internal LED
void bat2() {
b2rawa = analogRead(V2BATPIN); // measures 9v batt pin voltage
if ((b2interval*1000) < millis()){ // timer between scripts
Serial.print(b2rawa); Serial.print(" "); Serial.println((b2rawa*0.01321));// calculated usin measured resistor values and ~9.7V as peak.
b2runscript++; // (adds one)
b2interval = b2interval + b2intervalbetw;
}
// play tune, if script is to run
if (b2runscript >0 ){
if (b2indexer < b2arraysize){ // indexer starts at zero, so last one is at one less than the size of the array.
// add millis 'delay' timer script here. integrate into loop below?
if (millis()>b2prevtime) {
b2prevtime = millis() + b2notelength;
if (b2rawa > b2percent50 && b2ledtune50[b2indexer] == 1){
digitalWrite(8, HIGH);
Serial.println("2high1");
//Serial.println("tune1, b50");
//Serial.print("direct analog output of divider: "); Serial.println(measuredvbatraw);
}
else if (b2rawa > b2percent25 && b2ledtune25[b2indexer] == 1) {
digitalWrite(8, HIGH);
Serial.println("2high2");
}
else if (b2rawa <= b2percent25 && b2ledtune00[b2indexer] == 1) {
digitalWrite(8, HIGH);
Serial.println("2high3");
}
else{
digitalWrite(8, LOW);
Serial.println("2low");
}
b2indexer++;
}
}
else { // resets.
b2indexer = 0;
b2runscript--; // (takes off one)
//Serial.print("set index and runscript to zero"); Serial.println(runscript);
}
}
}
void setup(void)
{
#ifndef ESP8266
// while (!Serial); // for Leonardo/Micro/Zero
#endif
Serial.begin(9600); // opens serial communications for debugging/live output
// initialise output pins.
pinMode(fLED_pin, OUTPUT);
digitalWrite(fLED_pin, LOW);
pinMode(b2LED_pin, OUTPUT);
//-------------------------------------------------------------------- Next stuff I put at functions at the end.
//SD CARD
SDCARDsetup();
// initialise, setup, display info for acceleration. (to serial) -> change settings below
setuptheaccel();
// initialise and test if pressure/temp widget is functioning. (to serial) -> change settings below
// DOES NOT FUNCTION UNTILL SERIAL CONNECTED (unless disabled)
setupthepressure();
}
// ===========================================================================================================
void loop(void)
{
// times loop.
Serial.println(counter++); //counted loops
Serial.println(millis()-timeold);
timeold = millis();
updateData(); // gets new data and updates variables with it. could remove variables and write directly to file.
writeToFile(); //writes variables to a line in the file.
writeToSerial(); //writes variables to serial.
buzz(); //buzzer script
bat(); // battery 3v7 script
bat2(); // battery 9v script
}
// ====================================================================================================== LOOP FUNCTIONS
void updateData(void)
{
/* Get a new sensor event: accel then pressure/temp */
sensors_event_t event;
accel.getEvent(&event);
sensors_event_t temp_event, pressure_event;
bmp_temp->getEvent(&temp_event);
bmp_pressure->getEvent(&pressure_event);
// update variables. Units: m/s^2, degrees C, hPa, m.
timePoint = millis(); // delay between this event and the actual data collection? probably not the largest source of error.
accelx = event.acceleration.x;
accely = event.acceleration.y;
accelz = event.acceleration.z;
tempCels = temp_event.temperature;
pres = pressure_event.pressure;
pHeight = bmp.readAltitude(1013.25); /* NEED TO ADJUST TO LOCAL FORCAST! */
}
void writeToFile(void)
{
// write to SD card. could write function to squish this to one line in if statement.
file = SD.open("datafile.txt", FILE_WRITE);
if (file) {
file.print(accelx); file.print(" ");
file.print(accely); file.print(" ");
file.print(accelz); file.print(" ");
file.print(timePoint/1000, 3); file.print(" ");
file.print(tempCels); file.print(" ");
file.print(pres); file.print(" ");
file.println(pHeight);
file.close(); // good practice. can only write to one at a time.
} else {
Serial.println("error occured opening file when writing data");
}
}
void writeToSerial()
{
/* Display the results to serial */ //ADD to SD function as debug line that can be cut out.
Serial.print("X: "); Serial.print(accelx); Serial.print(" ");
Serial.print("Y: "); Serial.print(accelz); Serial.print(" ");
Serial.print("Z: "); Serial.print(accely); Serial.print(" ");Serial.println("m/s^2 ");
Serial.print(timePoint/1000, 3); Serial.println(" ");
Serial.print(F("Temperature = ")); Serial.print(tempCels); Serial.println(" *C");
Serial.print(F("Pressure = ")); Serial.print(pres); Serial.println(" hPa");
Serial.print(F("Approx altitude = "));
Serial.print(pHeight); /* Adjusted to local forecast! */
Serial.println(" m");
Serial.println();
}
//=========================================================== SETUP and global funcs below
//------------------------------------------------------------------- from library
// SD card, setup and check. Pauses code if not turned on.
void SDCARDsetup() {
pinMode(chipSelect, OUTPUT);
// test sd card // STOPS SCRIPT IF NOT DETECTED
Serial.print("Initializing SD card...");
// see if the card is present and can be initialized: //WILL STOP SCRIPT
if (!SD.begin(chipSelect)) {
Serial.println("Card failed, or not present");
// don't do anything more:
while (1);
}
Serial.println("card initialized.");
// ------------------------------------------ //I wrote this next bit (loosley following tutorial):
// delete current file: unbackslash this if you want to keep it on reboot.
//SD.remove("datafile.txt");
// file exists alredy test. if it does, writes a line to separate data.
if(SD.exists("datafile.txt")) {
Serial.println("file exists already, be careful to not stack data");
file = SD.open("datafile.txt", FILE_WRITE);
file.println("________________ SETUP REPEATED, new data starts here:");
file.println("Accelx Accely Accelz timeStampR tempCels pressure pheight"); // maybe add units?
file.close();
} else { // creates file and titles each column.
Serial.println("created new file");
file = SD.open("datafile.txt", FILE_WRITE);
file.println("Accelx Accely Accelz timeStampR tempCels pressure pheight"); // maybe add units?
file.close();
}
}
// sensor details for accelerometer, setup
void displaySensorDetails(void)
{
sensor_t sensor;
accel.getSensor(&sensor);
Serial.println("------------------------------------");
Serial.print ("Sensor: "); Serial.println(sensor.name);
Serial.print ("Driver Ver: "); Serial.println(sensor.version);
Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
Serial.println("------------------------------------");
Serial.println("");
delay(500);
}
//-----------------------------------------------------------------------------
// accel below
void displayDataRate(void)
{
Serial.print ("Data Rate: ");
switch(accel.getDataRate())
{
case ADXL345_DATARATE_3200_HZ:
Serial.print ("3200 ");
break;
case ADXL345_DATARATE_1600_HZ:
Serial.print ("1600 ");
break;
case ADXL345_DATARATE_800_HZ:
Serial.print ("800 ");
break;
case ADXL345_DATARATE_400_HZ:
Serial.print ("400 ");
break;
case ADXL345_DATARATE_200_HZ:
Serial.print ("200 ");
break;
case ADXL345_DATARATE_100_HZ:
Serial.print ("100 ");
break;
case ADXL345_DATARATE_50_HZ:
Serial.print ("50 ");
break;
case ADXL345_DATARATE_25_HZ:
Serial.print ("25 ");
break;
case ADXL345_DATARATE_12_5_HZ:
Serial.print ("12.5 ");
break;
case ADXL345_DATARATE_6_25HZ:
Serial.print ("6.25 ");
break;
case ADXL345_DATARATE_3_13_HZ:
Serial.print ("3.13 ");
break;
case ADXL345_DATARATE_1_56_HZ:
Serial.print ("1.56 ");
break;
case ADXL345_DATARATE_0_78_HZ:
Serial.print ("0.78 ");
break;
case ADXL345_DATARATE_0_39_HZ:
Serial.print ("0.39 ");
break;
case ADXL345_DATARATE_0_20_HZ:
Serial.print ("0.20 ");
break;
case ADXL345_DATARATE_0_10_HZ:
Serial.print ("0.10 ");
break;
default:
Serial.print ("???? ");
break;
}
Serial.println(" Hz");
}
void displayRange(void)
{
Serial.print ("Range: +/- ");
switch(accel.getRange())
{
case ADXL345_RANGE_16_G:
Serial.print ("16 ");
break;
case ADXL345_RANGE_8_G:
Serial.print ("8 ");
break;
case ADXL345_RANGE_4_G:
Serial.print ("4 ");
break;
case ADXL345_RANGE_2_G:
Serial.print ("2 ");
break;
default:
Serial.print ("?? ");
break;
}
Serial.println(" g");
}
// SETUP FOR ACCELEROMETER
void setuptheaccel(void)
{
// accel
Serial.println("Accelerometer Test"); Serial.println("");
/* Initialise the sensor */
if(!accel.begin())
{
/* There was a problem detecting the ADXL345 ... check your connections */
Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
while(1);
}
/* Set the range to whatever is appropriate for your project */
//accel.setRange(ADXL345_RANGE_16_G);
// accel.setRange(ADXL345_RANGE_8_G);
accel.setRange(ADXL345_RANGE_4_G);
// accel.setRange(ADXL345_RANGE_2_G);
/* Display some basic information on this sensor */
displaySensorDetails();
/* Display additional settings (outside the scope of sensor_t) */
displayDataRate();
displayRange();
Serial.println("");
}
//--------------------------------------------------------------------------------
// BAROMETER AND THERMOMETER
void setupthepressure(void)
{
//pressure sensor
//while ( !Serial ) delay(100); // wait for native usb
Serial.println(F("BMP280 Sensor event test"));
unsigned status;
//status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
status = bmp.begin();
if (!status) {
Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
"try a different address!"));
Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
Serial.print(" ID of 0xFF probably means a bad address, a BMP 180 or BMP085\n");
Serial.print(" ID of 0x56-0x58 represents a BMP 280,\n");
Serial.print(" ID of 0x60 represents a BME 280.\n");
Serial.print(" ID of 0x61 represents a BME 680.\n");
while (1) delay(10);
}
/* Default settings from datasheet. */
bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
Adafruit_BMP280::FILTER_X16, /* Filtering. */
Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
bmp_temp->printSensorDetails();
}