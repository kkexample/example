#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include "clui.h"

void LoserGameIntro();//the intro of the game
void StartMenu();
int ValidPass(char password[]);//check the validity of password(contains number,special char, ...)
char * TakePassword();  //take password with showing * instead of characters
void SignUp();
void Login();
void ShowUserScore(char username[]);//show the current user score in main menu
void MainMenu(char username[]);
void ShowAsciiArt(char filename[]);  //display whole data of filename
void ShowMistakes_SW(int mistakes); //show mistakes beautifully in short words
void ShowMistakes_LW(int mistakes); //show mistakes beautifully in long words
void PrintLoser_LW(int mistakes); //print Loser AsciiArt & mistakes in long words (> 8 characters)
void PrintLoser_SW(int mistakes); //in short words (<= 8 characters)
void AddScore(int score,char username[]);//add the word points to the user score
void Game(char username[]);
void SortScoreboard();
void ShowScoreboard(char username[]);
void ChangePassword(char username[]);
void NewWord(char username[]); //add new word in the wordlist
void ClearScreen();
int CharExist(char string[],char c); //check if the char c exist in the string or not
int CountFileWords();//count words in file
int CountFileUsers();//count users in file
void ClearInputBuffer();

typedef struct{
  char username[40];
  char password[40];
  int score;
} user; 

typedef struct{
  char username[40];
  int score;
} userScore;


int main(){
  ClearScreen();
  LoserGameIntro();
  init_clui();
  ClearScreen();
  StartMenu();
}


void LoserGameIntro(){
  char string[20]={"Hello & Wellcome to"};
  printf("\n\t       ");
  for(int i=0;i<20;i++){
    printf("\033[97;1m%c\033[0m",string[i]);
    flush();
    delay(205);
  }
  printf("\n");
  FILE *LoserGameArtPtr = fopen("losergameart.txt","r");
  if(LoserGameArtPtr == NULL)
    printf("error");
  char read_string[128];
  while(fgets(read_string,sizeof(read_string),LoserGameArtPtr)!= NULL){
    printf("\033[92;1m%s\033[0m",read_string);
    delay(150);
  }
  fclose(LoserGameArtPtr);
  printf("\t\t\t\t\t");
  for(int i=0;i<4;i++){
    printf(".");
    flush(); 
    delay(600);
  }
}

void StartMenu(){
  ClearScreen();
  char choice;
  printf("------------------\n    \033[33;1mStart Menu\033[0m\n------------------\n");
  printf("  \033[96;1m1.Log in\033[0m\n");
  printf("  \033[96;1m2.Sign up\033[0m\n");
  printf("  \033[96;1m3.Exit\033[0m\n------------------\n");
  scanf("%c",&choice);
  if(choice=='1'){
    ClearScreen();
    Login();
  }else if(choice=='2'){
    ClearScreen();
    SignUp();
  }else if(choice!='3'){
    StartMenu();
  }
}

int ValidPass(char password[]){
  if(strlen(password)<8){
    printf("\033[91m\nmust have at least 8 characters\033[0m\n");
    return 0;
  }
  int hasBigLetter = 0;
  int hasSmallLetter =0;
  int hasNumber =0;
  int hasSpecialChar =0;
  for(int i=0;i<strlen(password);i++){
    if(password[i]>47 && password[i]<58)
      hasNumber=1;
    if(password[i]>64 && password[i]<91)
      hasBigLetter=1;
    if(password[i]>96 && password[i]<123)
      hasSmallLetter=1;
    if(password[i]>32 && password[i]<48)
      hasSpecialChar=1;
    if(password[i]>57 && password[i]<65)
      hasSpecialChar=1;
    if(password[i]>90 && password[i]<97)
      hasSpecialChar=1;
    if(password[i]>122 && password[i]<127)
      hasSpecialChar=1;
  }
  printf("\n");
  if(hasBigLetter==0)
    printf("\033[91mmust contains big letters\033[0m\n");
  if(hasSmallLetter==0)
    printf("\033[91mmust contains small letters\033[0m\n");
  if(hasNumber==0)
    printf("\033[91mmust contains numbers\033[0m\n");
  if(hasSpecialChar==0)
    printf("\033[91mmust contains special characters (@,#,...)\033[0m\n");
  if((hasSpecialChar*hasSmallLetter*hasBigLetter*hasNumber)==1)
    return 1; 
  else
    return 0;
}

