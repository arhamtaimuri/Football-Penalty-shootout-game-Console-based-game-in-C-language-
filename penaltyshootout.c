#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>

#define MAX_NAME 30
#define MAX_PLAYER 50
#define MAX_HISTORY 100
#define KICKS_PER_TEAM 5


typedef enum direction{
  LEFT,
  CENTER, 
  RIGHT,

}direction;

typedef enum difficulty{
    EASY,
    MEDIUM,
    HARD
}difficulty_lvl;

typedef enum shotresult{
     GOAL,  //0
     SAVED,   //1
     MISSED    //2

}shotresults;
//ALL Structs

typedef struct Player{
    char name[MAX_NAME];
    int matches_played;
     int wins;
    int losses;
    int totalgoals;
    int totalsaves;
    float best_accuracy;
   

}Player;

typedef struct MatchStats{
   int goals;
   int saves;
   int misses;
   int shotstaken;
   float accuracy;

}MatchStats; 

typedef struct MatchRecord{
     char playername[MAX_NAME];
     int difficulty;
     int mode;   // mode means you are shooting penalty or saving goal
     int yourScore;
     int computerScore;
     int result;

}MatchRecord; 

typedef struct ShotTracker{
    int count[3];
}ShotTracker;

//All functions:  

void clearscreen(){
    #ifdef _WIN32
    system("cls");
    #else 
    system("clear");
    #endif
}

void clearInputBuffer(){     // this function is used for clearing like when the new user wants to type then scanf or fgets starts with empty without holding previous values(values like writing abc in %d format specifier)
    int i;
    while ((i = getchar()) != '\n' && i != EOF) {      //EOF is end of file , if not checked loop will run forever
    }
}


void printbanner(){       //remaining
    printf("=========PENALTY SHOOTOUT GAME=============");

}

void display_main_menu(){       //this is the menu , i have called it in main

    clearscreen();
    printbanner();
    printf("\n1.Play Shootout");
    printf("\n2.Goal Keeper mode");
    printf("\n3.Practice Mode");
    printf("\n4.Leaderboard");
    printf("\n5.Match History");
    printf("\n6.Exit");
}

int getValidINT(int min, int max){
    int input;
    int result;

    result = scanf("%d", &input);
    clearInputBuffer();
    while (result != 1 || input < min || input > max) {
        printf("\nINVALID INPUT. ENTER A NUMBER BETWEEN %d and %d ", min, max);
        result = scanf("%d", &input);
        clearInputBuffer();
    }
    return input;
}

int getMenuChoice(){        //it will ask the user to input form the menu
    printf("\nEnter choice(1-6):  ");
    int result=getValidINT(1,6);

    return result;
}



void pausescreen(){
    printf("\nPress ENTER to continue ");
    clearInputBuffer();
}



void getplayername(char playername[]){       //it will get player name 
    
    int valid=0;
    int length;

    printf("\nEnter your name:   ");
    while(!valid){
      fgets(playername,MAX_NAME,stdin);
      length=strlen(playername);
      
            if(length>0 && playername[length-1]=='\n'){
                playername[length-1]='\0';
            }
            else{
              clearInputBuffer();
            }

            for(int i=0;playername[i]!='\0';i++){
               if(playername[i]!=' '){
               valid=1;

               }
            }
               if(!valid){
                printf("INVALID NAME! ");
        
               }
            
    }
   

}



int getPlayerDirection(){
    printf("\nChoose direction:   ");
    printf("\n1.Left");
    printf("\n2.Center");
    printf("\n3.Right");
   int directions=getValidINT(1, 3);
   directions=directions-1;

   return directions;
}

int chooseDifficulty(){
    printf("\nChoose difficulty level:  ");
    printf("\n1.Easy");
    printf("\n2.Medium");
    printf("\n3.Hard(goalkeeper learns your habit)");
   int difficulty_type=getValidINT(1, 3);
    difficulty_type=difficulty_type-1;
    return difficulty_type;

}
//module C:

