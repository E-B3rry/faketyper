#include <Keyboard.h>
#include <avr/pgmspace.h>

// 2% chance
#define MISTAKE_RATE 2

#define MIN_STROKE_DELAY 90
#define MAX_STROKE_DELAY 150

#define INITIAL_BOOT_DELAY 5000
#define DEFAULT_FILE_DELAY 10000


const char file1[] PROGMEM = "escaped_file_content";
const char* const files[] PROGMEM = {
  file1,
};

const int num_files = sizeof(files) / sizeof(files[0]);
const int delays_between_files[] = {15000, 15000, 15000}; // Delays between files

const char qwerty_layout[] PROGMEM = "1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";


void setup() {
  randomSeed(analogRead(0));
  Keyboard.begin();
  delay(INITIAL_BOOT_DELAY);

  int num_delays = sizeof(delays_between_files) / sizeof(delays_between_files[0]);

  for (int i = 0; i < num_files; i++) {
    const char* file_content = (const char*)pgm_read_ptr(&files[i]);
    typeFile(file_content);
    if (i < num_files - 1) {
      if (i < num_delays) {
        delay(delays_between_files[i] ? delays_between_files[i] : DEFAULT_FILE_DELAY);
      } else {
        delay(DEFAULT_FILE_DELAY);
      }
    }
  }
}

void loop() {
  // Not needed
}


char getMistake(char key) {
  int index = -1;
  int len = strlen_P(qwerty_layout);

  for (int i = 0; i < len; i++) {
    if (pgm_read_byte(qwerty_layout + i) == key) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return key; // If key not found, return original
  }

  int left = index - 1;
  int right = index + 1;

  if (left < 0) {
    return pgm_read_byte(qwerty_layout + right);
  } else if (right >= len) {
    return pgm_read_byte(qwerty_layout + left);
  } else {
    return pgm_read_byte(qwerty_layout + (random(2) ? left : right));
  }
}

void getKeyAndShift(char c, char &key, bool &shift) {
  shift = false;

  if (c >= 'A' && c <= 'Z') {
    shift = true;
    c += 32;
  } else {
    switch (c) {
      case '!': c = '1'; shift = true; break;
      case '@': c = '2'; shift = true; break;
      case '#': c = '3'; shift = true; break;
      case '$': c = '4'; shift = true; break;
      case '%': c = '5'; shift = true; break;
      case '^': c = '6'; shift = true; break;
      case '&': c = '7'; shift = true; break;
      case '*': c = '8'; shift = true; break;
      case '(': c = '9'; shift = true; break;
      case ')': c = '0'; shift = true; break;
      case '_': c = '-'; shift = true; break;
      case '+': c = '='; shift = true; break;
      case '{': c = '['; shift = true; break;
      case '}': c = ']'; shift = true; break;
      case '|': c = '\\'; shift = true; break;
      case ':': c = ';'; shift = true; break;
      case '"': c = '\''; shift = true; break;
      case '<': c = ','; shift = true; break;
      case '>': c = '.'; shift = true; break;
      case '?': c = '/'; shift = true; break;
    }
  }

  key = c;
}

void typeFile(const char* file_content) {
  int len = strlen_P(file_content);
  for (int i = 0; i < len; i++) {
    char c = pgm_read_byte(file_content + i);
    char key;
    bool shift;

    getKeyAndShift(c, key, shift);

    if (c == '\n') {
      Keyboard.press(KEY_RETURN);
      Keyboard.release(KEY_RETURN);
    } else if (c == '\t') {
      Keyboard.press(KEY_TAB);
      Keyboard.release(KEY_TAB);
    } else {
      if (random(100) < MISTAKE_RATE) {
        char mistake = getMistake(key);
        if (shift) {
          Keyboard.press(KEY_LEFT_SHIFT);
          delay(random(60, 100));
        }
        Keyboard.write(mistake);
        if (shift) {
          delay(random(60, 100));
          Keyboard.release(KEY_LEFT_SHIFT);
        }
        delay(random(150, 300));
        Keyboard.press(KEY_BACKSPACE);
        Keyboard.release(KEY_BACKSPACE);
        delay(random(MIN_STROKE_DELAY, MAX_STROKE_DELAY));
      }
      if (shift) {
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(random(60, 100));
      }
      Keyboard.press(key);
      Keyboard.release(key);
      if (shift) {
        delay(random(60, 100));
        Keyboard.release(KEY_LEFT_SHIFT);
      }
    }
    delay(random(MIN_STROKE_DELAY, MAX_STROKE_DELAY));
  }
}
