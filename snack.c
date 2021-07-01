#include <stdio.h> //�зǿ�X��J 
#include <conio.h> //������V��ݭn�Ψ� 
#include <stdlib.h> // malloc �ݭn�Ψ� 
#include <windows.h> // sleep �ݭn�Ψ� 
#include <time.h> // �H���ؤl�ݭn�Ψ� 
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SNAKE 1     //�D���y�м���
#define FOOD 2      //�������y�м���
#define BAR 3       //�𪺮y�м���
#define MAPMAXX 6 
#define MAPMAXY 6
#define BARNUM 2*MAPMAXX+2*MAPMAXY-4
char map[MAPMAXX][MAPMAXY] = {0}; //��l�Ʀa��
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
void initSnack(mySnack *snack); // ��l�ƳD
void printMap(void); //�C�L�a�� 
void changeDirection(mySnack *snack); //���ܳD�Y��V�]�`�N��D�����׶W�L�@�`�ɤ���^�Y�^ 
void updateMap(mySnack *snack, myFood *food); //��s�a��(�D����, �Y�쭹��, �D�W���P�_)  
void generateFood(const mySnack *snack, myFood *food); //�Ͳ��������禡 
int isAlive(const mySnack *snack); //�P�_�D�O�_�s��(�P�_�D�O�_������ɩΪ̦Y��ۤv������)
void shuffle(int *array, size_t n); //���ð}�C 
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
    //�ھڦa�ϤW�C�I�����pø�s�C���] i ��� x �b�Aj ��� y �b�^�A����C�L
    int i, j;
    for(i = 0;i < MAPMAXX; i++){
        for(j = 0;j < MAPMAXY; j++){
            if(map[i][j] == 0)//�ťզa��
                putchar(' ');
            else if(map[i][j] == SNAKE)//�D��
                putchar('*');
            else if(map[i][j] == BAR)//����
                putchar('#');
            else if(map[i][j] == FOOD)//����
                putchar('$');
        }
        putchar('\n');
    }
    Sleep(500);     //��v�禡 �N�{�Ǳ��_500ms,�]�t�bwindow.h�]�blinux�U�� sleep(),#include <unistd.h>�^ 
    system("cls");  //�M�̨禡 �t�X�U�@�� print_game() �_�쭫�s��z�@�ΡA�]�t�bstdlib.h��
}

void changeDirection(mySnack *snack){
     int new_dir = snack->direction;//  = old_dir;
     if(_kbhit()){ //kbhit() �ˬd��e�O�_����L��J�A�Y���h��^�@�ӫD0�ȡA�_�h��^0 
         if(getch() == 224){ //�Ĥ@��getch()����224����U��V��, �ĤG���~�O���� 
             new_dir = getch();
             //�p�G�D�����פj��1�A�h����180�צ^�Y���A�p�G���U���O�^�Y��V�A�h����Ӥ�V��
             //abs(�s��V��h�¦���V)�� 2 ��� |LEFT-RIGHT|, ���^�Y 
             //abs(�s��V��h�¦���V)�� 8 ��� |UP-DOWN|, ���^�Y 
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
    switch (snack->direction){ //��s�D�Y�y��
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
        if(i == 0){ // �D�Y�w�g��s�L, ������s
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
    if (snack->entity->x == food->x && snack->entity->y == food->y){ //�ˬd���L�Y�쭹�� 
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
        if(snack->len + BARNUM >= MAPMAXX*MAPMAXY ){ //�˴��D�����L�W�L�e�\�d�� 
            printf("You Complete the Game!!\n");
            printf("Your Score is %d \n", snack->len - 1);
            system("pause");
            exit(0);
        }
        generateFood(snack, food); //���ͷs����
    }
    for (i = 0; i < MAPMAXX; i++){   //�N��ɻP�����[��a�ϸ̥h
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
    int eat_self = 0; // �Y�ۤv�X��(�Y�I�쨭��) 
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
        snack->entity->y != 0 && snack->entity->y != MAPMAXY -1 && !eat_self); //�Y�ۤv�άO�I����� 
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
