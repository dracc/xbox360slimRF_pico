#include <pico/stdlib.h>
#include <pico/time.h>
#include <vector>

#define bit(x) (uint32_t(1 << x))

#define SYNC_PIN 5
#define DATA_PIN 6
#define CLOCK_PIN 7

//Start Module (Needed for the sync led animation)
const std::vector<bool> start_cmd = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
//Makes the startup animation on the ring of light.
const std::vector<bool> power_cmd = {0, 0, 1, 0, 0, 0, 0, 1, 0, 1};
// Initiates the sync process.
const std::vector<bool> sync_cmd = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1};

void send_data(std::vector<bool> data) {
  int prev = 1;
  gpio_put(DATA_PIN, false);

  for (bool a: data) {
    while (prev == gpio_get(CLOCK_PIN)) {}
    prev = 0;

    sleep_us(1000);
    gpio_put(DATA_PIN, a);
    
    while (prev == gpio_get(CLOCK_PIN)) {}
    prev = 1;
  }
  gpio_put(DATA_PIN, true);
}

void pin_setup() {
  // Initialize GPIO
  gpio_init_mask(bit(SYNC_PIN) | bit(DATA_PIN) | bit(CLOCK_PIN));

  // Set up inputs
  gpio_pull_up(SYNC_PIN);
  gpio_pull_up(CLOCK_PIN);

  // Set up output
  gpio_set_dir(DATA_PIN, true);
  gpio_put(DATA_PIN, true);
}
    
// This only rolls the startup animations, but looks cool I guess
void startup_seq() {
  send_data(start_cmd);
  sleep_ms(50);
  send_data(power_cmd);
  sleep_ms(50);
}
    
int main() {
  pin_setup();

  // Wait for the RF board to be ready
  sleep_ms(1000);
  startup_seq();

  // Check for a button press every 100 ms
  for (;;) {
    sleep_ms(100);
    if (gpio_get(SYNC_PIN) == false) {
      send_data(sync_cmd);
      sleep_ms(3000);
    }
  } 
  return 0;
}
