#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define buzzer 11
#define relay 10
#define PASSWORD_LENGTH 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

char storedPassword[PASSWORD_LENGTH + 1] = "1234";
char enteredPassword[PASSWORD_LENGTH + 1];
byte currentPosition = 0;
byte a = 5;
bool value = true;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'},
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("DOOR LOCK SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(1, 0);
  lcd.print("ENTER PASSWORD");

  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(60);
    if (key == 'C') {
      resetPassword();
    } else if (key == 'D') {
      if (value) {
        doorlocked();
        value = false;
      } else {
        dooropen();
        value = true;
      }
    } else {
      processNumberKey(key);
    }
  }
}

void processNumberKey(char key) {
  if (currentPosition < PASSWORD_LENGTH) {
    lcd.setCursor(a, 1);
    lcd.print("*");
    a++;
    enteredPassword[currentPosition] = key;
    currentPosition++;
    enteredPassword[currentPosition] = '\0';
  }
}

bool checkPassword() {
  return strcmp(enteredPassword, storedPassword) == 0;
}

void dooropen() {
  if (checkPassword()) {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    digitalWrite(relay, LOW);
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("CORRECT PASSWORD");
    lcd.setCursor(0, 1);
    lcd.print("DOOR OPENED");
    delay(2000);
    lcd.clear();
    a = 5;
  } else {
    wrongPasswordAlert();
  }
  resetPassword();
}

void doorlocked() {
  if (checkPassword()) {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    digitalWrite(relay, HIGH);
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("CORRECT PASSWORD");
    lcd.setCursor(2, 1);
    lcd.print("DOOR LOCKED");
    delay(2000);
    lcd.clear();
    a = 5;
  } else {
    wrongPasswordAlert();
  }
  resetPassword();
}

void wrongPasswordAlert() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
  lcd.setCursor(0, 0);
  lcd.print("WRONG PASSWORD!");
  lcd.setCursor(0, 1);
  lcd.print("PLEASE TRY AGAIN");
  delay(2000);
  lcd.clear();
  a = 5;
}

void resetPassword() {
  currentPosition = 0;
  memset(enteredPassword, 0, sizeof(enteredPassword));
  lcd.clear();
  a = 5;
}