char* TakePassword(){
  char* password=(char*)malloc(40);
  int i=0;
  char ch;
  for(;i<40;){
    ch=getch();
    if(ch=='\n')
      break; 
    else if(ch==8 || ch==127){
      /*Ascii code of backspace is 8
      Ascii code of delete is 127
      in linux backspace key on the keyboard
      gain as delete (Ascii 127)*/
      if(i>0){
        i--;
        printf("\033[1D");
        flush();
        printf(" ");
        printf("\033[1D");
        flush();
      }
    }else if(ch==32){
      continue;
    }else{
      password[i]=ch;
      printf("\033[97;1m*\033[0m");
      i++;
    }
  }
  password[i]='\0';
  return password;
}

void SignUp(){
  char username[40],password[40];
  user temp;
  int username_exist=0;
  char choice;
  printf("------- \033[33;1mSign Up\033[0m -------\n\n");
  printf(" \033[94;1mUsername:\033[0m ");
  scanf("%s",username);
  FILE* usersPtr = fopen("users.txt","a+");
  if(usersPtr==NULL){
    printf("cant open the users file");
  }
  rewind(usersPtr);
  while (fread(&temp,sizeof(user),1,usersPtr)==1){
    if(strcmp(username,temp.username)==0){
      ClearScreen();
      printf("\033[91;1mThis Username has been already used!\033[0m\n");
      printf("---------\n\033[96;1m1.Log in\n2.Sign Up\n3.Exit\033[0m\n---------\n");
      ClearInputBuffer();
      scanf("%s",&choice);
      username_exist=1;
      fclose(usersPtr);
      if(choice=='1'){
        ClearScreen();
        Login();
      }else if(choice=='2'){
        ClearScreen();
          SignUp();
      }else{
        StartMenu();
      }
    }
  }
  if(username_exist==0){
    ClearInputBuffer();
    for(;;){
      printf(" \033[94;1mPassword:\033[0m ");
      strcpy(password,TakePassword());
      if(ValidPass(password)==1)
        break;
    }
    memset(temp.username,'\0',40);
    memset(temp.password,'\0',40);
    strcpy(temp.username,username);
    strcpy(temp.password,password);
    temp.score=0;
    fwrite(&temp,sizeof(user),1,usersPtr);
    fclose(usersPtr);
    printf("\n\033[92;1mYou have been Signed up successfully.\033[0m");
    printf("\n---------\n\033[96;1m1.login\n2.Exit\033[0m\n---------\n");
    scanf("%s",&choice);
    if(choice=='1'){
      ClearScreen();
      Login();
    }else{
      StartMenu();
    }
  }
}

void Login(){
  char username[40],password[40];
  user temp;
  int username_exist=0;
  char choice;
  printf("------- \033[33;1mLog In\033[0m -------\n\n");
  printf("\033[94;1mUsername:\033[0m ");
  scanf("%s",username);
  FILE* usersPtr = fopen("users.txt","r");
  if(usersPtr==NULL)
    printf("cant open the users file");
  rewind(usersPtr);
  while (fread(&temp,sizeof(user),1,usersPtr)==1){
    if(strcmp(username,temp.username)==0){
      username_exist=1;
      printf("\033[94;1mPassword:\033[0m ");
      ClearInputBuffer();
      strcpy(password,TakePassword());
      if(strcmp(password,temp.password)==0){
        ClearScreen();
        MainMenu(username);
      }else{
        ClearScreen();
        printf("\033[91;1mUsername or Password is wrong!\033[0m\n");
        Login();
      }
    }
  }
  fclose(usersPtr);
  if(username_exist==0){
    ClearScreen();
    printf("\033[91;1mUsername not found please Sign up first!\033[0m");
    printf("\n---------\n\033[96;1m1.Sign up\n2.Log in\n3.Exit\033[0m\n---------\n");
    ClearInputBuffer();
    scanf("%c",&choice);
    if(choice=='1'){
      ClearScreen();
      SignUp();
    }else if(choice=='2'){
      ClearScreen();
      Login();
    }else{
      StartMenu();
    }
  }
}

void ShowUserScore(char username[]){
  userScore temp;
  SortScoreboard();
  FILE* sortedScoresPtr = fopen("sortedScores.txt","r");
  if(sortedScoresPtr==NULL){
    printf("cant open the sorted scores file");
  }
  rewind(sortedScoresPtr);
  while (fread(&temp,sizeof(userScore),1,sortedScoresPtr)==1){
    if(strcmp(username,temp.username)==0){
      printf("||\033[37m Your Score:\033[0m \033[92;1m%d\033[0m ||\n",temp.score);
    }
  }
  fclose(sortedScoresPtr);
}

