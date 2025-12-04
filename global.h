#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

typedef struct list
{
    struct list *next;
    struct list *prev;   
}list_t, *list_p;

typedef struct 
{
    char username[20];
    char password[20];
    list_t list;
}user_t, *user_p;

typedef struct
{
    char id[20];
    char bookname[30];
    char author[20];
    char publisher[30];
    char publishyear[10];
    int num;
    list_t list;
}book_t, *book_p;

typedef struct
{
    char id[20];
    char username[20];
    char bookname[30];
    char status[20];
    char borrowtime[30];
    char returntime[30];
    char remindertime[30];
    list_t list;
}BorrowRecord_t, *BorrowRecord_p;

#ifndef _EXTERN_
#define _EXTERN_
extern user_p head;
extern book_p headbook;
extern BorrowRecord_p headborrow;
extern char USER[20];
extern void __initBook(book_p book,const char*id, const char* bookname, const char* author, 
                        const char* publisher, char* publishyear, int num);
extern void ListAdd(list_p new,list_p head);
extern user_p UserCreat(char *username,char *password);
extern void __initBorrowRecord(BorrowRecord_p borrowrecord, const char* id, const char* username, 
                        const char* bookname, const char* status,const char* borrowtime, 
                        const char* returntime, const char* remindertime);
#endif

#ifndef _LIST_ENTRY_
#define _LIST_ENTRY_
/*将大小结构体转化*/
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long long)(&((type *)0)->member)))
/*book_t char 类型成员排序*/
#define sort_choice(member)\
for (int i = 0; i < len-1; i++){\
    tmp = headbook->list.next;  \
    prev = &headbook->list;\
    for (int j = 0; j < len-1-i; j++){\
        ptr = list_entry(tmp, book_t, list);\
        ptrnext = list_entry(tmp->next, book_t, list);\
        if ((strcoll(ptr->member, ptrnext->member)>0&&choice2=='1')||\
            (strcoll(ptr->member, ptrnext->member)<0&&choice2=='2')){\
            next_node = tmp->next; \
            tmp->next = next_node->next;  \
            if (tmp->next != &headbook->list) {  \
                tmp->next->prev = tmp;\
            }else {\
                headbook->list.prev = tmp;\
            }\
            next_node->prev = prev;\
            next_node->next = tmp;\
            prev->next = next_node;\
            tmp->prev = next_node;\
            prev = next_node;\
        }else{\
            prev = tmp;\
            tmp = tmp->next;\
        }\
    }\
}\
/*book_t int 类型成员排序*/
#define sort_choice2(num)\
for (int i = 0; i < len-1; i++){\
    tmp = headbook->list.next;  \
    prev = &headbook->list;\
    for (int j = 0; j < len-1-i; j++){\
        ptr = list_entry(tmp, book_t, list);\
        ptrnext = list_entry(tmp->next, book_t, list);\
        if( (ptr->num>ptrnext->num&&choice2=='1')||\
            (ptr->num<ptrnext->num&&choice2=='2')){\
            next_node = tmp->next; \
            tmp->next = next_node->next;  \
            if (tmp->next != &headbook->list) {  \
                tmp->next->prev = tmp;\
            }else {\
                headbook->list.prev = tmp;\
            }\
            next_node->prev = prev;\
            next_node->next = tmp;\
            prev->next = next_node;\
            tmp->prev = next_node;\
            prev = next_node;\
        }else{\
            prev = tmp;\
            tmp = tmp->next;\
        }\
    }\
}\

#endif

#define Write2File(filename, head, struct_type, fmt, ...) \
do { \
    FILE *fp = fopen(filename, "w"); \
    if (fp == NULL) { \
        perror("fopen failed"); \
        exit(EXIT_FAILURE); \
    } \
    list_p tmp; \
    for (tmp = (head)->list.next; tmp != &(head)->list; tmp = tmp->next) { \
        struct_type *node = list_entry(tmp, struct_type, list); \
        fprintf(fp, fmt, ##__VA_ARGS__); \
    } \
    fclose(fp); \
} while (0)\


#define ReadFromFile(filename, head, struct_type, argc, fmt, ...) \
do { \
    FILE *fp = fopen(filename, "r"); \
    if (fp == NULL) { \
        perror("fopen failed (ReadFromFile)"); \
        return NULL;  \
    } \
    char line[256]; \
    while (fgets(line, sizeof(line), fp) != NULL) { \
        line[strcspn(line, "\n")] = '\0';  \
        struct_type tmp; \
        int sscanf_ret = sscanf(line, fmt, ##__VA_ARGS__); \
        if (sscanf_ret == argc) { \
            struct_type *new_node = (struct_type *)malloc(sizeof(struct_type)); \
            if (new_node == NULL) { \
                perror("malloc failed"); \
                fclose(fp); \
                return NULL; \
            } \
                __initBook(new_node, tmp.id, tmp.bookname, tmp.author, tmp.publisher, tmp.publishyear, tmp.num); \
            ListAdd(&new_node->list, &(head)->list);  \
        } else { \
            printf("解析失败：%s（预期 %d 个字段，实际 %d 个）\n", line, argc, sscanf_ret); \
        } \
    } \
    fclose(fp); \
} while(0)\


#define ReadFromBorFile(filename, head, struct_type, argc, fmt, ...) \
do { \
    FILE *fp = fopen(filename, "r"); \
    if (fp == NULL) { \
        perror("fopen failed (ReadFromFile)"); \
        return NULL;  \
    } \
    char line[256]; \
    while (fgets(line, sizeof(line), fp) != NULL) { \
        line[strcspn(line, "\n")] = '\0';  \
        struct_type tmp; \
        int sscanf_ret = sscanf(line, fmt, ##__VA_ARGS__); \
        if (sscanf_ret == argc) { \
            struct_type *new_node = (struct_type *)malloc(sizeof(struct_type)); \
            if (new_node == NULL) { \
                perror("malloc failed"); \
                fclose(fp); \
                return NULL; \
            } \
            __initBorrowRecord(new_node, tmp.id, tmp.username, tmp.bookname, tmp.status, tmp.borrowtime, tmp.returntime, tmp.remindertime); \
            ListAdd(&new_node->list, &(head)->list);  \
        } else { \
            printf("解析失败：%s（预期 %d 个字段，实际 %d 个）\n", line, argc, sscanf_ret); \
        } \
    } \
    fclose(fp); \
} while(0)\


#endif