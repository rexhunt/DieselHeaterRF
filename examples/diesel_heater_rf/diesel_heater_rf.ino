/*
 * 
 *    ESP32         CC1101
 *    -----         ------
 *    4   <-------> GDO2
 *    18  <-------> SCK
 *    3v3 <-------> VCC
 *    23  <-------> MOSI
 *    19  <-------> MISO
 *    5   <-------> CSn
 *    GND <-------> GND
 * 
 */

#include "../../DieselHeaterRF.h"

#define HEATER_POLL_INTERVAL  4000

#define HEATER_ADDRESS 0x6DC35C0D

uint32_t heaterAddr; // Heater address is a 32 bit unsigned int. Use the findAddress() to get your heater's address.

DieselHeaterRF heater;
heater_state_t state;

void setup() {

  Serial.begin(115200);

  heater.begin();

//Uses predetermined address if available
#ifndef HEATER_ADDRESS

  Serial.println("Started pairing, press and hold the pairing button on the heater's LCD panel...");

  heaterAddr = heater.findAddress(60000UL);
#else
  //Send heater wakeup code - Possibly not needed

  heaterAddr = HEATER_ADDRESS;
  Serial.print("Heater address preset to: ");
  Serial.println(heaterAddr, HEX);
#endif

  if (heaterAddr) {
    Serial.print("Got address: ");
    Serial.println(heaterAddr, HEX);
    heater.setAddress(heaterAddr);
    // Store the address somewhere, eg. NVS
  } else {
    Serial.println("Failed to find a heater");   
    Serial.println("Looping to listen, no action taken");  
    char buf[26]; 
    while(1) {
          heaterAddr = heater.findAddress(60000UL);
      }
  }
  
}

void loop() {

  heater.sendCommand(HEATER_CMD_WAKEUP);

  if (heater.getState(&state)) {
    Serial.printf("State: %d, Power: %d, Voltage: %f, Ambient: %d, Case: %d, Setpoint: %d, PumpFreq: %f, Auto: %d, RSSI: %d\n", state.state, state.power, state.voltage, state.ambientTemp, state.caseTemp, state.setpoint, state.pumpFreq, state.autoMode, state.rssi); 
  } else {
    Serial.println("Failed to get the state");
  }
  
  delay(HEATER_POLL_INTERVAL);

}
