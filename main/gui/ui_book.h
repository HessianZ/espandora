//
// Created by Hessian on 2023/7/25.
//

#ifndef ESPANDORA_UI_BOOK_H
#define ESPANDORA_UI_BOOK_H

#define BOOK_NAME_MAX_LEN 64

typedef struct {
    char name[BOOK_NAME_MAX_LEN];
    char path[32];
} BookInfo;


void ui_book_start(void (*fn)(void));

#endif //ESPANDORA_UI_BOOK_H