void MainMenu(char username[]){
  char choice;
  printf("---------------------\n");
  ShowUserScore(username);
  printf("---------------------\n");
  printf("        \033[33;1mMenu\033[0m\n");
  printf("---------------------\n");
  printf("  \033[96;1m1.Start Game\033[0m\n");
  printf("  \033[96;1m2.Show Scoreboard\033[0m\n");
  printf("  \033[96;1m3.New word\033[0m\n  \033[96;1m4.Change password\033[0m\n");
  printf("  \033[96;1m5.Exit\033[0m\n---------------------\n");
  scanf("%c",&choice);
  if(choice=='1'){
    Game(username);
  }else if(choice=='2'){
    ShowScoreboard(username);
  }else if(choice=='3'){
     ClearScreen();
     NewWord(username);
  }else if(choice=='4'){
    ClearScreen();
    ClearInputBuffer();
    ChangePassword(username);
  }else if(choice=='5'){
    StartMenu();
  }else{
    ClearScreen();
    MainMenu(username);
  }
}

void ShowAsciiArt(char filename[]){
  FILE *AsciiArtPtr = fopen(filename,"r");
  if(AsciiArtPtr == NULL)
    printf("error");
  char read_string[128];
  while(fgets(read_string,sizeof(read_string),AsciiArtPtr)!= NULL){
    printf("\033[92;1m%s\033[0m",read_string);
  }  
}

void ShowMistakes_SW(int mistakes){
  printf("\033[97;1m|\033[0m");
  for(int i=0;i<mistakes;i++){
    printf(" \033[91;1mX\033[0m ");
    printf("\033[97;1m|\033[0m");
  }
  for(int i=0;i<(5-mistakes);i++){
    printf(" \033[92;1mV\033[0m ");
    printf("\033[97;1m|\033[0m");
  }
}

void ShowMistakes_LW(int mistakes){
  printf("\033[97;1m|\033[0m");
  for(int i=0;i<mistakes;i++){
    printf(" \033[91;1mX\033[0m ");
    printf("\033[97;1m|\033[0m");
  }
  for(int i=0;i<(10-mistakes);i++){
    printf(" \033[92;1mV\033[0m ");
    printf("\033[97;1m|\033[0m");
  }
}

void PrintLoser_SW(int mistakes){
  switch (mistakes) {
    case 0: 
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    break;
    case 1:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    printf("\n");
    ShowAsciiArt("L.txt"); 
    break;
    case 2:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    printf("\n");
    ShowAsciiArt("Lo.txt"); 
    break;
    case 3:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    printf("\n");
    ShowAsciiArt("Los.txt"); 
    break;
    case 4:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    printf("\n");
    ShowAsciiArt("Lose.txt"); 
    break;
    case 5:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_SW(mistakes);
    printf("\n");
    ShowAsciiArt("Loser.txt"); 
    break;
  }
}

void PrintLoser_LW(int mistakes){
  switch (mistakes) {
    case 0:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    break;
    case 1:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    break;
    case 2:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("L.txt");
    break;
    case 3: 
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("L.txt");
    break;
    case 4:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Lo.txt");
    break;
    case 5:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Lo.txt");
    break;
    case 6:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Los.txt");
    break;
    case 7:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Los.txt");
    break;
    case 8:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Lose.txt");
    break;
    case 9:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Lose.txt");
    break;
    case 10:
    printf("\n\033[97;1mMistakes:\033[0m ");
    ShowMistakes_LW(mistakes);
    printf("\n");
    ShowAsciiArt("Loser.txt");
    break;
  }
}

void AddScore(int score,char username[]){
  user temp;
  FILE* usersPtr = fopen("users.txt","r+");
  if(usersPtr==NULL)
    printf("cant open the users file");
  while (fread(&temp,sizeof(user),1,usersPtr)==1){
    if(strcmp(username,temp.username)==0){
      fseek(usersPtr,-sizeof(user),SEEK_CUR);
      temp.score += score;
      fwrite(&temp,sizeof(user),1,usersPtr);
    }
  }
  fclose(usersPtr);
}

