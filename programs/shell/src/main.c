#include <simplstd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_COMMAND_SIZE 100
#define MAX_PARAM_SIZE 100
#define DELIMITER_CHAR ' '

#define COMMAND_STR_EXEC "exec"

const char *program_names[prog_id_count] = {
    "shell",
    "test2",
    "input",
    "test",
    "testcase1",
    "testcase2",
    "testcase3",
    "testcase3_1",
    "testcase4",
    "testcase4_1",
    "testcase4_2",
    "testcase5",
    "testcase5_1",
    "testcase5_2",
    "testcase5_3",
    "testcase6",
    "testcase7",
    "testcase7_1",
    "testcase8",
    "testcase9",
};

char *prompt = ">";

uint32_t get_char_index(const char *const str, const char targetChar);
void parse_command(const char *userInputStr);

void main() {
    char in_buf[STDIN_BUFFER_SIZE];
    printf("----------------- New shell session -----------------\n");
    while (1) {
        printf("%s ", prompt);
        gets(in_buf);
        parse_command(in_buf);
        wait();
    }
}

uint32_t get_char_index(const char *const str, const char targetChar) {
    uint32_t charIndex;
    for (charIndex = 0; str[charIndex] != '\0'; charIndex++) {
        if (str[charIndex] == targetChar) {
            break;
        }
    }
    return charIndex;
}

void handle_command_exec(const char *params) {
    const char *currentParamPtr = params;
    char currentParamStr[MAX_PARAM_SIZE];
    uint32_t delimiterIndex;
    bool is_program_name_found;
    while (1) {
        is_program_name_found = false;
        delimiterIndex = get_char_index(currentParamPtr, DELIMITER_CHAR);
        memcpy(currentParamStr, currentParamPtr, delimiterIndex);
        currentParamStr[delimiterIndex] = 0;
        for (int prog_id = 0; prog_id < prog_id_count; prog_id++) {
            if (strcmp(program_names[prog_id], currentParamStr) == 0) {
                is_program_name_found = true;
                exec(prog_id);
                break;
            }
        }
        if (is_program_name_found == false) {
            printf("ERROR: Invalid program name '%s' (ignoring)\n", currentParamStr);
        }
        if (currentParamPtr[delimiterIndex] == 0) {
            break;
        }
        currentParamPtr += delimiterIndex + 1;
    }
}

void parse_command(const char *userInputStr) {
    char commandStr[MAX_COMMAND_SIZE];

    uint32_t delimiterIndex = get_char_index(userInputStr, DELIMITER_CHAR);

    memcpy(commandStr, userInputStr, delimiterIndex);
    commandStr[delimiterIndex] = 0;

    if (strcmp(commandStr, COMMAND_STR_EXEC) == 0) {
        if (userInputStr[delimiterIndex] != 0) {
            handle_command_exec(userInputStr + delimiterIndex + 1);
        } else {
            printf("ERROR: Invalid number of parameters for command '%s' (ignoring)\n", commandStr);
        }
    } else {
        printf("ERROR: Invalid command '%s' (ignoring)\n", commandStr);
    }
}
