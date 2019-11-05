#include <kernel/ps2/keyboard.h>
#include <stdio.h>

ps2k_key keys[256];
char in_buffer[PS2K_IN_BUFFER_SIZE];
unsigned int in_buffer_start;
unsigned int in_buffer_end;
int is_buffering;

void ps2k_init() {
    in_buffer_start = 0;
    in_buffer_end = 0;
    is_buffering = 0;

    keys[PS2K_KEYCODE_ESC].pressed = 0;
    keys[PS2K_KEYCODE_ESC].ascii = 27;
    keys[PS2K_KEYCODE_ESC].s_ascii = 27;

    keys[PS2K_KEYCODE_F1].pressed = 0;
    keys[PS2K_KEYCODE_F1].ascii = 0;
    keys[PS2K_KEYCODE_F1].s_ascii = 0;

    keys[PS2K_KEYCODE_F2].pressed = 0;
    keys[PS2K_KEYCODE_F2].ascii = 0;
    keys[PS2K_KEYCODE_F2].s_ascii = 0;

    keys[PS2K_KEYCODE_F3].pressed = 0;
    keys[PS2K_KEYCODE_F3].ascii = 0;
    keys[PS2K_KEYCODE_F3].s_ascii = 0;

    keys[PS2K_KEYCODE_F4].pressed = 0;
    keys[PS2K_KEYCODE_F4].ascii = 0;
    keys[PS2K_KEYCODE_F4].s_ascii = 0;

    keys[PS2K_KEYCODE_F5].pressed = 0;
    keys[PS2K_KEYCODE_F5].ascii = 0;
    keys[PS2K_KEYCODE_F5].s_ascii = 0;

    keys[PS2K_KEYCODE_F6].pressed = 0;
    keys[PS2K_KEYCODE_F6].ascii = 0;
    keys[PS2K_KEYCODE_F6].s_ascii = 0;

    keys[PS2K_KEYCODE_F7].pressed = 0;
    keys[PS2K_KEYCODE_F7].ascii = 0;
    keys[PS2K_KEYCODE_F7].s_ascii = 0;

    keys[PS2K_KEYCODE_F8].pressed = 0;
    keys[PS2K_KEYCODE_F8].ascii = 0;
    keys[PS2K_KEYCODE_F8].s_ascii = 0;

    keys[PS2K_KEYCODE_F9].pressed = 0;
    keys[PS2K_KEYCODE_F9].ascii = 0;
    keys[PS2K_KEYCODE_F9].s_ascii = 0;

    keys[PS2K_KEYCODE_F10].pressed = 0;
    keys[PS2K_KEYCODE_F10].ascii = 0;
    keys[PS2K_KEYCODE_F10].s_ascii = 0;

    keys[PS2K_KEYCODE_F11].pressed = 0;
    keys[PS2K_KEYCODE_F11].ascii = 0;
    keys[PS2K_KEYCODE_F11].s_ascii = 0;

    keys[PS2K_KEYCODE_F12].pressed = 0;
    keys[PS2K_KEYCODE_F12].ascii = 0;
    keys[PS2K_KEYCODE_F12].s_ascii = 0;

    keys[PS2K_KEYCODE_BACKTICK].pressed = 0;
    keys[PS2K_KEYCODE_BACKTICK].ascii = '`';
    keys[PS2K_KEYCODE_BACKTICK].s_ascii = '~';

    keys[PS2K_KEYCODE_1].pressed = 0;
    keys[PS2K_KEYCODE_1].ascii = '1';
    keys[PS2K_KEYCODE_1].s_ascii = '!';

    keys[PS2K_KEYCODE_2].pressed = 0;
    keys[PS2K_KEYCODE_2].ascii = '2';
    keys[PS2K_KEYCODE_2].s_ascii = '@';

    keys[PS2K_KEYCODE_3].pressed = 0;
    keys[PS2K_KEYCODE_3].ascii = '3';
    keys[PS2K_KEYCODE_3].s_ascii = '#';

    keys[PS2K_KEYCODE_4].pressed = 0;
    keys[PS2K_KEYCODE_4].ascii = '4';
    keys[PS2K_KEYCODE_4].s_ascii = '$';

    keys[PS2K_KEYCODE_5].pressed = 0;
    keys[PS2K_KEYCODE_5].ascii = '5';
    keys[PS2K_KEYCODE_5].s_ascii = '%';

    keys[PS2K_KEYCODE_6].pressed = 0;
    keys[PS2K_KEYCODE_6].ascii = '6';
    keys[PS2K_KEYCODE_6].s_ascii = '^';

    keys[PS2K_KEYCODE_7].pressed = 0;
    keys[PS2K_KEYCODE_7].ascii = '7';
    keys[PS2K_KEYCODE_7].s_ascii = '&';

    keys[PS2K_KEYCODE_8].pressed = 0;
    keys[PS2K_KEYCODE_8].ascii = '8';
    keys[PS2K_KEYCODE_8].s_ascii = '*';

    keys[PS2K_KEYCODE_9].pressed = 0;
    keys[PS2K_KEYCODE_9].ascii = '9';
    keys[PS2K_KEYCODE_9].s_ascii = '(';

    keys[PS2K_KEYCODE_0].pressed = 0;
    keys[PS2K_KEYCODE_0].ascii = '0';
    keys[PS2K_KEYCODE_0].s_ascii = ')';

    keys[PS2K_KEYCODE_MINUS].pressed = 0;
    keys[PS2K_KEYCODE_MINUS].ascii = '-';
    keys[PS2K_KEYCODE_MINUS].s_ascii = '_';

    keys[PS2K_KEYCODE_EQUALS].pressed = 0;
    keys[PS2K_KEYCODE_EQUALS].ascii = '=';
    keys[PS2K_KEYCODE_EQUALS].s_ascii = '+';

    keys[PS2K_KEYCODE_BACKSPACE].pressed = 0;
    keys[PS2K_KEYCODE_BACKSPACE].ascii = 8;
    keys[PS2K_KEYCODE_BACKSPACE].s_ascii = 8;

    keys[PS2K_KEYCODE_TAB].pressed = 0;
    keys[PS2K_KEYCODE_TAB].ascii = '\t';
    keys[PS2K_KEYCODE_TAB].s_ascii = '\t';

    keys[PS2K_KEYCODE_Q].pressed = 0;
    keys[PS2K_KEYCODE_Q].ascii = 'q';
    keys[PS2K_KEYCODE_Q].s_ascii = 'Q';

    keys[PS2K_KEYCODE_W].pressed = 0;
    keys[PS2K_KEYCODE_W].ascii = 'w';
    keys[PS2K_KEYCODE_W].s_ascii = 'W';

    keys[PS2K_KEYCODE_E].pressed = 0;
    keys[PS2K_KEYCODE_E].ascii = 'e';
    keys[PS2K_KEYCODE_E].s_ascii = 'E';

    keys[PS2K_KEYCODE_R].pressed = 0;
    keys[PS2K_KEYCODE_R].ascii = 'r';
    keys[PS2K_KEYCODE_R].s_ascii = 'R';

    keys[PS2K_KEYCODE_T].pressed = 0;
    keys[PS2K_KEYCODE_T].ascii = 't';
    keys[PS2K_KEYCODE_T].s_ascii = 'T';

    keys[PS2K_KEYCODE_Y].pressed = 0;
    keys[PS2K_KEYCODE_Y].ascii = 'y';
    keys[PS2K_KEYCODE_Y].s_ascii = 'Y';

    keys[PS2K_KEYCODE_U].pressed = 0;
    keys[PS2K_KEYCODE_U].ascii = 'u';
    keys[PS2K_KEYCODE_U].s_ascii = 'U';

    keys[PS2K_KEYCODE_I].pressed = 0;
    keys[PS2K_KEYCODE_I].ascii = 'i';
    keys[PS2K_KEYCODE_I].s_ascii = 'I';

    keys[PS2K_KEYCODE_O].pressed = 0;
    keys[PS2K_KEYCODE_O].ascii = 'o';
    keys[PS2K_KEYCODE_O].s_ascii = 'O';

    keys[PS2K_KEYCODE_P].pressed = 0;
    keys[PS2K_KEYCODE_P].ascii = 'p';
    keys[PS2K_KEYCODE_P].s_ascii = 'P';

    keys[PS2K_KEYCODE_LSQBRACKET].pressed = 0;
    keys[PS2K_KEYCODE_LSQBRACKET].ascii = '[';
    keys[PS2K_KEYCODE_LSQBRACKET].s_ascii = '{';

    keys[PS2K_KEYCODE_RSQBRACKET].pressed = 0;
    keys[PS2K_KEYCODE_RSQBRACKET].ascii = ']';
    keys[PS2K_KEYCODE_RSQBRACKET].s_ascii = '}';

    keys[PS2K_KEYCODE_ENTER].pressed = 0;
    keys[PS2K_KEYCODE_ENTER].ascii = '\n';
    keys[PS2K_KEYCODE_ENTER].s_ascii = '\n';

    keys[PS2K_KEYCODE_CAPSLOCK].pressed = 0;
    keys[PS2K_KEYCODE_CAPSLOCK].ascii = 0;
    keys[PS2K_KEYCODE_CAPSLOCK].s_ascii = 0;

    keys[PS2K_KEYCODE_A].pressed = 0;
    keys[PS2K_KEYCODE_A].ascii = 'a';
    keys[PS2K_KEYCODE_A].s_ascii = 'A';

    keys[PS2K_KEYCODE_S].pressed = 0;
    keys[PS2K_KEYCODE_S].ascii = 's';
    keys[PS2K_KEYCODE_S].s_ascii = 'S';

    keys[PS2K_KEYCODE_D].pressed = 0;
    keys[PS2K_KEYCODE_D].ascii = 'd';
    keys[PS2K_KEYCODE_D].s_ascii = 'D';

    keys[PS2K_KEYCODE_F].pressed = 0;
    keys[PS2K_KEYCODE_F].ascii = 'f';
    keys[PS2K_KEYCODE_F].s_ascii = 'F';

    keys[PS2K_KEYCODE_G].pressed = 0;
    keys[PS2K_KEYCODE_G].ascii = 'g';
    keys[PS2K_KEYCODE_G].s_ascii = 'G';

    keys[PS2K_KEYCODE_H].pressed = 0;
    keys[PS2K_KEYCODE_H].ascii = 'h';
    keys[PS2K_KEYCODE_H].s_ascii = 'H';

    keys[PS2K_KEYCODE_J].pressed = 0;
    keys[PS2K_KEYCODE_J].ascii = 'j';
    keys[PS2K_KEYCODE_J].s_ascii = 'J';

    keys[PS2K_KEYCODE_K].pressed = 0;
    keys[PS2K_KEYCODE_K].ascii = 'k';
    keys[PS2K_KEYCODE_K].s_ascii = 'K';

    keys[PS2K_KEYCODE_L].pressed = 0;
    keys[PS2K_KEYCODE_L].ascii = 'l';
    keys[PS2K_KEYCODE_L].s_ascii = 'L';

    keys[PS2K_KEYCODE_SEMICOLON].pressed = 0;
    keys[PS2K_KEYCODE_SEMICOLON].ascii = ';';
    keys[PS2K_KEYCODE_SEMICOLON].s_ascii = ':';

    keys[PS2K_KEYCODE_SINGLEQUOTE].pressed = 0;
    keys[PS2K_KEYCODE_SINGLEQUOTE].ascii = '\'';
    keys[PS2K_KEYCODE_SINGLEQUOTE].s_ascii = '"';

    keys[PS2K_KEYCODE_BACKSLASH].pressed = 0;
    keys[PS2K_KEYCODE_BACKSLASH].ascii = '\\';
    keys[PS2K_KEYCODE_BACKSLASH].s_ascii = '|';

    keys[PS2K_KEYCODE_LSHIFT].pressed = 0;
    keys[PS2K_KEYCODE_LSHIFT].ascii = 0;
    keys[PS2K_KEYCODE_LSHIFT].s_ascii = 0;

    keys[PS2K_KEYCODE_Z].pressed = 0;
    keys[PS2K_KEYCODE_Z].ascii = 'z';
    keys[PS2K_KEYCODE_Z].s_ascii = 'Z';

    keys[PS2K_KEYCODE_X].pressed = 0;
    keys[PS2K_KEYCODE_X].ascii = 'x';
    keys[PS2K_KEYCODE_X].s_ascii = 'X';

    keys[PS2K_KEYCODE_C].pressed = 0;
    keys[PS2K_KEYCODE_C].ascii = 'c';
    keys[PS2K_KEYCODE_C].s_ascii = 'C';

    keys[PS2K_KEYCODE_V].pressed = 0;
    keys[PS2K_KEYCODE_V].ascii = 'v';
    keys[PS2K_KEYCODE_V].s_ascii = 'V';

    keys[PS2K_KEYCODE_B].pressed = 0;
    keys[PS2K_KEYCODE_B].ascii = 'b';
    keys[PS2K_KEYCODE_B].s_ascii = 'B';

    keys[PS2K_KEYCODE_N].pressed = 0;
    keys[PS2K_KEYCODE_N].ascii = 'n';
    keys[PS2K_KEYCODE_N].s_ascii = 'N';

    keys[PS2K_KEYCODE_M].pressed = 0;
    keys[PS2K_KEYCODE_M].ascii = 'm';
    keys[PS2K_KEYCODE_M].s_ascii = 'M';

    keys[PS2K_KEYCODE_COMA].pressed = 0;
    keys[PS2K_KEYCODE_COMA].ascii = ',';
    keys[PS2K_KEYCODE_COMA].s_ascii = '<';

    keys[PS2K_KEYCODE_DOT].pressed = 0;
    keys[PS2K_KEYCODE_DOT].ascii = '.';
    keys[PS2K_KEYCODE_DOT].s_ascii = '>';

    keys[PS2K_KEYCODE_SLASH].pressed = 0;
    keys[PS2K_KEYCODE_SLASH].ascii = '/';
    keys[PS2K_KEYCODE_SLASH].s_ascii = '?';

    keys[PS2K_KEYCODE_RSHIFT].pressed = 0;
    keys[PS2K_KEYCODE_RSHIFT].ascii = 0;
    keys[PS2K_KEYCODE_RSHIFT].s_ascii = 0;

    keys[PS2K_KEYCODE_LCTRL].pressed = 0;
    keys[PS2K_KEYCODE_LCTRL].ascii = 0;
    keys[PS2K_KEYCODE_LCTRL].s_ascii = 0;

    keys[PS2K_KEYCODE_LALT].pressed = 0;
    keys[PS2K_KEYCODE_LALT].ascii = 0;
    keys[PS2K_KEYCODE_LALT].s_ascii = 0;

    keys[PS2K_KEYCODE_SPACE].pressed = 0;
    keys[PS2K_KEYCODE_SPACE].ascii = 32;
    keys[PS2K_KEYCODE_SPACE].s_ascii = 32;

    keys[PS2K_KEYCODE_UP].pressed = 0;
    keys[PS2K_KEYCODE_UP].ascii = PS2K_UP_MAP_ASCII;
    keys[PS2K_KEYCODE_UP].s_ascii = PS2K_UP_MAP_ASCII;

    keys[PS2K_KEYCODE_DOWN].pressed = 0;
    keys[PS2K_KEYCODE_DOWN].ascii = PS2K_DOWN_MAP_ASCII;
    keys[PS2K_KEYCODE_DOWN].s_ascii = PS2K_DOWN_MAP_ASCII;

    keys[PS2K_KEYCODE_LEFT].pressed = 0;
    keys[PS2K_KEYCODE_LEFT].ascii = PS2K_LEFT_MAP_ASCII;
    keys[PS2K_KEYCODE_LEFT].s_ascii = PS2K_LEFT_MAP_ASCII;

    keys[PS2K_KEYCODE_RIGHT].pressed = 0;
    keys[PS2K_KEYCODE_RIGHT].ascii = PS2K_RIGHT_MAP_ASCII;
    keys[PS2K_KEYCODE_RIGHT].s_ascii = PS2K_RIGHT_MAP_ASCII;
}

