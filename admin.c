#include "admin.h"

int AdminLogin(){
    char username[20];
    char password[20];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    if( !strcmp(username, "1") && !strcmp(password, "1")){
        return 1;
    }else{
        printf("用户名或密码错误\n");
        return 0;
    }
}

void Sort(char choice, char choice2){
    book_p ptr,ptrnext;
    list_p tmp = headbook->list.next;
    list_p prev = &headbook->list;
    list_p next_node;
    int len = 0;
    while (tmp != &headbook->list){
        len++;
        tmp = tmp->next;
    }
    if (len < 2){
        return;
    }
    // printf("len = %d\n",len);
    if (choice == '1'){
        sort_choice(bookname);
    }
    else if (choice == '2'){
        sort_choice(author);
    }
    else if (choice == '3'){
        sort_choice(publisher);
    }
    else if (choice == '4'){
        sort_choice(publishyear);
    }
    else if (choice == '5'){
        sort_choice2(num);
    }
    else if (choice == '6'){
        sort_choice2(id);
    }
    else{
        printf("输入错误\n");
        return;
    }
}

void BookShow(){
    printf("============图书信息============\n");
    book_p ptr;
    list_p tmp = headbook->list.next;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        printf("编号：%s\n书名：%s\n作者：%s\n出版社：%s\n出版年份：%s\n库存：%d\n",
            ptr->id,ptr->bookname,ptr->author,ptr->publisher,ptr->publishyear,ptr->num);
        printf("------------------------------\n");
        tmp = tmp->next;
    }
    printf("选择排序方式：\n1.按书名排序\n2.按作者排序\n3.按出版社排序\n4.按出版年份排序\n5.按库存排序\n6.按编号排序\n(任意键退出)");
    char choice[2], choice2[2];
    scanf("%s", choice);  
    if (choice[0]<'1'||choice[0]>'6')
        return;
    printf("1.升序\n2.降序\n");
    scanf("%s", choice2); 
    if (choice2[0]>'0'&&choice2[0]<'3'){       
        Sort(choice[0],choice2[0]);
    }
    else{
        return;
    }
    
    printf("\n============图书信息（排序后）============\n");
    tmp = headbook->list.next;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        printf("编号：%s\n书名：%s\n作者：%s\n出版社：%s\n出版年份：%s\n库存：%d\n",
            ptr->id,ptr->bookname,ptr->author,ptr->publisher,ptr->publishyear,ptr->num);
        printf("------------------------------\n");
        tmp = tmp->next;
    }
    return;
}