void Game(char username[]){
  if(CountFileWords()==0){
    ClearScreen();
    ClearInputBuffer();
    printf("\033[91;1mwordslist is empty!\033[0m\n");
    MainMenu(username);
  }else{
    int mistakes=0,flag=0,choice=0,use_hint=0;
    char word[40],getword[40],remain_characters[40];
    memset(word,'\0',40);
    srand(time(0));
    int randomword=rand()%CountFileWords();
    FILE* wordlistPtr = fopen("wordlist.txt","r");
    if(wordlistPtr==NULL)
      printf("cant open the words file");
    rewind(wordlistPtr);
    while (fscanf(wordlistPtr,"%s",getword)==1){
      if(randomword==0)
        strcpy(word,getword);
      randomword--;
    }
    fclose(wordlistPtr);
    char ShownWord[40];
    memset(ShownWord,'\0',40);
    for(int i=0;i<strlen(word);i++){
      ShownWord[i]='_';
    }
    char input_char,j;
    char input_chars[40];
    memset(input_chars,'\0',40);
    if (strlen(word)<=8){
      j=0;
      for(;;){
        ClearScreen();
        printf("\033[97;1mInput letters:\033[0m ");
        for(int i=0;;i++){
          if(input_chars[i]=='\0')
            break;
          printf("\033[95;1m%c\033[0m,",input_chars[i]);
        }
        if(use_hint==0)
          printf("\n\033[97;1mAvailable Hints:\033[0m \033[93;1m#\033[0m");
        else
          printf("\n\033[97;1mAvailable Hints:\033[0m ");
        PrintLoser_SW(mistakes);
        if(mistakes==5){
        break;
        }
        printf("\n\n\033[97;1mGuess the word :\033[0m   ||  \033[96;1m%s\033[0m  ||\n",ShownWord);
        printf("\n\033[90mEnter your guess letter:\033[0m ");
        ClearInputBuffer();
        input_char=getchar();
        if(input_char==35){ //ASCII 35 is #
          if(use_hint==1){
            for(;;){
              printf("\033[91mYou have used your hint!\033[0m\n");
              printf("\n\033[90mEnter your guess letter:\033[0m ");
              printf("");
              ClearInputBuffer();
              input_char=getchar();
              if(input_char!=35)
                break;
            }
            if(input_char>64 && input_char<91){
            input_char=input_char+32;
            }
            if(CharExist(input_chars,input_char)!=0){
              printf("\033[90mYou have choosed this letter before.\nTry another one:\033[0m ");
              ClearInputBuffer();
              input_char=getchar();
              if(input_char>64 && input_char<91)
                input_char=input_char+32;
              for(;;){
                if(CharExist(input_chars,input_char)!=0){
                  printf("\033[90mSame! Try another letter please:\033[0m ");
                  ClearInputBuffer();
                  input_char=getchar();
                  if(input_char>64 && input_char<91)
                    input_char=input_char+32;
                }else break;
              }
            }
          }else{
            int t=0;
            for(;;){
              srand(time(0));    
              memset(remain_characters,'\0',40);
              for(int i=0;i<40;i++){
                for(int k=t;k<strlen(word);k++){
                  if(CharExist(input_chars,word[k])==0){
                    remain_characters[i]=word[k];
                    t=k+1;
                    break;
                  }
                }
              }
              input_char=remain_characters[(rand())%(strlen(remain_characters))];
              use_hint=1;
              AddScore(-3,username);
              break;
            }
          }
        }else{
          if(input_char>64 && input_char<91)
            input_char=input_char+32;
          if(CharExist(input_chars,input_char)!=0){
            printf("\033[90mYou have choosed this letter before.\nTry another one:\033[0m ");
            ClearInputBuffer();
            input_char=getchar();
            if(input_char>64 && input_char<91)
              input_char=input_char+32;
            for(;;){
              if(CharExist(input_chars,input_char)!=0){
                printf("\033[90mSame! Try another letter please:\033[0m ");
                ClearInputBuffer();
                input_char=getchar();
                if(input_char>64 && input_char<91)
                  input_char=input_char+32;
              }else break;
            }
          }
        }
        input_chars[j]=input_char;
        j++;
        flag=0;
        for(int i=0;i<strlen(word);i++){
          if(input_char==word[i]){
            flag++;
            ShownWord[i]=input_char;
          }
        }
        if(CharExist(ShownWord,'_')==0){
          ClearScreen();
          printf("\033[97;1mInput letters:\033[0m ");
          for(int i=0;;i++){
            if(input_chars[i]=='\0')
              break;
            printf("\033[95;1m%c\033[0m,",input_chars[i]);
          } 
          if(use_hint==0)
            printf("\n\033[97;1mAvailable Hints:\033[0m \033[93;1m#\033[0m\033[90m (Enter # to use for -3 points)\033[0m");
          else
            printf("\n\033[97;1mAvailable Hints:\033[0m ");
          PrintLoser_SW(mistakes);
          printf("\n\n\033[97;1mGuess the word :\033[0m   ||  \033[96;1m%s\033[0m  ||\n",ShownWord);
          printf("\n----------------- \033[93;1mYou Win\033[0m ------------------");
          AddScore(strlen(ShownWord),username);
          break;
        }
        if(flag==0){
          mistakes++;
        }
      }
      printf("\n-----------------\n\033[36;1m1.Play again\n2.Back to menu\033[0m\n-----------------\n");    
      ClearInputBuffer();
      scanf("%c",&choice);
      if(choice=='1'){
        Game(username);
      }else{
        ClearScreen();
        ClearInputBuffer();
        MainMenu(username);
      }
    }else{
      j=0;
      for(;;){
        ClearScreen();
        printf("\033[97;1mInput letters:\033[0m ");
        for(int i=0;;i++){
          if(input_chars[i]=='\0')
            break;
          printf("\033[95;1m%c\033[0m,",input_chars[i]);
        }
        if(use_hint==0)
          printf("\n\033[97;1mAvailable Hints:\033[0m \033[93;1m#\033[0m\033[90m (Enter # to use)\033[0m");
        else
          printf("\n\033[97;1mAvailable Hints:\033[0m ");
        PrintLoser_LW(mistakes);
        if(mistakes==10)
          break;
        printf("\n\n\033[97;1mGuess the word :\033[0m   ||  \033[96;1m%s\033[0m  ||\n",ShownWord);
        printf("\n\033[90mEnter your guess letter:\033[0m ");
        ClearInputBuffer();
        input_char=getchar();
        if(input_char==35){ //Ascii code of # 
          if(use_hint==1){
            for(;;){
              printf("\033[91mYou have used your hint!\033[0m\n");
              printf("\n\033[90mEnter your guess letter:\033[0m ");
              printf("");
              ClearInputBuffer();
              input_char=getchar();
              if(input_char!=35)
                break;
            }
            if(input_char>64 && input_char<91)
              input_char=input_char+32;
            if(CharExist(input_chars,input_char)!=0){
              printf("\033[90mYou have choosed this letter before.\nTry another one:\033[0m ");
              ClearInputBuffer();
              input_char=getchar();
              if(input_char>64 && input_char<91)
                input_char=input_char+32;
              for(;;){
                if(CharExist(input_chars,input_char)!=0){
                  printf("\033[90mSame! Try another letter please:\033[0m ");
                  ClearInputBuffer();
                  input_char=getchar();
                  if(input_char>64 && input_char<91)
                    input_char=input_char+32;
                }else break;
              }
            }
          }else{
            int t=0;
            for(;;){
              srand(time(0));    
              memset(remain_characters,'\0',40);
              for(int i=0;i<40;i++){
                for(int k=t;k<strlen(word);k++){
                  if(CharExist(input_chars,word[k])==0){
                    remain_characters[i]=word[k];
                    t=k+1;
                    break;
                  }
                }
              }
              input_char=remain_characters[(rand())%(strlen(remain_characters))];
              use_hint=1;
              AddScore(-3,username);
              break;
            }
          }
        }else{
          if(input_char>64 && input_char<91)
            input_char=input_char+32;
          if(CharExist(input_chars,input_char)!=0){
            printf("\033[90mYou have choosed this letter before.\nTry another one:\033[0m ");
            ClearInputBuffer();
            input_char=getchar();
            if(input_char>64 && input_char<91)
              input_char=input_char+32;
            for(;;){
              if(CharExist(input_chars,input_char)!=0){
                printf("\033[90mSame! Try another letter please:\033[0m ");
                ClearInputBuffer();
                input_char=getchar();
                if(input_char>64 && input_char<91)
                  input_char=input_char+32;
              }else break;
            }
          }
        }
        input_chars[j]=input_char;
        j++;
        flag=0;
        for(int i=0;i<strlen(word);i++){
          if(input_char==word[i]){
            flag++;
            ShownWord[i]=input_char;
          }
        }
        if(CharExist(ShownWord,'_')==0){
          ClearScreen();
          printf("\033[97;1mInput letters:\033[0m ");
          for(int i=0;;i++){
            if(input_chars[i]=='\0')
            break;
            printf("\033[95;1m%c\033[0m,",input_chars[i]);
          }
          if(use_hint==0)
            printf("\n\033[97;1mAvailable Hints:\033[0m \033[93;1m#\033[0m");
          else
            printf("\n\033[97;1mAvailable Hints:\033[0m ");
          PrintLoser_SW(mistakes);
          printf("\n\n\033[97;1mGuess the word :\033[0m   ||  \033[96;1m%s\033[0m  ||\n",ShownWord);
          printf("\n----------------- \033[93;1mYou Win\033[0m ------------------");
          AddScore(strlen(ShownWord),username);
          break;
        }
        if(flag==0)
          mistakes++;
      }
      printf("\n-----------------\n\033[36;1m1.Play again\n2.Back to menu\033[0m\n-----------------\n");  
      ClearInputBuffer();
      scanf("%c",&choice);
      if(choice=='1'){
        Game(username);
      }else{
        ClearScreen();
        ClearInputBuffer();
        MainMenu(username);
      }
    }
  }
}

