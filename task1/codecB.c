void encode(char *message)
{
    int i = 0;
    while (message[i]) {
        message[i] = (message[i] + 3) % 255; // there are 255 chars
        i++;
    }
}

void decode(char *message) {
    int i = 0;
    while (message[i]) {
        message[i] = (message[i] - 3) % 255; // there are 255 chars
        i++;
    }
}

