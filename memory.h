#include <LittleFS.h>
#include <FS.h>

int TWINKLE_SPEED = 4;
int TWINKLE_DENSITY = 8;
int SECONDS_PER_PALETTE = 3;
int BRIGHTNESS = 255;
int MODE = 0;

//function prototypes
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void startMemory() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  TWINKLE_SPEED = readFile(LittleFS, "/twinklespeed.txt").toInt();
  TWINKLE_DENSITY = readFile(LittleFS, "/twinkledensity.txt").toInt();
  SECONDS_PER_PALETTE = readFile(LittleFS, "/secondspermode.txt").toInt();
  BRIGHTNESS = readFile(LittleFS, "/brightness.txt").toInt();
  MODE = readFile(LittleFS, "/mode.txt").toInt();
}
