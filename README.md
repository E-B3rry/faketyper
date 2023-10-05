# FakeTyper

**Disclaimer:** This program is provided for entertainment purposes only and may not have practical applications. The code quality is not up to any professional standards. Really.

FakeTyper simulates typing on a keyboard using an Arduino board with USB HID support. It reads specified files and types them with occasional mistakes and corrections.

## Contents

- `faketyper.ino`: Arduino sketch for typing simulation.
- `utils/generate_keystroke.sh`: Script to process input files and generate PROGMEM content.

## Usage

1. Prepare input files and place them in a directory.
2. Run `generate_keystroke.sh` with the input directory and file extensions:

   ```
   ./generate_keystroke.sh /path/to/input/files '.c .h'
   ```

   This generates a `faketyper.out` file with PROGMEM content.

3. Update `faketyper.ino` with the content of `faketyper.out`:

   - Type `y` when prompted by the script to update automatically.
   - Or manually copy the content from `faketyper.out` to `faketyper.ino`.

4. Edit any parameters you want, most of them are preprocessor directives at top of the program.
5. Connect a compatible Arduino board to your computer.
6. Upload `faketyper.ino` to the board.
7. The board will simulate typing the content with occasional mistakes and corrections.