//module D:

//module E:



//stats.accuracy = calculateAccuracy(&stats); write this after match ends

//module F: 

void initStats( MatchStats *stats){
     stats->accuracy=0;     //-> It is called structure pointer same as (*stats).accuracy
     stats->goals=0; 
     stats->misses=0;
     stats->saves=0;
     stats->shotstaken=0;
}

void shooterFunction(MatchStats *stats, shotresults shotresult){
    stats->shotstaken++;

    if(shotresult==GOAL){
        stats->goals++;
    }
    else if(shotresult==MISSED){
        stats->misses++;
    }

}

void keeperFunction(MatchStats *stats, shotresults shotresult){
  
    if(shotresult==SAVED){
        stats->saves++;

}

}

float calculateAccuracy(MatchStats *stats){
     float accuracy;
    if(stats->shotstaken==0){
        return 0;
    }
    else{
    accuracy=((float)stats->goals/stats->shotstaken)*100;
    }
   return accuracy;
   
}

void displayMATCHSUMMARY(MatchStats stats){
     printf("\n================MATCH-SUMMARY================");
     printf("\nGOALS SCORED:   %d",stats.goals);
     printf("\nShots Taken:   %d",stats.shotstaken);
     printf("\nGOALS Missed:   %d",stats.misses);
     printf("\nYour Accuracy:   %.2f",stats.accuracy);
    
     printf("\nYou can maximize your SCORE! Hit Play again");

}

void updatePlayerProfile(Player *player,MatchStats stats,MatchRecord *won){
              player->matches_played++;
            if(won->result==1){
                player->wins++;
            } 
            else{
                player->losses++;
            }
            player->totalgoals+=stats.goals;
            player->totalsaves+=stats.saves;

         if(stats.accuracy>player->best_accuracy){
         player->best_accuracy=stats.accuracy;
     }

}

//module G:
void saveplayers(Player players[], int playercount){
  FILE *fp;
  fp=fopen("players.txt","w");

  if(fp==NULL){
    printf("Could not save players! ");
    return;
  }
  else{
    for(int i=0;i<playercount;i++){
        fprintf(fp,"%s,%d,%d,%d,%d,%d,%.2f\n",players[i].name,players[i].matches_played,players[i].wins,players[i].losses,players[i].totalgoals,players[i].totalsaves,players[i].best_accuracy);
    }
    fclose(fp);
  }
}


int findPlayer(Player players[], int playercount, char name[]){    // this is a dynamic function , it will compare the name user will write with the name already stored in players.txt file
                                                                   // if it founds the same name as stored in players.txt then it will start with their existing profile, if not then will create a new

    for(int i=0;i<playercount;i++){
        if(strcmp(players[i].name,name)==0){       
            return i;
        }
    }

    return -1;
}


void loadPlayers(Player players[], int *playercount){
     *playercount=0;
    FILE *fp;
    fp=fopen("players.txt","r");

    if(fp==NULL){
       return;
    }

    while(*playercount<MAX_PLAYER &&  fscanf(fp,"%29[^,],%d,%d,%d,%d,%d,%f\n",players[*playercount].name,&players[*playercount].matches_played,&players[*playercount].wins,&players[*playercount].losses,&players[*playercount].totalgoals,&players[*playercount].totalsaves,&players[*playercount].best_accuracy)==7){
         (*playercount)++;

    }
   

    fclose(fp);

}

void appendMatchHistory(MatchRecord record){
    FILE *fp;

    fp=fopen("history.txt","a");
    if(fp==NULL){
        return;
    }
    else{
      fprintf(fp,"%s,%d,%d,%d,%d,%d\n",record.playername,record.difficulty,record.mode,record.yourScore,record.computerScore,record.result);
      }

    fclose(fp);
}

