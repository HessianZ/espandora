//
// Created by Hessian on 2023/7/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *cnNums[] = {
        "零",
        "一",
        "二",
        "三",
        "四",
        "五",
        "六",
        "七",
        "八",
        "九",
};

static const char *cnNumUnits[] = {
        "万",
        "千",
        "百",
        "十",
        ""
};

void num2cn(int number, char* dest)
{
    if (number < 0 || number > 10000) {
        strcpy(dest, "数字超出范围");
        return;
    }
    char num_str[10];
    sprintf(&num_str, "%d", number);
    char *ns = &num_str;
    for (int i = 0, len = strlen(ns); i < len; ++i) {
        int num = ns[i] - 0x30; // ASCII 0-9 = 30-39
        strcat(dest, cnNums[num]);
        strcat(dest, cnNumUnits[5 - len + i]);
    }
}
