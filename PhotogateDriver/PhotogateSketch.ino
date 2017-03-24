//Arduino sketch for photogate.  Signal IN -> PIN 8.  16 Sep. 2015, Revised 24 March 2017
//A timer which is triggered by raising a digital input to HIGH (8 default).
//Data prited to serial is deliminated by "_" and "~".

#define ledPin  13						// LED connected to digital pin 13
#define gatePin 8						// Photogate connected to digital pin 8

int countMode1 = 0;
int countMode2 = 0;
int count_reset;
int userMode;							// 1 for gap timer, 2 for event timer, 3 for event counter, 0 to clear choice for invalid userIn
int countMode3 = 0;							// variable for storing event count integer
int value = LOW;						// previous value of the LED
int gateState_current;					// variable to store current gate state
int gateState_last;						// variable to store last gate state
int blinking;							// condition for blinking - timer is timing
long interval = 100;					// blink interval - change to suit
long previousMillis = 0;				// variable to store last time LED was updated
unsigned long startTime, elapsedTime, eventTime, cumulativeTime, cumulativeTimeNext, fractional;		// start time for stop watch

// variable used to store fractional part of time



// its the same counter



void setup()

{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	pinMode(8, INPUT);
  userMode = 2;

	//USER OPTIONS MENU

	//Serial.println("\n\n ---------------\n   Select Mode:\n[1] - Gap Timer\n[2] - Event Timer\n[3] - Event Counter \n --------------");

}


void loop()
{

	// -----------   USER CONTROL MODES ---------------

	if (Serial.available()) {
		char modeSelect = Serial.read();
		if (modeSelect == '1'){												// Gap timer 
			userMode = 1;
			//Serial.println("\n\nGap Timer Ready\n");
		}

		else if (modeSelect == '2'){										//  Event timer
			userMode = 2;
			//Serial.println("\n\nEvent Timer Ready\n");
		}

		else if (modeSelect == '3'){
			userMode = 3;
			//Serial.println("\n\nEvent Counter Ready - Press 'r' to reset");
		}

		else if (modeSelect == 'r' || 'R'){
			countMode3 = 0, countMode2 = 0, countMode1 = 0;
			Serial.flush();
			//Serial.println("\nCounter Reset\n");
		}


		else{
			userMode = 0;
			//Serial.println("\n\nEnter a valid input\n\n--------------\nSelect Mode:\n[1] - Gap Timer\n[2] - Event Timer\n[3] - Event Counter\n--------------");
			Serial.flush();
		}
	}

	gateState_current = digitalRead(gatePin);                                            // reads the gate state and stores

	// -------------   MODE 1 - GAP TIMER   ----------------------------

	if (userMode == 1){
		countMode3 = 0;																		// reset count from mode 3,2
		countMode2 = 0;

		if (gateState_current == LOW && gateState_last == HIGH  &&  blinking == false){      // check for a low to high transistion, if true then found a new button press while clock is not running - start the clock
			startTime = micros();                                                             // store the start time
			blinking = true;                                                                  // turn on blinking while timing
			gateState_last = gateState_current;                                               // store buttonState in lastButtonState, to compare next time
		}

		else if (gateState_current == LOW && gateState_last == HIGH && blinking == true){     // check for a high to low transition.  if true then found a new button press while clock is running - stop the clock and report
			elapsedTime = micros() - startTime;                                            // store elapsed time
			blinking = false;                                                                // turn off blinking, all done timing
			gateState_last = gateState_current;                                              // store buttonState in lastButtonState, to compare next time
			countMode1++;

			
			Serial.print(countMode1);  // print count
			Serial.print("_");
			// routine to report elapsed time 
			Serial.print((int)(elapsedTime / 1000000L));                                       // divide by 1000 to convert to seconds - then cast to an int to print
			Serial.print(".");                                                               // print decimal point
			fractional = (int)(elapsedTime % 1000000L);                                         // use modulo operator to get fractional part of time 



			// pad in leading zeros 
			//if (fractional == 0)
			//	Serial.print("000");                                                            // add three zero's
			//else if (fractional < 10)                                                          // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
			//	Serial.print("00");                                                             // add two zeros
			//	else if (fractional < 100)
			//		Serial.print("0");                                                              // add one zero
			Serial.print(fractional);                                                        // print fractional part of time 
      Serial.print("~");
		}
		else{
			gateState_last = gateState_current;                                                 // store buttonState in lastButtonState, to compare next time
		}
	}

	// ---------------------   MODE 2 - EVENT TIMER   -------------------------

	else if (userMode == 2){
		countMode3 = 0;																				// reset count var for future use
		if (gateState_current == HIGH && gateState_last == LOW){      // check for a low to high transistion, if true then found a new button press while clock is not running - start the clock
			startTime = micros();                                                             // store the start time
			cumulativeTime = micros();

			countMode2++;
			//Serial.print(count);
			//	Serial.print("\t");
			blinking = true;                                                                  // turn on blinking while timing



			gateState_last = gateState_current;                                               // store buttonState in lastButtonState, to compare next time
		}

		else if (gateState_current == LOW && gateState_last == HIGH){     // check for a high to low transition.  if true then found a new button press while clock is running - stop the clock and report

			eventTime = micros() - startTime;                                            // store elapsed time

			blinking = false;                                                                // turn off blinking, all done timing
			gateState_last = gateState_current;                                              // store buttonState in lastButtonState, to compare next time



			// routine to report elapsed time 

			
			Serial.print(countMode2);  // print count
			Serial.print("_");

//			Serial.print(cumulativeTime / 1000000L);
//			Serial.print(".");
//			Serial.print(cumulativeTime % 1000000);
//			Serial.print("\t");

			Serial.print(eventTime / 1000000L);                                       // divide by 1000 to convert to seconds - then cast to an int to print
			Serial.print(".");                                                               // print decimal point
			Serial.print(eventTime % 1000000L);
      Serial.print("~");

		}
		else{
			gateState_last = gateState_current;                                                 // store buttonState in lastButtonState, to compare next time
		}
	}

	//  -----------------------   MODE 3 - EVENT COUNTER    -----------------------------------

	else if (userMode == 3){

		countMode2 = 0;																			// reset count variabe from last session
		if (gateState_current == HIGH && gateState_last == LOW){								//  detect new gate obstruction
			countMode3++;


			Serial.print(countMode3);
			Serial.print("_");
			Serial.print(micros() / 1000000L);                                       // divide by 1000 to convert to seconds - then cast to an int to print
			Serial.print(".");                                                               // print decimal point 
			Serial.print(micros() % 1000000L);
      Serial.print("~");

			gateState_last = gateState_current;
		}

		else if (gateState_current == LOW && gateState_last == HIGH){							// check for a high to low transition.  if true then found a new button press while clock is running - stop the clock and report
			//blinking = false;                                                                // turn off blinking, all done timing

			gateState_last = gateState_current;
		}

	}



	// blink routine - blink the LED while timing
	// check to see if it's time to blink the LED; that is, the difference
	// between the current time and last time we blinked the LED is larger than
	// the interval at which we want to blink the LED.
	if ((millis() - previousMillis > interval)) {
		if (blinking == true){
			previousMillis = millis();                                                      // remember the last time we blinked the LED  if the LED is off turn it on and vice-versa.
			if (value == LOW)
				value = HIGH;
			else
				value = LOW;
			digitalWrite(ledPin, value);
		}
		else{
			digitalWrite(ledPin, LOW);                                                      // turn off LED when not blinking
		}
	}

}