void ps2k_handle_irq1(uint8_t keycode) {
    if (keycode < 0x81) {
        keys[keycode].pressed = 1;
        if (is_buffering && keys[keycode].ascii != 0) {
            if (ps2k_is_pressed(PS2K_KEYCODE_LSHIFT) || ps2k_is_pressed(PS2K_KEYCODE_RSHIFT)) {
                ps2k_in_buffer_add(keys[keycode].s_ascii);
            } else {
                ps2k_in_buffer_add(keys[keycode].ascii);
            }
        }
    } else {
        keys[keycode - 0x80].pressed = 0;
    }
}

int ps2k_is_pressed(uint8_t keycode) {
    return keys[keycode].pressed;
}

void ps2k_start_buffering() {
    is_buffering = 1;
}

void ps2k_stop_buffering() {
    is_buffering = 0;
}

char ps2k_get_char() {
    char c = ps2k_in_buffer_get();
    return c;
}

void ps2k_in_buffer_add(char c) {
    unsigned int diff = 0;
    if (in_buffer_start < in_buffer_end) {
        diff = in_buffer_end - in_buffer_start;
    } else {
        diff = (PS2K_IN_BUFFER_SIZE - in_buffer_start + in_buffer_end) % PS2K_IN_BUFFER_SIZE;
    }
    if (diff < PS2K_IN_BUFFER_SIZE - 1) {
        in_buffer[in_buffer_end] = c;
        in_buffer_end = (in_buffer_end + 1) % PS2K_IN_BUFFER_SIZE;
    }
}

char ps2k_in_buffer_get() {
    char c;
    if (in_buffer_start == in_buffer_end) {
        c = EOF;
    } else {
        c = in_buffer[in_buffer_start];
        in_buffer_start = (in_buffer_start + 1) % PS2K_IN_BUFFER_SIZE;
    }
    return c;
}
