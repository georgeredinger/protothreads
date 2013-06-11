#include "pt.h"
#include <IRremote.h>

#define PIN_RECV 2
#define PIN_IR 3
#define PIN_STATUS 4

IRsend irsend;

static struct pt pt_read, pt_mark_space;

static int readThread(struct pt *pt){
	static unsigned long timestamp = 0;
	PT_BEGIN(pt);
	while(1) { 
		PT_WAIT_UNTIL(pt, millis() - timestamp > 100 );
		timestamp = millis(); // take a new timestamp
    int state = digitalRead(PIN_RECV);
	  digitalWrite(PIN_STATUS,!state);
	}
	PT_END(pt);
}

static int markspaceThread(struct pt *pt){
	static unsigned long timestamp = 0;
	PT_BEGIN(pt);
	while(1) { 
		PT_WAIT_UNTIL(pt, millis() - timestamp > 1000 );
		timestamp = millis(); // take a new timestamp
	  irsend.mark(0);
		PT_WAIT_UNTIL(pt, millis() - timestamp > 1000 );
		timestamp = millis(); // take a new timestamp
	  irsend.space(0);
	}
	PT_END(pt);
}

void setup()
{
	  PT_INIT(&pt_mark_space);
	  PT_INIT(&pt_read);

	  Serial.begin(115200);
		Serial.println("IR demo");
	  pinMode(PIN_RECV, INPUT);
		pinMode(PIN_STATUS, OUTPUT);
		irsend.enableIROut(38);
	  irsend.space(0);
}


void loop() { 
  markspaceThread(&pt_mark_space);
  readThread(&pt_read);
}

