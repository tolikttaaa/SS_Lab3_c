//
// Created by ttaaa on 6/3/21.
//

#include "com_ifmo_ttaaa_ss_lab3_app_ScriptModeLib.h"
#include "script_mode.h"
#include "fat32_lib.h"

JNIEXPORT jlong JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_getPartition (JNIEnv *jenv, jobject jobj, jstring path) {
    (void)&jobj;
    struct partition_value *partition = get_partition((char *) (*jenv)->GetStringUTFChars(jenv, path, 0));
    return (long) partition;
}

JNIEXPORT jstring JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_lsCommand(JNIEnv *jenv, jobject jobj, jlong partPointer) {
    (void)&jobj;
    struct partition_value *partition = (struct partition_value *) (long) partPointer;
    char *msg = ls_command(partition);
    return (*jenv)->NewStringUTF(jenv, msg);
}

JNIEXPORT jint JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_cdCommand(JNIEnv *jenv, jobject jobj, jlong partPointer, jstring to) {
    (void)&jobj;
    struct partition_value *partition = (struct partition_value *) (long) partPointer;
    int res = cd_command(partition, (char *) (*jenv)->GetStringUTFChars(jenv, to, 0));
    return res;
}

JNIEXPORT jint JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_cpCommand(JNIEnv *jenv, jobject jobj, jlong partPointer, jstring source, jstring to) {
    (void)&jobj;
    struct partition_value *partition = (struct partition_value *) (long) partPointer;
    int res = cp_command(partition, (char *) (*jenv)->GetStringUTFChars(jenv, source, 0), (char *) (*jenv)->GetStringUTFChars(jenv, to, 0));
    return res;
}

JNIEXPORT jstring JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_helpCommand(JNIEnv * jenv, jobject jobj) {
    (void)&jobj;
    char *msg = help_command();
    return (*jenv)->NewStringUTF(jenv, msg);
}

JNIEXPORT void JNICALL Java_com_ifmo_ttaaa_ss_1lab3_app_ScriptModeLib_exitCommand(JNIEnv *jenv, jobject jobj, jlong partPointer) {
    (void)&jenv;
    (void)&jobj;
    struct partition_value *partition = (struct partition_value *) (long) partPointer;
    close_partition(partition);
}
