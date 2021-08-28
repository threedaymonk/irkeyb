#include <Keyboard.h>
#include <IRremote.h>

#define IR_RECEIVE_PIN 2
#define MIN_INTERVAL 200
#define KEY_PRTSCR 0xCE

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  Keyboard.begin();
}

void type(uint32_t command) {
  // One for All code 1840
  switch(command) {
    case 0x408: break; // Power - reprogram to TV power

    case 0x48f: Keyboard.write('r'); break; // RW
    case 0x4b0: Keyboard.write('p'); break; // Play
    case 0x48e: Keyboard.write('f'); break; // FF

    case 0x4b1: Keyboard.write('x'); break; // Stop
    case 0x4ba: Keyboard.write(' '); break; // Pause = Play/pause
    case 0x4bd: break; // Rec not used

    case 0x4ab: Keyboard.write('c'); break; // Guide = Context menu
    case 0x443: Keyboard.write('m'); break; // Menu
    case 0x4aa: Keyboard.write('i'); break; // Info

    case 0x440: Keyboard.write(KEY_UP_ARROW); break; // Up
    case 0x441: Keyboard.write(KEY_DOWN_ARROW); break; // Down
    case 0x407: Keyboard.write(KEY_LEFT_ARROW); break; // Left
    case 0x406: Keyboard.write(KEY_RIGHT_ARROW); break; // Right
    case 0x444: Keyboard.write(KEY_RETURN); break; // OK

    case 0x40b: Keyboard.write('o'); break; // AV = Info
    case 0x45b: Keyboard.write(KEY_ESC); break; // Exit

    case 0x402: Keyboard.write('+'); break; // V+
    case 0x403: Keyboard.write('-'); break; // V-
    case 0x428: Keyboard.write(KEY_BACKSPACE); break; // Back
    case 0x409: Keyboard.write(KEY_F8); break; // Mute
    case 0x400:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.release(KEY_UP_ARROW);
      Keyboard.release(KEY_LEFT_CTRL);
      break; // + = Subtitles up
    case 0x401:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.release(KEY_DOWN_ARROW);
      Keyboard.release(KEY_LEFT_CTRL);
      break; // - = Subtitles down

    case 0x472:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('o');
      Keyboard.release('o');
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_CTRL);
      break; // Red = Debug info
    case 0x471: Keyboard.write(KEY_PRTSCR); break; // Yellow = Screenshot
    case 0x463: Keyboard.write('l'); break; // Yellow = Subtitle language
    case 0x461: Keyboard.write('t'); break; // Blue = Subtitles

    case 0x411: Keyboard.write('1'); break; // 1
    case 0x412: Keyboard.write('2'); break; // 2
    case 0x413: Keyboard.write('3'); break; // 3

    case 0x414: Keyboard.write('4'); break; // 4
    case 0x415: Keyboard.write('5'); break; // 5
    case 0x416: Keyboard.write('6'); break; // 6

    case 0x417: Keyboard.write('7'); break; // 7
    case 0x418: Keyboard.write('8'); break; // 8
    case 0x419: Keyboard.write('9'); break; // 9

    case 0x49f: break; // Apps
    case 0x410: Keyboard.write('0'); break; // 0
    case 0x40f: break; // TV = reprogram to TV input
  }
}

void dispatch(uint32_t command, bool isRepeat) {
#ifdef DEBUG
  Serial.print("cmd:    ");
  Serial.println(command, HEX);
  Serial.print("repeat: ");
  Serial.println(isRepeat);
  Serial.println();
#endif
  type(command);
}

void loop() {
  static uint32_t command, lastCommand = 0;
  static unsigned long lastCommandAt = 0;

  if (IrReceiver.decode()) {
    IrReceiver.resume();

    if (IrReceiver.decodedIRData.protocol != 0) {
      command = IrReceiver.decodedIRData.address * 0x100 + IrReceiver.decodedIRData.command;
      if (command == lastCommand) {
        if ((millis() - lastCommandAt) > MIN_INTERVAL) {
          lastCommandAt = millis();
          dispatch(command, true);
        }
      } else {
        lastCommandAt = millis();
        lastCommand = command;
        dispatch(command, false);
      }
    }
  }
}
