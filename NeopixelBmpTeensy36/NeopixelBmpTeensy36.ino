#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD_t3.h>
#include <SD.h>

File bmpImage, textFile, settingsFile;
int32_t width, height;
const int chipSelect = BUILTIN_SDCARD;

const int numpixels = 30;
const int pin = 15;
const int brightness = 96;
const int speed = 5; //delay in milliseconds


Adafruit_NeoPixel strip = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1); // <- this is how you should block execution, not with returns
  }
  Serial.println("initialization done.");

  strip.setBrightness(brightness);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.println("done!");

}

void loop() {
  writeBmpToStrip("file.bmp");
}

int32_t readNbytesInt(File *p_file, int position, byte nBytes)
{
  if (nBytes > 4)
    return 0;

  p_file->seek(position);

  int32_t weight = 1;
  int32_t result = 0;
  for (; nBytes; nBytes--)
  {
    result += weight * p_file->read();
    weight <<= 8;
  }
  return result;
}

void writeBmpToStrip(const char *filename) {
    bmpImage = SD.open(filename, FILE_READ);
    int32_t dataStartingOffset = readNbytesInt(&bmpImage, 0x0A, 4);

    width = readNbytesInt(&bmpImage, 0x12, 4);
    height = readNbytesInt(&bmpImage, 0x16, 4);

    bmpImage.seek(dataStartingOffset);

    byte R, G, B;

    for (int32_t i = 0; i < height; i++) {
      for (int32_t j = 0; j < width; j++) {
        B = bmpImage.read();
        G = bmpImage.read();
        R = bmpImage.read();
        strip.setPixelColor(j, strip.Color(R, G, B));
      }
      //B = bmpImage.read();
      //B = bmpImage.read();
      strip.show();
      delay(speed);
    }
    bmpImage.close();
}
