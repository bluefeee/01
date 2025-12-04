#include "user.h"

user_p head = NULL;
book_p headbook = NULL;
BorrowRecord_p headborrow = NULL;
char USER[20];

user_p UserInit(){
    head = (user_p)malloc(sizeof(user_t));
    head->list.next = &head->list;
    head->list.prev = &head->list;
    return head;
}

void __initBook(book_p book, const char* id,const char* bookname, const char* author, const char* publisher, char* publishyear, int num) {
    strcpy(book->id, id);
    strcpy(book->bookname, bookname);
    strcpy(book->author, author);
    strcpy(book->publisher, publisher);
    strcpy(book->publishyear, publishyear);
    book->num = num;
    book->list.next = &(book->list);
    book->list.prev = &(book->list);
}

book_p BookInit(){
    headbook = (book_p)malloc(sizeof(book_t));
    headbook->list.next = &headbook->list;
    headbook->list.prev = &headbook->list;

    ReadFromFile("book.txt", headbook,book_t,6,"%s %s %s %s %s %d",
        tmp.id,tmp.bookname,tmp.author,tmp.publisher,tmp.publishyear,&tmp.num);
        
    // Write2File("book1.txt", headbook,book_t,"%s %s %s %s %s %d\n",
    //     node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
    return headbook;
}

void __initBorrowRecord(BorrowRecord_p borrowrecord, const char* id, const char* username, const char* bookname, const char* status, const char* borrowtime, const char* returntime, const char* remindertime) {
    strcpy(borrowrecord->id, id);
    strcpy(borrowrecord->username, username);
    strcpy(borrowrecord->bookname, bookname);
    strcpy(borrowrecord->status, status);
    strcpy(borrowrecord->borrowtime, borrowtime);
    strcpy(borrowrecord->returntime, returntime);
    strcpy(borrowrecord->remindertime, remindertime);
    borrowrecord->list.next = &(borrowrecord->list);
    borrowrecord->list.prev = &(borrowrecord->list);

}


BorrowRecord_p BorrowRecordInit(){
    headborrow = (BorrowRecord_p)malloc(sizeof(BorrowRecord_t));
    headborrow->list.next = &headborrow->list;
    headborrow->list.prev = &headborrow->list;

    ReadFromBorFile("borrowrecord.txt", headborrow,BorrowRecord_t,7, "%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^|]|%29[^|]|%29[^\n]",
        tmp.id,tmp.username,tmp.bookname,tmp.status,tmp.borrowtime,tmp.returntime,tmp.remindertime);

    return headborrow;
}

int compareCurrentAndReturnTime(const char *returntime) {
    struct tm return_tm;
    char weekday[4], month[4];
    time_t current_time, return_time;

    // 1. 解析 returntime 字符串（格式：%a %b %d %H:%M:%S %Y）
    sscanf(returntime, "%3s %3s %d %d:%d:%d %d",
           weekday, month,
           &return_tm.tm_mday, &return_tm.tm_hour, &return_tm.tm_min, &return_tm.tm_sec,
           &return_tm.tm_year);

    // 2. 修正 tm 结构体（同之前的逻辑）
    return_tm.tm_year -= 1900; // 年份偏移修正
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strcmp(month, months[i]) == 0) {
            return_tm.tm_mon = i;
            break;
        }
    }
    return_tm.tm_isdst = -1; // 自动处理夏令时

    // 3. 将 returntime 转为时间戳
    return_time = mktime(&return_tm);
    if (return_time == -1) { // 解析失败（格式错误）
        printf("returntime 格式错误，无法比较！\n");
        return -2; // 异常标记
    }

    // 4. 获取当前时间戳
    current_time = time(NULL);

    // 5. 数值比较（时间戳越大，时间越晚）
    if (current_time > return_time) {
        return 1; // 当前时间晚于还书时间（已超时）
    } else if (current_time < return_time) {
        return -1; // 当前时间早于还书时间（未超时）
    } else {
        return 0; // 时间相等
    }
}

BorrowRecord_t* BorrowRecordAdd(char*id,char *bookname,char *username){
    BorrowRecord_p rec = (BorrowRecord_p)malloc(sizeof(BorrowRecord_t));
    strcpy(rec->id,id);
    strcpy(rec->bookname,bookname);
    strcpy(rec->username,username);
    strcpy(rec->status,"借阅中");
    time_t now_time;
    time(&now_time);    
    strncpy(rec->borrowtime, ctime(&now_time), 24);
    rec->borrowtime[24] = '\0';
    // strcpy(rec->borrowtime,ctime(&now_time));//Fri Nov 21 14:29:28 2025
    strcpy(rec->returntime,"暂无归还");
    ListAdd(&rec->list,&headborrow->list);
    
    

    struct tm tm_info;
    char weekday[4], month[4];
    sscanf(rec->borrowtime, "%3s %3s %d %d:%d:%d %d",
       weekday,    // 星期（如 Wed）
       month,      // 月份（如 Nov）
       &tm_info.tm_mday, // 日期（20）
       &tm_info.tm_hour, // 小时（14）
       &tm_info.tm_min,  // 分钟（30）
       &tm_info.tm_sec,  // 秒（00）
       &tm_info.tm_year); // 年份（2024）
    tm_info.tm_year -= 1900; 
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strcmp(month, months[i]) == 0) {
            tm_info.tm_mon = i;
            break;
        }
    }
    tm_info.tm_isdst = -1; 
    tm_info.tm_sec += 1;
    mktime(&tm_info);
    setlocale(LC_TIME, "C");
    strftime(rec->remindertime, sizeof(rec->remindertime), 
                        "%a %b %d %H:%M:%S %Y", &tm_info);
    printf("定时归还时间：%s\n",rec->remindertime);

    Write2File("borrowrecord.txt", headborrow,BorrowRecord_t, "%s|%s|%s|%s|%s|%s|%s\n",
        node->id,node->username,node->bookname,node->status,node->borrowtime,node->returntime,node->remindertime);

    return rec;
}