void BookDelete(){
    printf("============图书信息删除============\n");
    printf("请输入要删除的图书编号：");
    char id[20];
    scanf("%s", id);
    book_p ptr;
    BorrowRecord_p ptrborrow;
    list_p tmp = headborrow->list.next;
    list_p prev = &headbook->list;
    while (tmp != &headborrow->list){
        ptrborrow = list_entry(tmp, BorrowRecord_t, list);
        if (!strcmp(ptrborrow->id,id)&&!strcmp(ptrborrow->status,"借阅中")){
            printf("该图书存在借阅信息：\n");
            printf("借阅人：%s\t书名：%s\t借阅日期：%s\n",
                ptrborrow->username,ptrborrow->bookname,ptrborrow->borrowtime);
            printf("请先处理借阅信息\n");
            return;
        }
        tmp = tmp->next;
    }
    tmp = headbook->list.next;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        if (!strcmp(ptr->id,id)){   
            prev->next = tmp->next;
            tmp->next->prev = prev;
            // tmp->next = NULL;
            // tmp->prev = NULL;
            free(ptr);
            printf("删除成功\n");
            Write2File("book.txt", headbook,book_t,"%s %s %s %s %s %d\n",
                        node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    printf("删除失败,请检查输入是否正确\n");
}

void BookAdd(){
    printf("============图书信息录入============\n");
    char id[20];
    book_p ptr;
    list_p tmp = headbook->list.next;
    book_p book = (book_p)malloc(sizeof(book_t));
    printf("请输入图书编号：");
    scanf("%s", book->id);
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        if (!strcmp(ptr->id,book->id)){
            printf("该图书已存在，请直接在更新界面修改库存：\n");
            return;
        }
        tmp = tmp->next;
    }

    printf("请输入书名：");
    scanf("%s", book->bookname);
    printf("请输入作者：");
    scanf("%s", book->author);
    printf("请输入出版社：");
    scanf("%s", book->publisher);
    printf("请输入出版年份：");
    scanf("%s", book->publishyear);
    printf("请输入库存：");
    scanf("%d", &book->num);
    __initBook(book, book->id, book->bookname, book->author, book->publisher, book->publishyear, book->num);
    ListAdd(&book->list,&headbook->list);
    Write2File("book.txt", headbook,book_t,"%s %s %s %s %s %d\n",
                node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
}

void __bookupdate(book_p ptr){
    static char choice;
    scanf("%s", &choice);
    while (1)
    {
        switch(choice){
            case '1':
                printf("请输入新的书名：");
                scanf("%s", ptr->bookname);
                printf("修改成功,新的书名为%s\n",ptr->bookname);
                return;
            case '2':
                printf("请输入新的作者：");
                scanf("%s", ptr->author);
                printf("修改成功,新的作者为%s\n",ptr->author);
                return;
            case '3':
                printf("请输入新的出版社：");
                scanf("%s", ptr->publisher);
                printf("修改成功,新的出版社为%s\n",ptr->publisher);
                return;
            case '4':
                printf("请输入新的出版年份：");
                scanf("%s", ptr->publishyear);
                printf("修改成功,新的出版年份为%s\n",ptr->publishyear);
                return;
            case '5':
                printf("请输入新的库存：");
                scanf("%d", &ptr->num); 
                printf("修改成功,新的库存为%d\n",ptr->num);
                return;
            case '0':
                return;
            default:
                printf("输入错误，请重新输入\n");
                break;  
        }
    }
}

void BookUpdate(){
    printf("============图书信息更新============\n");
    book_p ptr;
    printf("请输入要更新的图书编号：");
    char id[20];
    scanf("%s", id);
    BorrowRecord_p ptrborrow;
    list_p tmp = headborrow->list.next;
    while (tmp != &headborrow->list){
        ptrborrow = list_entry(tmp, BorrowRecord_t, list);
        if (!strcmp(ptrborrow->id,id)&&!strcmp(ptrborrow->status,"借阅中")){
            printf("该图书存在借阅信息：\n");
            printf("借阅人：%s\t书名：%s\t借阅日期：%s\n",
                ptrborrow->username,ptrborrow->bookname,ptrborrow->borrowtime);
            printf("请先处理借阅信息\n");
            return;
        }
        tmp = tmp->next;
    }
    tmp = headbook->list.next;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        if( !strcmp(ptr->id,id)){
            printf("书籍信息：\n");
            printf("编号：%s,书名：%s\n作者：%s\n出版社：%s\n出版年份：%s\n库存：%d\n",
                ptr->id,ptr->bookname,ptr->author,ptr->publisher,ptr->publishyear,ptr->num);
            printf("请输入修改序号(1.书名 2.作者 3.出版社 4.出版年份 5.库存 0.退出)：");
            __bookupdate(ptr);
            Write2File("book.txt", headbook,book_t,"%s %s %s %s %s %d\n",
                        node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
            return;
        }
        tmp = tmp->next;
    }
    printf("没有找到该书籍,请检查查询信息是否输入正确\n");
}

void UserShow(){
    printf("============用户信息============\n");
    user_p ptr;
    list_p tmp = head->list.next;
    while (tmp != &head->list){
        ptr = list_entry(tmp, user_t, list);
        printf("用户名：%s\n密码：%s\n\n",
            ptr->username,ptr->password);
        tmp = tmp->next;
    }   
}

void UserAdd(){
    printf("============用户添加============\n");
    printf("请输入用户名：");
    char username[20];
    scanf("%s", username);
    printf("请输入密码：");
    char password[20];
    scanf("%s", password);
    user_p user = UserCreat(username,password);
    ListAdd(&user->list,&head->list);
}

void UserDelete(){
    printf  ("============用户删除============\n");
    printf("请输入要删除的用户名：");
    char username[20];
    scanf("%s", username);
    fflush(stdin);
    user_p ptr;
    list_p tmp = head->list.next;
    list_p prev = &head->list;
    while (tmp != &head->list){
        ptr = list_entry(tmp, user_t, list);
        if (!strcmp(ptr->username,username)){
            prev->next = tmp->next;printf("1\n");
            tmp->next->prev = prev;
            // tmp->next = NULL;
            // tmp->prev = NULL;
            free(ptr);printf("1\n");
            printf("删除成功\n");
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    printf("没有找到该用户,请检查查询信息是否输入正确\n");
}

void UserUpdate(){
    printf("============用户更新============\n");
    printf("请输入要更新的用户名：");
    char username[20];
    scanf("%s", username);
    user_p ptr;
    list_p tmp = head->list.next;
    list_p prev = &head->list;
    while (tmp != &head->list){
        ptr = list_entry(tmp, user_t, list);
        if (!strcmp(ptr->username,username)){
            printf("请输入新的密码：");
            scanf("%s", ptr->password);
            printf("修改成功\n");
            return;
        }
        tmp = tmp->next;
    }
    printf("没有找到该用户,请检查查询信息是否输入正确\n");
}

void UserManage(){
    while (1)
    {
        printf("============用户管理============\n");
        printf("请选择：\n");
        printf("1. 查看用户信息\n");
        printf("2. 新用户添加\n");
        printf("3. 用户删除\n");
        printf("4. 用户更新\n");    
        printf("5. 退出\n");
        static char choice;
        scanf("%s", &choice);
        switch(choice){
            case '1':
                UserShow();
                break;
            case '2':
                UserAdd();
                break;
            case '3':
                UserDelete();
                break;
            case '4':
                UserUpdate();
                break;
            case '5':
                return;
                break;
            default:
                printf("输入错误，请重新输入\n");
                break;
        }
    }
}

void BookCount(){
    book_p ptr;
    list_p tmp = headbook->list.next;
    int count = 0;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        count+= ptr->num;
        tmp = tmp->next;
    }
    printf("图书库内图书数量：%d\n", count);
    return ;
}

void PopBookCount(){
    BorrowRecord_p ptr;
    list_p tmp = headbook->list.next;
    char bookname[10][20];
    int i = 0,len = 0,max = 0;
    int count[10] = {0};
    while (tmp != &headbook->list){
        len++;
        printf("书名：%20s\t剩余数量：%d\n",
            list_entry(tmp, book_t, list)->bookname,
            list_entry(tmp, book_t, list)->num);
        strcpy(bookname[i++],list_entry(tmp, book_t, list)->bookname);
        tmp = tmp->next;
    }
    printf("-----------------------------\n");
    for ( i = 0; i < len; i++){
        count[i] = 0;
        tmp = headborrow->list.next;
        while (tmp != &headborrow->list){
            ptr = list_entry(tmp, BorrowRecord_t, list);
            if(!strcmp(ptr->bookname,bookname[i])){
                count[i]++;
                if (count[i] > max)
                    max = count[i];              
            }
            tmp = tmp->next;
        }
        printf("书名：%20s\t借阅次数：%d\n",bookname[i],count[i]);
    }   
    printf("-----------------------------\n");  
    for ( i = 0; i < len; i++){
        if (max == count[i]&&max != 0){
            printf("近期热门书籍：%20s\t借阅次数：%d\n",bookname[i],count[i]);
            return;       
        }
    }
    printf("暂无借阅记录，无法统计热门书籍\n");
    return;
}

void UserBorrowCount(){
    BorrowRecord_p ptr;
    list_p tmp = head->list.next;
    char username[10][20];
    int i = 0,len = 0,max = 0;
    int count[10] = {0};
    while (tmp != &head->list){
        len++;
        strcpy(username[i++],list_entry(tmp, user_t, list)->username);
        tmp = tmp->next;
    }
    tmp = headborrow->list.next;
    i = 0;
    while (tmp != &headborrow->list){
        ptr = list_entry(tmp, BorrowRecord_t, list);
        if (username[i]==ptr->username){
            count[i]++;
        }
        i++;
        tmp = tmp->next;
    }
    for ( i = 0; i < len; i++){
        printf("用户名：%20s\t借阅次数：%d\n",username[i],count[i]);
    }
    if (i==0){
        printf("暂无借阅记录，无法统计用户借阅情况\n");
    }
    
    return ;
}

void Statistic(){
    printf("=============信息统计=============\n");
    BookCount();
    PopBookCount();
    UserBorrowCount();
}

void AdminUI(){
    while (1)
    {
        printf("============管理员登录============\n");
        if(AdminLogin()){
            break;
        }
    }
    while (1)
    {
        printf("============管理员操作============\n");
        printf("请选择：\n");
        printf("1. 图书信息显示\n");
        printf("2. 图书信息删除\n");
        printf("3. 图书信息录入\n");
        printf("4. 图书信息更新\n");
        printf("5. 信息统计\n");
        printf("6. 用户管理\n");
        printf("7. 退出\n");
        static char choice;
        scanf("%s", &choice);
        switch(choice){
            case '1':
                BookShow();
                break;
            case '2':
                BookDelete();
                break;
            case '3':
                BookAdd();
                break;
            case '4':
                BookUpdate();
                break;
            case '5':
                Statistic();
                break;
            case '6':
                UserManage();
                break;
            case '7':
                return;
                break;
            default:    
                printf("输入错误，请重新输入\n");
                break;
        }
    }
}