void view_match_history(){
    MatchRecord record;
    FILE *fp;
    fp=fopen("history.txt","r");

    if(fp==NULL){
        printf("NO MATCHES YET! ");
          pausescreen();
        return;
    }
    else{
        char *difficultyNames[] = {"Easy", "Medium", "Hard"};
       char *resultNames[] = {"Loss", "Win"};
        char *modenames[] = {"shooting", "saving"};

        printf("\n==================== MATCH HISTORY ====================\n");
        printf("%-20s%-10s%-10s%-6s%-6s%-8s\n","Name","Level","Mode","You","CPU","Result");
        printf("-------------------------------------------------------\n");
        
        while(fscanf(fp,"%29[^,],%d,%d,%d,%d,%d\n",record.playername,&record.difficulty,&record.mode,&record.yourScore,&record.computerScore,&record.result)==6){
       printf("%-20s%-10s%-10s%-6d%-6d%-8s\n",record.playername,difficultyNames[record.difficulty],modenames[record.mode-1],record.yourScore,record.computerScore,resultNames[record.result]);                                   //here i use record.mode-1 because array index starts at 0 but mode starts at 1

        }
    }
    fclose(fp);
     pausescreen();
}

void sortLeaderboard(Player players[], int playercount){         //this function uses the bubblesort
                                                          //basically this function is used for only sorting purpose so that person with most wins come up 
    for(int i=0;i<playercount-1;i++){         
        for(int j=0;j<playercount-i-1;j++){

           if(players[j].wins<players[j+1].wins){
                 Player temp;
                temp=players[j+1];
               players[j+1]=players[j];
                players[j]=temp;

           }
        }
    }
}

void displayLeaderboard(Player players[], int playercount){        //previous function was used for sorting now main resposibility left for this funciton is displaying
    Player copy[MAX_PLAYER]; // so that real array of players dont change , it act as a temporary variable
                              // as leaderboard will not reamin the same everytime thats why we use as a copy

     if(playercount==0){
        printf("NO PLAYERS YET! ");
        pausescreen();
        return;
    }
    else{
        for(int i=0;i<playercount;i++){
            copy[i]=players[i];
        }

        sortLeaderboard(copy,playercount);

        printf("\n==================== LEADERBOARD ====================\n");
        printf("%-6s%-20s%-10s%-6s%-8s%-8s%-8s%-10s\n","Rank","Name","Matches","Wins","Losses","Goals","Saves","Accuracy");
        printf("-----------------------------------------------------------------------\n");

        for(int i=0;i<playercount;i++){
            printf("%-6d%-20s%-10d%-6d%-8d%-8d%-8d%-10.2f\n",i+1,copy[i].name,copy[i].matches_played,copy[i].wins,copy[i].losses,copy[i].totalgoals,copy[i].totalsaves,copy[i].best_accuracy);
        }

        pausescreen();
    }
}

int main(){
     srand(time(NULL));
    int playercount=0;
    int choice;
    Player player[MAX_PLAYER];
   char name[MAX_NAME];
   loadPlayers(player, &playercount);   //load all players
   getplayername(name);
    int current_index=findPlayer(player,playercount,name);    //current index is the slot for player name like 0 for player1 , 1 for player2 etc
    if(current_index==-1){   // this means no same player is found now we will create a new player
        if(playercount<MAX_PLAYER){
               strcpy(player[playercount].name, name);
         player[playercount].matches_played=0;
         player[playercount].wins=0;
         player[playercount].losses=0;
         player[playercount].totalgoals=0;
          player[playercount].totalsaves=0;
           player[playercount].best_accuracy=0;
           current_index=playercount;
           playercount++;

        } 
       
    }

    do{
 display_main_menu();
   choice=getMenuChoice();
   //if ocndition for choice==1

   //if condition for choice==2

   //if condition for choice==3


 if(choice==4){
    displayLeaderboard(player,playercount);
 }
 else if(choice==5){
    view_match_history();
 }
pausescreen();

 }
    while(choice!=6);


    //Menu loop ends

  saveplayers(player,playercount);  //immediately save the player as a profile is created


}