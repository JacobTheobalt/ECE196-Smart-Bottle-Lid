#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
double count=0;

// Define resolution of display 128x32
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 32)
#error("Error defining the height.");
#endif

void setup_LCD(){
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C address: 0x3C 
  display.display();
  delay(2000);
  display.clearDisplay();  // Clear the buffer.
}

void showText(String text, int x, int y,int size, boolean d) {

  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
}

void showInDisplay(String text){
  display.clearDisplay();
  showText(text, 0, 0, 1, false);
  display.display();
}