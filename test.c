#include <IRremote.h>

/*-----( Global Constants )-----*/

bool canOpenDoor = false;
bool isDoorOpen = false;
bool isPowerOn = true;

const int doorPin = 10;
const int buzerPin = 9;
const int receiver = 11;      // Signal Pin of IR receiver to Arduino Digital Pin 11
const int COMBINATION_LENGTH = 4;
const char GOOD_LOCK[] = "0920";

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
char irCodeCombination[COMBINATION_LENGTH + 1] = ""; // 4 char array to keep track of IR codes
bool shouldCheckCombination = false;

bool checkCombination() {
  return strcmp(irCodeCombination, GOOD_LOCK) == 0;
}

void addToCombination(char num, bool resetIndex=false) {
  static int index = 0;

  if (index < COMBINATION_LENGTH) {
    irCodeCombination[index++] = num;
    irCodeCombination[index] = '\0'; // Null-terminate the string
  }
  if (resetIndex) {
    index = 0;
  }
}

void addAndCheckCombination(char num) {
  if (shouldCheckCombination) {
    addToCombination(num);
    if (checkCombination()) {
      Serial.println("Combination correct!");
      canOpenDoor = true;
    }
  }
}

/*-----( Function )-----*/
void translateIR() {          // takes action based on IR code received
  static int index = 0;

  switch(results.value){
    case 0xFFA25D: Serial.println("POWER");
      if (isPowerOn) {
        if (checkCombination()) {
          Serial.println("Turning off");
          isPowerOn = false;
        }
      } else {
        Serial.println("Turning on");
        isPowerOn = true;
      }
      break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK"); break;
    case 0xFF02FD: Serial.println("PAUSE"); break;
    case 0xFFC23D: Serial.println("FAST FORWARD"); break;
    case 0xFFE01F:
      Serial.println("DOWN");
      addToCombination('A', true);
      shouldCheckCombination = false;
      break;
    case 0xFFA857: Serial.println("VOL-"); break;
    case 0xFF906F:
      Serial.println("UP");
      shouldCheckCombination = true;
      break;
    case 0xFF9867: Serial.println("EQ"); break;
    case 0xFFB04F: Serial.println("ST/REPT"); break;
    case 0xFF6897: Serial.println("0"); addAndCheckCombination('0'); break;
    case 0xFF30CF: Serial.println("1"); addAndCheckCombination('1'); break;
    case 0xFF18E7: Serial.println("2"); addAndCheckCombination('2'); break;
    case 0xFF7A85: Serial.println("3"); addAndCheckCombination('3'); break;
    case 0xFF10EF: Serial.println("4"); addAndCheckCombination('4'); break;
    case 0xFF38C7: Serial.println("5"); addAndCheckCombination('5'); break;
    case 0xFF5AA5: Serial.println("6"); addAndCheckCombination('6'); break;
    case 0xFF42BD: Serial.println("7"); addAndCheckCombination('7'); break;
    case 0xFF4AB5: Serial.println("8"); addAndCheckCombination('8'); break;
    case 0xFF52AD: Serial.println("9"); addAndCheckCombination('9'); break;
    case 0xFFFFFFFF:
      Serial.println("REPEAT");
      break;
    default:
      Serial.print(" other button   ");
      Serial.println(results.value);
      break;
  }// End Case
} //END translateIR


void setup(){   /*----( SETUP: RUNS ONCE )----*/
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();           // Start the receiver
  pinMode(buzerPin, OUTPUT);
  digitalWrite(buzerPin, LOW);
  pinMode(doorPin, OUTPUT);
  digitalWrite(doorPin, LOW);
}/*--(end setup )---*/

void loop(){   /*----( LOOP: RUNS CONSTANTLY )----*/
  if (irrecv.decode(&results))   // have we received an IR signal?
  {
    translateIR();
    delay(500);                 // Do not get immediate repeat
    irrecv.resume();            // receive the next value
  }
  if (isPowerOn) {
      Serial.println("power is on, checking door");
      if (canOpenDoor) {
        Serial.println("door can be opened now");
        delay(10000);
        canOpenDoor = false;
      } else {
        if (isDoorOpen){
          Serial.println("alarm");
          digitalWrite(buzerPin, HIGH);
          delay(500);
        } else {
          digitalWrite(buzerPin, LOW);
        }
      }
      isDoorOpen = !digitalRead(10);
  }
}/* --(end main loop )-- */
