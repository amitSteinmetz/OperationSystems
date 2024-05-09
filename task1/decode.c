#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    // Usage should be with 3 argumants in command line
    if (argc != 3) 
    {
        printf("Usage: %s <codec> <message>\n", argv[0]);
        return 1;
    }
    
    // Store input argumants in variables
    char *codec_lib = argv[1];
    char *message = argv[2];
    
    /* Load requested library */
    void *handle = dlopen(codec_lib, RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "Failed to open library: %s\n", dlerror());
        return 1;
    }

    /* get the address of "decode" function using dlsym */
    void (*decode_pfunc)(char*); 
    decode_pfunc = dlsym(handle, "decode");
    
    char *error = NULL; // If something fails - error will contain an error message
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "Failed to find decode function: %s\n", error);
        dlclose(handle);
        return 1;
    }

    // Call the function
    (*decode_pfunc)(message);

    // Print the result
    printf("%s\n", message);

    dlclose(handle);
    
    return 0;
}