void SortScoreboard(){
  userScore sorted_scores[100],temp;
  user temp2[100];
  int i=0;
  FILE *usersPtr = fopen("users.txt","r");
  if(usersPtr==NULL)
    printf("cant open the users file");
  while (fread(&temp2[i],sizeof(user),1,usersPtr)==1){
    memset(sorted_scores[i].username,'\0',40);
    strcpy(sorted_scores[i].username,temp2[i].username);
    sorted_scores[i].score=temp2[i].score;
    i++;
  }
  fclose(usersPtr);
  for(int j=1;j<i;j++){
    for(int k=0;k<(i-j);k++){
      if(sorted_scores[k+1].score>sorted_scores[k].score){
        temp=sorted_scores[k];
        sorted_scores[k]=sorted_scores[k+1];
        sorted_scores[k+1]=temp;
      }
    }
  }
  FILE* sortedScoresPtr=fopen("sortedScores.txt","w");
  for(int j=0;j<i;j++){
    fwrite(&sorted_scores[j],sizeof(userScore),1,sortedScoresPtr);
  }
  fclose(sortedScoresPtr);
}

void ShowScoreboard(char username[]){
  int rank=1;
  ClearScreen();
  SortScoreboard();
  printf("   \t\t\033[33;1mScoreboard\033[0m         \n--------------------------------------------\n");
  userScore temp;
  FILE* sortedScoresPtr = fopen("sortedScores.txt","r");
  if(sortedScoresPtr==NULL)
    printf("cant open the sorted scores file");
  rewind(sortedScoresPtr);
  while (fread(&temp,sizeof(userScore),1,sortedScoresPtr)==1){
    if(CountFileUsers()>5){
      if(rank<4)
        printf("\033[92;1m%d.\033[0m",rank);
      else if(rank>(CountFileUsers()-3))
        printf("\033[91;1m%d.\033[0m",rank);
      else 
        printf("\033[37;1m%d.\033[0m",rank);
    }else if(CountFileUsers()>1){
      if(rank==1)
        printf("\033[92;1m%d.\033[0m",rank);
      else 
        printf("\033[91;1m%d.\033[0m",rank);
    }else
      printf("\033[37;1m%d.\033[0m",rank);
    rank++;
    if(rank<11)
      printf(" ");
    printf("\033[96;1m%s\033[0m ",temp.username);
    for(int i=0;i<(36-strlen(temp.username));i++){
      printf("\033[90m.\033");
    }
    printf(" \033[97;1m%d\033[0m",temp.score);
    if(strcmp(username,temp.username)==0){
      printf(" \033[95;1m<-- You\033[0m");
    }
    printf("\n");
  }
  fclose(sortedScoresPtr);
  printf("--------------------------------------------");
  printf("\n\033[90;1mPress any key to back to menu\033[0m");
  ClearInputBuffer();
  getchar();
  ClearScreen();
  MainMenu(username);
}

