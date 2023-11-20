#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

//icon
#define SNAKE 1
#define FOOD 2
#define WALL 3


#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

char map[17][17] = {0}; //map[0][0]=0
unsigned char snake[50] = {77}; //(4,13)=(0100,1101),snake[0]=77;
unsigned char food = 68;  //food的初始坐標(4,4)
char len = 1;

unsigned char generate_food();

//印出畫面
void print_game(){
    for(int i=0; i<17;i++){
        for(int j=0;j<17;j++){
            if(map[i][j] == 0){
                putchar(' ');
            }
            else if(map[i][j] == SNAKE){
                putchar('o');
            }
            else if(map[i][j] == FOOD){
                putchar('@');
            }
            else if(map[i][j] == WALL){
                putchar('#');
            }
        }
        putchar('\n');
    }
    Sleep(500);
    system("cls");
}

//取得鍵盤方向
int get_dir(int old_dir){

    int new_dir = old_dir;
    if(_kbhit()){
        getch();
        new_dir = getch();
        //printf("%d\n",new_dir);
    }
    //解決snake往右邊時不能往左邊,往上時不能直接往下
    if(len > 1){
        //if(old_dir == UP && new_dir == DOWN)
            // return old_dir;
        //if(old_dir == DOWN && new_dir == UP)
            // return old_dir;
        //if(old_dir == RIGHT && new_dir == LEFT)
            //return old_dir;
        //if(old_dir == LEFT && new_dir == RIGHT)
            //return old_dir;

        if(abs(new_dir-old_dir) == 8 || abs(new_dir-old_dir) == 2 )
            return old_dir;

    }
    return new_dir;

}

//控制蛇的移動
void move_snake(int dir){

    //snake變長
    int last = snake[0],current;
    bool grow = false;    //有無吃到food

    //food
    unsigned char fx,fy,x,y;
    fx = food >> 4;      //0100
    fy = food  & 0x0F;  //不要0000 只要0100,0x是16進制,F是1111

    //解析蛇index=77的位子成x跟y
    x = snake[0] >> 4;
    y = snake[0] & 0x0F;

    switch(dir){
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;

    }
    //更新蛇的位置,把x,y在整合回來成(x,y)
    snake[0] = (x << 4) | y;

    //判斷grow
    if(snake[0] == food){     //如果蛇的坐標 == food的坐標
        grow = true;
        //吃到food,@要隨機變換位置
        food = generate_food();
    }
    for(int i = 0; i <len; i++){   //如果grow是true,就是有移動
        if(i == 0)
            continue;
        current = snake[i]; //current記錄當前位置
        snake[i] = last;    //i往前移動到last
        last = current;   //最後last要指向current
    }
    if(grow == true){
        snake[len] = last;   //蛇尾len,就要增加last在尾巴
        len++;
    }

    //wall
    for(int i =0;i<17;i++){
        for(int j=0; j<17;j++){
           if(i == 0 || i == 16|| j == 0 || j == 16){
            map[i][j] = WALL;  //i is y 垂直 ,j is x 水平
           }
           //food的位置
           else if(i == fy && j == fx ){
                map[i][j] = FOOD;
           }
           else{
            map[i][j] = 0;
           }
        }
    }
    //snake的長度
    for(int i = 0; i < len ; i++){

        //更新snake吃到food,增加身長的坐標
        x = snake[i] >> 4;
        y = snake[i] & 0x0F;

        if(snake[i] > 0){
            map[y][x] = SNAKE;
        }
    }

}

//改變food的位子
unsigned char generate_food(){

    unsigned char food_,fx,fy;
    bool is_snake = false;     //food剛好產生在snake的身上

    srand((unsigned int)time(NULL));
    do{
        is_snake = false;
        food_ =(unsigned char) ( rand() % 256); //隨機產生亂數0~255
        //解析x,y
        fx = food_ >> 4;
        fy = food_ & 0x0F;

        for(int i= 0; i < len ; i++){
            if(food_ == snake[i]){
                is_snake = true;    //ture就是food有落在snake上,就要在從新一遍
            }
        }
    }
    while( fx == 0 || fx == 16 || fy == 0 || fy == 16 || is_snake == true);

    return  food_;
}

//判斷snake是否有存活
bool is_alive(){
    bool self_eat = false;

    //邊界
    unsigned char x,y;
    x = snake[0] >> 4;
    y = snake[0] & 0x0F;

    //snake撞到自己
    for(int i = 1; i < len ; i++ ){
        if(snake[0] == snake[i])
            self_eat = true;    //true就是死掉
    }
    //snake撞到邊界
    return( x == 0 || x == 16| y== 0 || y==16 || self_eat) ? false : true;   //fals就是死了

}


int main(){

    //初始化 map
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            map[i][j]=0;
        }
    }


    //初始化 snake
    for(int i=1;i<50;i++){
        snake[i] = 0;
    }


    int dir = UP; //初始值,snake不會停要一直往前
    while(1){
        print_game();
        dir = get_dir(dir);
        move_snake(dir);
        if(!is_alive())
            break;

    }
    printf("Game Over");

    return 0;

}

//往上下走,移動y坐標,往左右走是動x坐標


/*
//只要賦值一次就可以
srand((unsigned int)time(NULL));
for(int i = 0; i<10 ; i++){
    int a = rand() % 10; //取餘數10,等於0~9
    printf("%d\n",a);
}
*/
