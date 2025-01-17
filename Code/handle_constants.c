#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "handle_constants.h"

// David Garcia, NIA: 251587

int is_constant_valid(const char* string) {
    for (size_t i = 0; i < strlen(string); i++) {

        //For each character, we look if it is an alphanumeric character or a lower bar (_)
        if (!(isalnum(string[i]) || string[i] == '_')) {            
            //The isalnum function will return 1 if the character in string[i] is an alphanumeric character, and 0 otherwise
            return 0; // Not a valid constant
        }
    }
    return 1; // Valid constant
}

char* handle_constants(char* source_code, size_t* size_source_code) {
    
    
    char* outputCode = (char *)malloc(*size_source_code * sizeof(char));
    strcpy(outputCode, source_code);  // Copy the input source code into the outputCode variable

    // Create a separate variable for tokenization
    char* tokenizedCode = (char *)malloc(*size_source_code * sizeof(char));
    strcpy(tokenizedCode, outputCode);
    
    // We tokenize the source code by lines, and then iterate through these lines
    char *line = strtok(tokenizedCode, "\n");

    while (line != NULL) {

        // Check if the line contains #define
        if (strncmp(line, "#define", 7) == 0) {
            char *token = strtok(line, " ");
            // We get the identifier after the #define found
            token = strtok(NULL, " ");

            /*We check if the token extracted after the #define is not NULL (so there's an identifier after the #define) 
            and a valid constant, using the is_constant_valid function */
            if (token != NULL && is_constant_valid(token)) {
                // This makes that only the lines started with #define and a valid constant are processed 
                char *constant_value = strtok(NULL, "\n");

                // Then, we replace all occurrences of the valid constant in the code
                if (constant_value != NULL) {
                    char *occurrence = strstr(outputCode, token);
                    
                    // We iterate through the occurrences of each constant to replace them with their value
                    while (occurrence != NULL) {

                        //We use the memmove and memcpy functions to replace the values correctly, so no overlapping errors happen
                        /*The memmove function makes space for the replacement value and has the parameters:
                        - destination pointer: Position just after the constant value to replace
                        - source pointer: The position after the original constant identifier in the code
                        - number of bytes to move.
                        */
                        memmove(occurrence + strlen(constant_value), occurrence + strlen(token), *size_source_code - (occurrence - outputCode) - strlen(token));
                        
                        //We copy the replacement value into the space generated by the memmove function above
                        memcpy(occurrence, constant_value, strlen(constant_value));

                        // To find the next occurrence
                        occurrence = strstr(occurrence + strlen(constant_value), token);
                    }
                    // We remove the #define line from the code
                    memmove(line, line + strlen(line) + 1, *size_source_code - (line - outputCode) - strlen(line));
                    *size_source_code -= strlen(line) + 1;
                }
            }
        }
        // Move to the next line
        line = strtok(NULL, "\n");
    }
    free(tokenizedCode);

    
    return outputCode;
}
