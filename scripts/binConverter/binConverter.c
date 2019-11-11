#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BIN_SIZE 102400
#define OUTPUT_BUFFER_NAME "elf_buffer"
#define OUTPUT_BUFFER_SIZE "elf_size"
#define OUTPUT_SRC_PATH "kernel/src/elf_"
#define OUTPUT_HEADER_PATH "kernel/include/elf/elf_"
#define INPUT_BIN_PREPATH "programs/"
#define BYTES_PER_LINE 10

uint8_t binBuffer[102400];

int main(int argc, char **argv) {
    const char *binName = "prog";
    char devosRepoPath[255];
    if (!getenv("DEVOS_REPODIR")) {
        printf("ERROR: Devos repo dir not specified in env variable.\nUse create_env_variables.sh script\n");
        return 6;
    }
    strcpy(devosRepoPath, getenv("DEVOS_REPODIR"));
    strcat(devosRepoPath, "/");

    if (argc < 2) {
        printf("ERROR: Invalid number of arguments.\nSpecify target elf binary\n");
        return 1;
    } else if (argc > 2) {
        binName = argv[2];
    }

    //open target binary file
    char binPath[256];
    strcpy(binPath, devosRepoPath);
    strcat(binPath, INPUT_BIN_PREPATH);
    strcat(binPath, argv[1]);
    FILE *binFile;
    binFile = fopen(binPath, "rb");  // r for read, b for binary
    if (!binFile) {
        printf("ERROR: Failed to open target elf binary file\n");
        return 3;
    }

    //check size
    fseek(binFile, 0L, SEEK_END);
    uint32_t binSize = ftell(binFile);
    fseek(binFile, 0L, SEEK_SET);

    if (binSize >= MAX_BIN_SIZE) {
        printf("ERROR: Target binary file size is to big\n");
        return 2;
    }

    //read file content
    fread(binBuffer, sizeof(binBuffer), 1, binFile);

    //close binary file
    fclose(binFile);

    //open output source file
    FILE *outputFile;
    char outputFileName[256];
    strcpy(outputFileName, devosRepoPath);
    strcat(outputFileName, OUTPUT_SRC_PATH);
    strcat(outputFileName, binName);
    strcat(outputFileName, ".c");
    outputFile = fopen(outputFileName, "w");  // r for read, b for binary
    if (!outputFile) {
        printf("ERROR: Failed to open output source file\n");
        return 4;
    }

    //write output file header
    fprintf(outputFile, "#include <stdint.h>\n\nconst uint32_t %s_" OUTPUT_BUFFER_SIZE " = %u;\nconst uint8_t %s_" OUTPUT_BUFFER_NAME "[%d] = {",
            binName, binSize, binName, binSize);

    //write binary content
    for (int i = 0; i < binSize - 1; i++) {
        if (i % BYTES_PER_LINE == 0) {
            fprintf(outputFile, "\n\t");
        }
        fprintf(outputFile, "0x%02x, ", binBuffer[i]);
    }

    fprintf(outputFile, "0x%02x", binBuffer[binSize - 1]);

    //write file footer
    fprintf(outputFile, "};\n");

    //close output source file
    fclose(outputFile);

    //open output header file
    strcpy(outputFileName, devosRepoPath);
    strcat(outputFileName, OUTPUT_HEADER_PATH);
    strcat(outputFileName, binName);
    strcat(outputFileName, ".h");
    outputFile = fopen(outputFileName, "w");  // r for read, b for binary
    if (!outputFile) {
        printf("ERROR: Failed to open output header file\n");
        return 4;
    }

    fprintf(outputFile, "#pragma once\n#include <stdint.h>\n\nextern const uint32_t %s_" OUTPUT_BUFFER_SIZE ";\nextern const uint8_t %s_" OUTPUT_BUFFER_NAME ";\n", binName, binName);

    //close output header file
    fclose(outputFile);
    return 0;
}