void ChangePassword(char username[]){
  char password[40],confirm[40];
  int flag=0;
  user temp;
  FILE* usersPtr = fopen("users.txt","r+");
  if(usersPtr==NULL)
    printf("cant open the users file");
  rewind(usersPtr);
  while (fread(&temp,sizeof(user),1,usersPtr)==1){
    if(strcmp(username,temp.username)==0){
      printf("\033[94;1mEnter current password:\033[0m ");
      strcpy(password,TakePassword());
      printf("\n");
      if(strcmp(password,temp.password)==0){
        for(;;){
          for(;;){
            printf("\033[94;1mEnter new password:\033[0m ");
            strcpy(password,TakePassword());
            if(ValidPass(password)==1)
              break;
          }
          printf("\033[94;1mEnter it again:\033[0m ");
          strcpy(confirm,TakePassword());
          if(strcmp(password,confirm)==0){
            fseek(usersPtr,-sizeof(user),SEEK_CUR);
            strcpy(temp.password,password);
            fwrite(&temp,sizeof(user),1,usersPtr);
            printf("\033[92;1mpassword changed successfully.\033[0m\n");
            break;
            flag++;
          }else{
            printf("\033[91;1m\nDoes'nt match!\033[0m\n");
          }
        }
        if(flag>0){
          break;
        }
      }else{
        printf("\033[91;1mWrong!\033[0m\n");
        ChangePassword(username);
      }
    }
  }
  fclose(usersPtr);
  printf("--------------------------------------------");
  printf("\n\033[90;1mPress any key to back to menu\033[0m");
  ClearInputBuffer();
  getchar();
  ClearScreen();
  MainMenu(username);
}

