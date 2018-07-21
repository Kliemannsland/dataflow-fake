#include <Adafruit_NeoPixel.h>

//Set Data-Out-Pin
#define PIN TX

//Set amount of pixels
#define NUM_PIXELS 250

//Set frequency
#define RAND_UP 30
#define RAND_DOWN 900

//Set speed
#define SPEED 10

const int UP = 1;
const int DOWN = 2;
const int BOTH = 3;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
int LED_STATS[NUM_PIXELS];

void setup() {
  strip.begin();
  clearStrip();
  randomSeed(analogRead(0));
  delay(1000);
}


void loop() {
  generate();
  prepare();
  strip.show();
  move();
  delay(SPEED);
}

void generate() {
  if (random(0, RAND_UP) == RAND_UP - 1) add(UP);
  if (random(0, RAND_DOWN) == RAND_DOWN - 1) add(DOWN);
}

void prepare() {
  for (int count = 0; count < NUM_PIXELS; count++) {
    if(LED_STATS[count] == UP) {
      strip.setPixelColor(count, 0, 0, 100);
    } else if(LED_STATS[count] == DOWN) {
      strip.setPixelColor(count, 100, 0, 0);
    } else if(LED_STATS[count] == BOTH) {
      strip.setPixelColor(count, 100, 0, 100);
    } else {
      strip.setPixelColor(count, 0, 0, 0);
    }
  }
}

void move() {
  int tmp[NUM_PIXELS];

  for (int i = 0; i < NUM_PIXELS; i++) {
    if (LED_STATS[i] == UP) {
      tmp[i+1] = UP;
      tmp[i] = 0;
    }

    if (LED_STATS[i] == DOWN) {
      if (tmp[i] != UP) tmp[i] = 0;
      if (tmp[i-1] == UP) {
        tmp[i-1] = BOTH; 
      } else {
        tmp[i-1] = DOWN;
      }
    }

    if (LED_STATS[i] == BOTH) {
      tmp[i] = 0;
      tmp[i-1] = DOWN;
      tmp[i+1] = UP;
    }
  }

  memcpy(LED_STATS, tmp, NUM_PIXELS * sizeof(int) );
}

void add(int type) {
  if (type == UP) LED_STATS[0] = UP;
  if (type == DOWN) LED_STATS[NUM_PIXELS-1] = DOWN;
}

void clearStrip() {
  for (int i = 0; i < NUM_PIXELS; i++){
    LED_STATS[i] = 0;
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  }
}
