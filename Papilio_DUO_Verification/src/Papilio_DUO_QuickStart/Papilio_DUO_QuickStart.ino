/*
  Gadget Factory
  Papilio Pro QuickStart Example
 
 Pressing any of the 4 pushbuttons on the Button/LED Wing will light the corresponding LED. 
 
 BPW5007 Button/LED Wing Reference:
 http://www.gadgetfactory.net/gf/project/bpw5007-butnled/
 
 Hardware:
 * Connect a Button/LED Wing to any available Wing Slot

 created 2010
 by Jack Gassett from existing Arduino code snippets
 http://www.gadgetfactory.net
 
 11/19/2012 Jack Gassett
   -Added LED1 blinking
 
 This example code is in the public domain.
 */

#define PORTAB GPIODATA(0)
#define PORTC GPIODATA(1)

int ledPins[] = { 
  15, 17, 19, 21, 25, 29, 33, 37, 41, 45, 49, 53, 22, 26, 30, 34, 38, 42, 46, 50  };       // an array of pin numbers to which LEDs are attached
int ledCount = 20;           // the number of pins (i.e. the length of the array)

int buttonPins[] = { 
  14, 16, 18, 20, 23, 27, 31, 35, 39, 43, 47, 51, 24, 28, 32, 36, 40, 44, 48, 52 };       // an array of pin numbers to which Buttons are attached
int buttonCount = 20;           // the number of pins (i.e. the length of the array)

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int thisPin;
int ledState = LOW;
int incomingByte = 0;

// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33; 
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!'; 

void setup() {
  // initialize the LED pins as an output:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);  
  }
  
  // initialize the pushbutton pin as an input:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  } 

  // initialize all of the ATmega32U4 pins as inputs so as not to interfere.
  for (int thisPin = 0; thisPin < 14; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  }   
  
  //Delay for 1 seconds to prevent detection as a serial mouse.
  delay(2000);
  
  //Setup Serial port and send out Title
  Serial.begin(9600); 

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  // prints title with ending line break 
  Serial.println("ASCII Table ~ Character Map"); 
}

void testport(volatile uint32_t* port, char pName[2], uint32_t check1, uint32_t check2) {
    int status = 0;
    int temp;
    digitalWrite(0, LOW);
    delay(10);
      //Serial.println(*port, HEX);    
    if (*port == check1) {
      status = 1;
    }
    else
      temp = *port;
    digitalWrite(0, HIGH);
    delay(10);
      //Serial.println(*port, HEX);    
    if (*port == check2 && status==1){
      Serial.print(pName);
      Serial.println(" Passed");}
    else{
      Serial.print(pName);
      Serial.println(" Failed");
      Serial.print("Should be:");
      Serial.print(check1, HEX);
      Serial.print(" ");
      Serial.print(check2, HEX);
      Serial.print(" "); 
      Serial.print(check1, BIN);
      Serial.print(" ");
      Serial.println(check2, BIN);      

      Serial.print("Actual   :");
      Serial.print(temp, HEX);
      Serial.print(" ");
      Serial.print(*port, HEX);
      Serial.print(" "); 
      Serial.print(temp, BIN);
      Serial.print(" ");
      Serial.println(*port, BIN);       
      Serial.println();    
    }
}

void printnibble(unsigned int c)
{
	c&=0xf;
	if (c>9)
		Serial.write(c+'a'-10);
	else
		Serial.write(c+'0');
}

void printhexbyte(unsigned int c)
{
	printnibble(c>>4);
	printnibble(c);
}


void printhex(unsigned int c)
{
	printhexbyte(c>>24);
	printhexbyte(c>>16);
	printhexbyte(c>>8);
	printhexbyte(c);
}

void memfill(volatile unsigned*end)
{
	volatile unsigned *c = (volatile unsigned*)0x2000;
	while (c<end) {
		*c = (unsigned)c ^ 0xffffbeef;
        c++;
	}
}

int memcheck(volatile unsigned *end)
{
	volatile unsigned *c = (volatile unsigned*)0x2000;
	while (c<end) {
		unsigned v = (unsigned)c ^ 0xffffbeef;
		if (v !=*c) {
			Serial.print("Error at address "); printhex((unsigned)c); Serial.println("");
			return -1;
		}
		c++;
	}
    return 0;
}

void memtest()
{
	volatile unsigned *ptr = (volatile unsigned *)0x2000;

//	while (!Serial.available());
//	Serial.read();

	Serial.println("Starting memory test from 0x2000 and beyond");
	unsigned v = *((unsigned*)0);
	Serial.print("check value is 0x"); printhex(v); Serial.println("");

	int i;
	for (;;) {
		if (*ptr == v) {
			Serial.print("Found tag at "); printhex((unsigned)ptr); Serial.println("");
			Serial.println("Testing up to that. Filling...");
			memfill(ptr);
			Serial.println("Checking...");
			if (memcheck(ptr)==0) {
				Serial.println("All OK!");
			}
			break;
		}
		ptr++;
	}
        Serial.println();
}

void loop(){
  incomingByte = Serial.read();
  if (incomingByte == 109)    //Memtest if m is sent on serial port.
    memtest();

  //This sends the ASCII table to the serial port.
  
  // prints value unaltered, i.e. the raw binary version of the 
  // byte. The serial monitor interprets all bytes as 
  // ASCII, so 33, the first number,  will show up as '!' 
  Serial.print(thisByte);    //TODO: how to get this working on ZPUino.

  Serial.print(", dec: "); 
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);      
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);  


  Serial.print(", hex: "); 
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);     

  Serial.print(", oct: "); 
  // prints value as string in octal (base 8);
  Serial.print(thisByte, OCT);     

  Serial.print(", bin: "); 
  // prints value as string in binary (base 2) 
  // also prints ending line break:
  Serial.println(thisByte, BIN);   

  // if printed last visible character '~' or 126, stop: 
  if(thisByte == 126) {     // you could also use if (thisByte == '~') {
    thisByte = 33;
  } 
  // go on to the next character
  thisByte++;  
  
  //This section blinks the LED's and keeps them solid if a button is pressed. 
  delay(200);                  // wait for a second 
  ledState = !ledState;  
  for (thisPin = 0; thisPin < buttonCount; thisPin++)  {
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPins[thisPin]);
  
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState == HIGH) {     
      // turn LED on:    
      digitalWrite(ledPins[thisPin], HIGH);  
    } 
    else {
      // toggle LED:
      digitalWrite(ledPins[thisPin], ledState); 
    }
  }
  //digitalWrite(13,ledState);  //For LED1
}
