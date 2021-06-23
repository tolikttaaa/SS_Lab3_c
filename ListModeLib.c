//
// Created by ttaaa on 6/3/21.
//
#include "com_ifmo_ttaaa_ss_lab3_app_ListModeLib.h"
#include "list_mode.h"

JNIEXPORT jstring JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ListModeLib_runListMode(JNIEnv *jenv, jobject jobj) {
    (void)&jobj;
    char *msg = run_list_mode();
    return (*jenv)->NewStringUTF(jenv, msg);
}