void NewWord(char username[]){
  char word[50],newword[50];
  int word_exist=0;
  char choice;
  ClearScreen();
  printf("\033[94;1mEnter a word to add to the wordslist:\033[0m ");
  scanf("%s",&newword);
  FILE* wordlistPtr = fopen("wordlist.txt","a+");
  if(wordlistPtr==NULL){
    printf("cant open the wordlist file");
  }
  rewind(wordlistPtr);
  while(fscanf(wordlistPtr,"%s",word)==1){
    if(strcmp(newword,word)==0){
      word_exist=1;
      ClearScreen();
      printf("\033[91;1mThis word was in the list\033[0m\n");
      printf("-------------------\n\033[96;1m1.Try another word\n2.Back to menu\033[0m\n-------------------\n");
      ClearInputBuffer();
      scanf("%c",&choice);
      if(choice=='1'){
        fclose(wordlistPtr);
        NewWord(username);
      }else{
        ClearScreen();
        MainMenu(username);
      }
      break;
    }
  }
  if(word_exist==0){
    fprintf(wordlistPtr,"%s\n",newword);
    ClearScreen();
    printf("\"\033[97;1m%s\033[0m\" \033[92;1madded to the list.\033[0m\n",newword);
    printf("-------------------\n\033[96;1m1.Add another word\n2.Back to menu\033[0m\n-------------------\n");
    ClearInputBuffer();
    scanf("%c",&choice);
    if(choice=='1'){
      fclose(wordlistPtr);
      NewWord(username);
    }else{
      fclose(wordlistPtr);
      ClearScreen();
      ClearInputBuffer();
      MainMenu(username); 
    }
  }
  fclose(wordlistPtr);
}

void ClearScreen(){
 printf("\e[1:1H\e[2J"); 
}

int CharExist(char string[],char c){
  int checker=0;
  for (int i=0;i<strlen(string);i++){
    if(string[i]==c){
      checker=1;
    }
  }
  return checker;
}

int CountFileWords(){
  char word[40];
  int words_count=0;
  FILE* wordlistPtr = fopen("wordlist.txt","r");
  if(wordlistPtr==NULL)
    printf("cant open the words file");
  rewind(wordlistPtr);
  while (fscanf(wordlistPtr,"%s",word)==1){
    words_count++;
  }
  fclose(wordlistPtr);
  return words_count;
}

int CountFileUsers(){
  userScore temp;
  int users_count=0;
  FILE* sortedScoresPtr = fopen("sortedScores.txt","r");
  if(sortedScoresPtr==NULL)
    printf("cant open the words file");
  rewind(sortedScoresPtr);
  while (fread(&temp,sizeof(userScore),1,sortedScoresPtr)==1){
    users_count++;
  }
  fclose(sortedScoresPtr);
  return users_count;
}

void ClearInputBuffer(){
while ((getchar())!='\n'); 
}
