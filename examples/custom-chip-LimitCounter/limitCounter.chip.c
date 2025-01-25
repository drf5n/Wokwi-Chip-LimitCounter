// Counting inverter
// Copied from https://wokwi.com/projects/327458636089524820
// For information and examples see:
// https://docs.wokwi.com/chips-api/getting-started
// See also https://github.com/wokwi/inverter-chip
// 
// Original by Uri Shaked, counter mods by David Forrest
/*
This is intended to add some limit switch ability for steppers

*/

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_count;  // count falling edges
  pin_t pin_up;     // up/not_down
  pin_t pin_no_a;
  pin_t pin_nc_a;
  pin_t pin_no_b;
  pin_t pin_nc_b;
  int32_t thresholdLow;
  int32_t thresholdHigh;
  bool lowTriggered;
  bool highTriggered;

  uint32_t Lower_attr; // control
  uint32_t lower_attr; // diagram.json
  uint32_t Upper_attr; // control
  uint32_t upper_attr; // diagram.json

} chip_state_t;

int32_t count = 0;

static void set_outputs(void *user_data){
  chip_state_t *chip = (chip_state_t*)user_data;
  chip->highTriggered = count > chip->thresholdHigh ;
  chip->lowTriggered = count < chip->thresholdLow ;

  pin_write(chip->pin_nc_b,!chip->highTriggered);
  pin_write(chip->pin_no_b,chip->highTriggered);
  pin_write(chip->pin_nc_a,!chip->lowTriggered);
  pin_write(chip->pin_no_a,chip->lowTriggered); 
}

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  if(value==0){ // count falling
    chip_state_t *chip = (chip_state_t*)user_data;
      chip->thresholdLow = attr_read_float(chip->lower_attr);
  chip->thresholdHigh = attr_read_float(chip->upper_attr);

    if(pin_read(chip->pin_up)==HIGH){ // count upwards
      ++count;
      if(chip->highTriggered == false && count > chip->thresholdHigh){
        chip->highTriggered = true;
        pin_write(chip->pin_nc_b,!chip->highTriggered);
        pin_write(chip->pin_no_b,chip->highTriggered);
      }
      if(chip->lowTriggered == true && count > chip->thresholdLow){
        chip->lowTriggered = false;
        pin_write(chip->pin_nc_a,!chip->lowTriggered);
        pin_write(chip->pin_no_a,chip->lowTriggered);
      }
    } else {
      --count;
      if(chip->highTriggered == true && count < chip->thresholdHigh){
        chip->highTriggered = false;
        pin_write(chip->pin_nc_b,!chip->highTriggered);
        pin_write(chip->pin_no_b,chip->highTriggered);
      }
      if(chip->lowTriggered == false && count < chip->thresholdLow){
        chip->lowTriggered = true;
        pin_write(chip->pin_nc_a,!chip->lowTriggered);
        pin_write(chip->pin_no_a,chip->lowTriggered);
      }
    }
    printf("Pin change: %d %d %d\n", chip->thresholdLow ,count, chip->thresholdHigh);
  }
}

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_count = pin_init("COUNT", INPUT);
  chip->pin_no_a = pin_init("NO_LO", OUTPUT);
  chip->pin_nc_a = pin_init("NC_LO", OUTPUT);
  chip->pin_no_b = pin_init("NO_HI", OUTPUT);
  chip->pin_nc_b = pin_init("NC_HI", OUTPUT);
  chip->pin_up = pin_init("UP", INPUT_PULLUP);
  chip->pin_count = pin_init("COUNT", INPUT_PULLUP);

  // read controls
  chip->lower_attr = attr_init_float("lower", -10);
  chip->upper_attr = attr_init_float("upper", 10);
  chip->thresholdLow = attr_read_float(chip->lower_attr);
  chip->thresholdHigh = attr_read_float(chip->upper_attr);

  //attr_init("lower", 0);
 // attr_init("upper", 10);

  set_outputs(chip);
//  pin_write(chip->pin_nc_a,!chip->highTriggered);
//  pin_write(chip->pin_no_a,chip->highTriggered);
//  pin_write(chip->pin_nc_b,!chip->lowTriggered);
//  pin_write(chip->pin_no_b,chip->lowTriggered);


  const pin_watch_config_t config = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->pin_count, &config);  
}
