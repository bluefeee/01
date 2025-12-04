#include "admin.h"
#include "user.h"
#include "global.h"


void MainUI();

int main()
{
    setlocale(LC_ALL, ""); 
    MainUI();
    return 0;
}

void MainUI(){
    static char choice;
    UserInit();
    BookInit();
    BorrowRecordInit();
    while (1)
    {
        printf("============图书管理系统============\n");
        printf("请选择：\n");
        printf("1. 用户登录\n");
        printf("2. 管理员登录\n");
        printf("3. 退出\n");
        scanf("%s", &choice);
        switch(choice){
            case '1':
                UserUI();
                break;
            case '2':
                AdminUI();
                break;
            
            case '3':
                return;
                break;

            default:
                printf("没有该选项，请重新输入：\n");
                break;
        } 
    }
}
