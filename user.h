#ifndef _USER_H_
#define _USER_H_

#include "global.h"

user_p UserInit();
book_p BookInit();
void UserUI();
int loginUser();
void registerUser();
void UserOperate();
void FindBook();
void BorrowBook();
void ReturnBook();
void BorrowRecordFind();
void ListAdd(list_p new,list_p head);
user_p UserCreat(char *username,char *password);
BorrowRecord_p BorrowRecordInit();
void __initBook(book_p book,const char*id, const char* bookname, const char* author,
                 const char* publisher, char* publishyear, int num);
void __initBorrowRecord(BorrowRecord_p borrowrecord, const char* id, const char* username, 
                        const char* bookname, const char* status,const char* borrowtime, 
                        const char* returntime, const char* remindertime);


#endif