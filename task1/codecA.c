#include <ctype.h>

void encode(char *message) {
    int i = 0;
    while (message[i]) {
        if (islower(message[i])) {
            message[i] = toupper(message[i]);
        } else if (isupper(message[i])) {
            message[i] = tolower(message[i]);
        }
        i++;
    }
}

// In codecA, the implementation of encode and decode is same
void decode(char *message) {
    encode(message);
}