int BorrowRecordReturn(char *id,char *username){
    BorrowRecord_p ptr;
    list_p tmp = headborrow->list.next;
    while (tmp != &headborrow->list){
        ptr = list_entry(tmp, BorrowRecord_t, list);
        if( !strcmp(ptr->id,id) && 
            !strcmp(ptr->username,username)&& 
            !strcmp(ptr->status,"借阅中")){
            time_t now_time;
            time(&now_time);    
            strncpy(ptr->returntime, ctime(&now_time), 24);
            ptr->returntime[24] = '\0';
            // strcpy(ptr->returntime,ctime(&now_time));
            strcpy(ptr->status,"已归还");
            Write2File("borrowrecord.txt", headborrow,BorrowRecord_t, "%s|%s|%s|%s|%s|%s|%s\n",
                        node->id,node->username,node->bookname,node->status,node->borrowtime,node->returntime,node->remindertime);
            return 1;
        }
        tmp = tmp->next;
    }
    printf("归还信息错误\n");
    return 0;
}

user_p UserCreat(char *username,char *password){
    user_p node = (user_p)malloc(sizeof(user_t));
    strcpy(node->username,username);
    strcpy(node->password,password);
    printf("username:%s\npassword:%s\n",node->username,node->password);
    node->list.next = &node->list;
    node->list.prev = &node->list;   
    return node;
}

void  ListAdd(list_p new,list_p pre){
    new->next = pre->next;
    pre->next->prev = new;
    pre->next = new;
    new->prev = pre;
}

void registerUser( ){
    printf("==============用户注册==============\n");
    printf("请输入用户名：");
    char username[20];
    scanf("%s",username);
    printf("请输入密码：");
    char password[20];
    scanf("%s",password);
    user_p ptr;
    list_p tmp = head->list.next;
    do{
        ptr = list_entry(tmp, user_t, list);
        if(!strcmp(ptr->username,username)){
            printf("用户名已存在，请重新输入\n");
            return;
        }
        tmp = tmp->next;
    }while (tmp != &head->list);
    user_p node = UserCreat(username,password);
    ListAdd(&node->list,&head->list);
}

int loginUser( ){
    printf("==============用户登录==============\n");
    printf("请输入用户名：");
    char username[20];
    scanf("%s",username);
    printf("请输入密码：");
    char password[20];
    scanf("%s",password);
    user_p ptr;
    list_p tmp = head->list.next;
    do{
        ptr = list_entry(tmp, user_t, list);
        if(!strcmp(ptr->username,username)
            && !strcmp(ptr->password,password)){
            printf("登录成功\n");
            strcpy(USER ,username);
            return 1;
        }
        else{
            // printf("usr:%s\npwd:%s\n",ptr->username,ptr->password);
            tmp = tmp->next;
        }   
    }while (tmp != &head->list);
    printf("请检查用户名/密码是否正确\n");
    return 0;
}

void UserOperate( ){
    while (1)
    {
        printf("============用户操作界面============\n");
        printf("请选择：\n");
        printf("1.图书查询\n");
        printf("2.图书借阅\n");
        printf("3.图书归还\n");
        printf("4.借阅查询\n");
        printf("5.退出\n");
        static char choice;
        scanf("%s",&choice);
        switch (choice){
            case '1':
                FindBook();
                break;
            case '2':
                BorrowBook();
                break;
            case '3':
                ReturnBook();
                break;
            case '4':
                BorrowRecordFind();
                break;
            case '5':
                return;
            default:
                printf("输入错误，请重新输入\n");
                break;
        }
    }
}

