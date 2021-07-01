#include <stdio.h> //標準輸出輸入 
#include <conio.h> //偵測方向鍵需要用到 
#include <stdlib.h> // malloc 需要用到 
#include <windows.h> // sleep 需要用到 
#include <time.h> // 隨機種子需要用到 
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SNAKE 1     //蛇的座標標識
#define FOOD 2      //食物的座標標識
#define BAR 3       //牆的座標標識
#define MAPMAXX 6 
#define MAPMAXY 6
#define BARNUM 2*MAPMAXX+2*MAPMAXY-4
char map[MAPMAXX][MAPMAXY] = {0}; //初始化地圖
typedef struct foodStruct {
    int x;
    int y;
    int bucketSize;
    int *bucket;
} myFood;
typedef struct snackEntity {
    int x;
    int y;
    struct snackEntity *next;
} mySnackEntity;
typedef struct snackStruct {
    int len;
    int direction;
    mySnackEntity *entity;
} mySnack;
void initSnack(mySnack *snack); // 初始化蛇
void printMap(void); //列印地圖 
void changeDirection(mySnack *snack); //改變蛇頭方向（注意當蛇身長度超過一節時不能回頭） 
void updateMap(mySnack *snack, myFood *food); //更新地圖(蛇移動, 吃到食物, 蛇增長與否)  
void generateFood(const mySnack *snack, myFood *food); //生產食物的函式 
int isAlive(const mySnack *snack); //判斷蛇是否存活(判斷蛇是否撞到邊界或者吃到自己的身體)
void shuffle(int *array, size_t n); //打亂陣列 
int main(void){
    if(MAPMAXY<2 || MAPMAXX <=2){
        printf("MAP is too small, each axis length should be larger than 2\n");
        system("PAUSE");
        return 0;
    }
    mySnack snack;
    myFood food = {-1, -1, 0, NULL};
    initSnack(&snack);
    generateFood(&snack, &food);
    while(1){
        printMap();
        changeDirection(&snack);
        updateMap(&snack, &food);
        int x = 0;
        if(x = !isAlive(&snack))
            break;
    }
    printf("Game Over!\n");
    printf("Your Score is %d \n", snack.len - 1);
    system("PAUSE");
    return 0;
}

void initSnack(mySnack *snack){
    snack->len = 1;
    snack->direction = UP;
    mySnackEntity *entity = (mySnackEntity *)malloc(sizeof(mySnackEntity));
    entity->x = MAPMAXX/2; 
    entity->y = MAPMAXY/2;
    entity->next = NULL;
    snack->entity = entity;
}

void printMap(void){
    //根據地圖上每點的情況繪製遊戲（ i 表示 x 軸，j 表示 y 軸），按行列印
    int i, j;
    for(i = 0;i < MAPMAXX; i++){
        for(j = 0;j < MAPMAXY; j++){
            if(map[i][j] == 0)//空白地方
                putchar(' ');
            else if(map[i][j] == SNAKE)//蛇身
                putchar('*');
            else if(map[i][j] == BAR)//圍欄
                putchar('#');
            else if(map[i][j] == FOOD)//食物
                putchar('$');
        }
        putchar('\n');
    }
    Sleep(500);     //休眠函式 將程序掛起500ms,包含在window.h（在linux下用 sleep(),#include <unistd.h>） 
    system("cls");  //清屏函式 配合下一次 print_game() 起到重新整理作用，包含在stdlib.h中
}

void changeDirection(mySnack *snack){
     int new_dir = snack->direction;//  = old_dir;
     if(_kbhit()){ //kbhit() 檢查當前是否有鍵盤輸入，若有則返回一個非0值，否則返回0 
         if(getch() == 224){ //第一次getch()產生224表按下方向鍵, 第二次才是其方位 
             new_dir = getch();
             //如果蛇身長度大於1，則不能180度回頭走，如果按下的是回頭方向，則按原來方向走
             //abs(新方向減去舊有方向)為 2 表示 |LEFT-RIGHT|, 為回頭 
             //abs(新方向減去舊有方向)為 8 表示 |UP-DOWN|, 為回頭 
             if(snack->len > 1 && (abs(new_dir - snack->direction) == 2 || abs(new_dir - snack->direction) == 8))
                    new_dir = snack->direction;
             snack->direction = new_dir; 
         }
     }
} 

