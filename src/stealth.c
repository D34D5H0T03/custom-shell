#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>

#include "stealth.h"

void mask_process(char** argv, const char* fake_name){
    //modify the kernel thread name
    prctl(PR_SET_NAME, fake_name, 0, 0, 0);

    size_t original_len = strlen(argv[0]);
    size_t fake_len = strlen(fake_name);

    memset(argv[0], 0, original_len); //wipe the original name

    if (fake_len > original_len) {
        strncpy(argv[0], fake_name, original_len);
    } else {
        strncpy(argv[0], fake_name, fake_len);
    }
}