void __findbook(char *bookinfor){
    book_p ptr;
    list_p tmp = headbook->list.next;
    char flag = 0,*p;        
    int month;
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        p = &ptr->publishyear[5];
        month = atoi(&bookinfor[5]);// atoi(p);
        if( (!strcmp(ptr->bookname,bookinfor)) || (!strcmp(ptr->author,bookinfor)) || (!strcmp(ptr->id,bookinfor)) 
            || ((!strncmp(ptr->publishyear,bookinfor,4))&&(((month-4)<=atoi(p))&&((month+4)>=atoi(p))))
            || (strstr(ptr->bookname, bookinfor) != NULL)|| (strstr(ptr->author, bookinfor) != NULL)){
            printf("书籍信息：\n");
            printf("编号：\t\t%s\n书名：\t\t%s\n作者：\t\t%s\n出版社：\t%s\n出版年份：\t%s\n库存：\t\t%d\n",
                ptr->id,ptr->bookname,ptr->author,ptr->publisher,ptr->publishyear,ptr->num);
            printf("--------------------------\n");
            flag = 1;
        }
        tmp = tmp->next;
    }
    if(!flag)   printf("没有找到该书籍,请检查查询信息是否输入正确\n");
}

void FindBook(){
    printf("==============图书查询==============\n");
    while (1){
        printf("请输入要查询的图书，支持书名，作者，出版年月模糊查询\n(格式：1书名/2作者/3出版年份/4编号/输入0则退出)：");
        static char choice[10];
        scanf("%s",choice);
        if(choice[0] == '0'){
            return;
        }
        else if(choice[0] > '0' && choice[0] <= '4'){
            __findbook(choice+1);
        }
        else{
            printf("输入错误，请重新输入\n");
        }
    }
}


void BorrowBook( ){
    printf("==============图书借阅==============\n");  
    book_p ptr;
    list_p tmp = headborrow->list.next;
    while (tmp != &headborrow->list){
        if(compareCurrentAndReturnTime(list_entry(headborrow->list.next, BorrowRecord_t, list)->remindertime)==1
            && !strcmp(list_entry(headborrow->list.next, BorrowRecord_t, list)->status,"借阅中")){
            printf("您当前还有逾期书籍未归还，请先归还后再借阅\n");
            return;
        }
        tmp = tmp->next;
    }
    tmp = headbook->list.next;
    printf("请输入要借阅的图书编号：\n");
    char id[20];
    scanf("%s",id);
    while (tmp != &headbook->list){
        ptr = list_entry(tmp, book_t, list);
        if( !strcmp(ptr->id,id)){
            if(ptr->num > 0){
                ptr->num--;
                printf("借阅成功\n");
                BorrowRecordAdd(ptr->id,ptr->bookname,USER);
                Write2File("book.txt", headbook,book_t,"%s %s %s %s %s %d\n",
                            node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
            }
            else{
                printf("该书籍库存不足\n");
            }       
            printf("该书籍剩余%d本\n",ptr->num);
            return;
        }
        tmp = tmp->next;
    }
    printf("没有找到该书籍,请检查信息是否输入正确\n");
}


void ReturnBook( ){
    printf("==============图书归还==============\n");
    book_p ptr;
    BorrowRecord_p ptrBrow;
    list_p tmpbook = headbook->list.next;
    list_p tmp = headborrow->list.next;

    printf("请输入要归还的图书编号：");
    char id[20];
    scanf("%s",id);
    while (tmp != &headborrow->list){
        ptrBrow = list_entry(tmp, BorrowRecord_t, list);
        if( !strcmp(ptrBrow->id,id)&&!strcmp(ptrBrow->status,"借阅中")){
            if(BorrowRecordReturn(id,USER)){
                printf("归还成功\n");
                while(tmpbook != &headbook->list){
                    ptr = list_entry(tmpbook, book_t, list);
                    if( !strcmp(ptr->id,id)){
                        ptr->num++;
                    }
                    tmpbook = tmpbook->next;
                }
                Write2File("book.txt", headbook,book_t,"%s %s %s %s %s %d\n",
                            node->id,node->bookname,node->author,node->publisher,node->publishyear,node->num);
            }
            printf("该书籍剩余%d本\n",ptr->num);
            return;
        }
        tmp = tmp->next;
    }
    printf("没有找到该书籍,请检查信息是否输入正确\n");
}

void BorrowRecordFind(){
    printf("==============借阅查询==============\n");
    BorrowRecord_p ptr;
    list_p tmp = headborrow->list.next;
    do{
        ptr = list_entry(tmp, BorrowRecord_t, list);
        if( !strcmp(ptr->username,USER)){
            printf("借阅信息：\n");
            printf("书名：\t\t%s\n用户名：\t%s\n归还状态：\t%s\n借阅时间：%s\n归还时间：%s\n",
                ptr->bookname,ptr->username,ptr->status,ptr->borrowtime,ptr->returntime);
        }
        tmp = tmp->next;
    }while (tmp != &headborrow->list);

}

void UserUI(){
      
    
    printf("初始化一个测试账号\n");
    user_p u1 = UserCreat("1","1");
    ListAdd(&u1->list,&head->list);
    
    while (1)
    {
        printf("==============用户界面==============\n");
        printf("请选择：\n");
        printf("1.注册\n");
        printf("2.登录\n");
        printf("3.退出\n");
        static char choice;
        scanf("%s",&choice);
        switch (choice){
            case '1':
                registerUser( );
                break;
            case '2':
                if(loginUser( )){
                    UserOperate( );
                }
                break;
            case '3':
                return;
            default:
                printf("输入错误，请重新输入\n");
                break;
        }
    }
}