void updateMap(mySnack *snack, myFood *food){
    int i, j;
    mySnackEntity *ptr;
    int last_x = snack->entity->x;
    int last_y = snack->entity->y;
    int temp_x;
    int temp_y;
    switch (snack->direction){ //更新蛇頭座標
        case UP:
            (snack->entity->x)--;
            break;
        case DOWN:
            (snack->entity->x)++;
            break;
        case LEFT:
            (snack->entity->y)--;
            break;
        case RIGHT:
            (snack->entity->y)++;
            break;
    }
    ptr = snack->entity;
    for(i = 0; i < snack->len; i++){ //snack move
        if(i == 0){ // 蛇頭已經更新過, 不必更新
            ptr = ptr->next;
            continue;
        }
        temp_x = ptr->x;
        temp_y = ptr->y;
        ptr->x = last_x;
        ptr->y = last_y;
        last_x = temp_x;
        last_y = temp_y;
        ptr = ptr->next;
    }
    if (snack->entity->x == food->x && snack->entity->y == food->y){ //檢查有無吃到食物 
        mySnackEntity *ptr = snack->entity;
        mySnackEntity *entity = (mySnackEntity *)malloc(sizeof(mySnackEntity));
        entity->x = last_x; 
        entity->y = last_y;
        entity->next = NULL;
        while(ptr->next != NULL){
            ptr = ptr->next;      
        }
        ptr->next = entity;
        snack->len++;
        if(snack->len + BARNUM >= MAPMAXX*MAPMAXY ){ //檢測蛇長有無超過容許範圍 
            printf("You Complete the Game!!\n");
            printf("Your Score is %d \n", snack->len - 1);
            system("pause");
            exit(0);
        }
        generateFood(snack, food); //產生新食物
    }
    for (i = 0; i < MAPMAXX; i++){   //將邊界與食物加到地圖裡去
        for (j = 0; j < MAPMAXY; j++){    
            if (i == 0 || i == MAPMAXX - 1 || j == 0 || j == MAPMAXY - 1)
                map[i][j] = BAR;
            else
                map[i][j] = 0;       
        }
    }
    ptr = snack->entity; 
    while(ptr != NULL){
        map[ptr->x][ptr->y] = SNAKE;        
        ptr = ptr->next;
    }
    map[food->x][food->y] = FOOD;
}

void generateFood(const mySnack *snack, myFood *food){
     int i;
     int in_snack = 0;
     int food_x, food_y;
     if(food->bucket == NULL){
         food->bucketSize = MAPMAXX * MAPMAXY;
         food->bucket = (int *)malloc(food->bucketSize * sizeof(int));
         for(i = 0; i < food->bucketSize; i++)
             food->bucket[i] = i;
     }
     mySnackEntity *ptr;
     shuffle(food->bucket, food->bucketSize);
     for(i = 0; i < food->bucketSize; i++){
         food_x = food->bucket[i] / (MAPMAXY);
         food_y = food->bucket[i] % (MAPMAXY);
         ptr = snack->entity;
         in_snack = 0;
         while(ptr != NULL){
             if(ptr->x == food_x && ptr->y == food_y){
                 in_snack = 1;
                 break;
             }
             ptr = ptr->next;
         }
         if(food_x != 0 && food_x != MAPMAXX - 1 && food_y != 0 && food_y != MAPMAXY - 1 &&
             in_snack == 0){
             food->x = food_x; 
             food->y = food_y;
             break;
         }
     }
}

int isAlive(const mySnack *snack){
    int eat_self = 0; // 吃自己旗標(頭碰到身體) 
    int i;
    mySnackEntity *ptr = snack->entity->next;
    while(ptr != NULL){
        if(snack->entity->x == ptr->x && snack->entity->y == ptr->y){
            eat_self = 1;
            break;
        }
        ptr = ptr->next;
    }
    return (snack->entity->x != 0 && snack->entity->x != MAPMAXX - 1 && 
        snack->entity->y != 0 && snack->entity->y != MAPMAXY -1 && !eat_self); //吃自己或是碰到邊界 
}

void shuffle(int *array, size_t n){
    srand((unsigned int)time(NULL));
    if (n > 1){
        size_t i;
        for (i = 0; i < n; i++){
            size_t j = rand() / ( RAND_MAX / n);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
