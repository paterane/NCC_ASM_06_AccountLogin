// Program    : ASM06_AccountLogin.c
// Created by : Peter Oo
// Created on : 1/18/2023 (1:01 PM).
// About      :

#include "include/ASM06_AccountLogin.h"

typedef struct{
    char name[30];
    int age;
    char id[30];
    char email[50];
    char pass[30];
}db;

void printTitle(char *title, char frame);
void loadingDataFromFile();
void logInSection();
void management();
int accountFound(char *email, char *password);
void displayInfo(db user);
void game_in();
void play_corner(db user);
int isNameUnique(char *str);
int isEmailUnique(char *str);
int isMailValid(char *email, char *r_pattern);
void registration_corner();
void savingDataToFile();

db *player_info;
int global_idx = 0;

int main(){
    player_info = (db*)(malloc((global_idx + 1) * sizeof(db)));
    loadingDataFromFile();
    while(1){
        printTitle("Player Center", '*');
        colorPrint("<g>Hello</g>, <y>Log in, Sign up, manage or Exit</y>: ");
        char *guide = scanner(0);
        wordLower(guide);
        if(stringCmp(guide, "log in") != -1)
            logInSection();
        else if(stringCmp(guide, "sign up") != -1)
            registration_corner();
        else if(stringCmp(guide, "manage") != -1)
            management();
        else if(stringCmp(guide, "exit") != -1){
            savingDataToFile();
            break;
        }
        else{
            colorPrint("<r>[-]Wrong Input!!!</r>\n");
        }
        free(guide);
    }
    free(player_info);
    return 0;
}

void logInSection(){
    char *uEmail = NULL; char *upass = NULL;
    printTitle("Account Log In", '#');
    while(1){
        while(1){
            printf("Enter your Email: ");
            uEmail = scanner(0);
            if(isMailValid(uEmail, "a-z0-9"))
                break;
            else{
                colorPrint("<r>[-]Email format not valid!</r>\n");
            }

        }
        printf("Enter your password: ");
        upass = scanner(1);
        if((stringCmp(uEmail, "\0") != -1) && (stringCmp(upass, "\0") != -1)){
            colorPrint("<r>Email and password are required. Must be filled!!!</r>\n");
        }
        else
            break;
    }
    int user_idx;
    if((user_idx = accountFound(uEmail, upass)) != -1){
        free(uEmail);
        free(upass);
        play_corner(player_info[user_idx]);
    }
    else{
        colorPrint("<r>[-]Account Not Found!</r> Please, Sign up\n");
    }
}

/*
 * Access and modify all the accounts
 * */
void management(){
    printTitle("Administration Corner", '*');
    while(1){
        colorPrint("Hi, Admin: ");
        char *userIn = scanner(0);
        wordLower(userIn);
        if(stringCmp(userIn, "back") != -1)
            break;
        else
            colorPrint("<r>[-]Wrong Input!!!</r>\n");
    }
}

/*
 * RETURN: -1 if not found, user index if found
 * */
int accountFound(char *email, char *password){
    int i;
    int found = false;
    for(i=0; i < global_idx; i++){
        if((stringCmp(email, player_info[i].email) != -1) &&
           (stringCmp(password, player_info[i].pass) != -1)){
            found = true;
            break;
        }
    }
    if(found)
        return i;
    else
        return -1;
}

void game_in(){
    int count = 0;
    srand(time(NULL));
    printf("Guss 0 -> 9\n");
    printf("If any 2 of 5 rounds is correctly predicted, your are in good luck..\n");
    for(int i=0; i<5; i++){
        int num = rand() % 10;
        colorPrint("<y>Enter your number</y>: ");
        int guess = 0;
        scanf("%d", &guess);
        if(guess == num){
            count++;
            colorPrint("<r>What a guess...\nYou guessed right!!!</r>\n");
        }
        else{
            colorPrint("<b>Oops..., you're... wrong</b>\n");
        }
    }
    if(count >= 2){
        printf("Well done, sir!\tYou are lucky...\n");
    }
    else{
        printf("Well..., you can try again..\n");
    }
}

void displayInfo(db user){
    colorPrint("user name    : <b>%s\n</b>user age     : <b>%d\n</b>user ID      : <b>%s\n</b>", user.name, user.age, user.id);
    colorPrint("user email   : <b>%s\n</b>user password: <b>%s\n</b>", user.email, user.pass);

}

void play_corner(db user){
    printTitle("Test Your Luck", '$');
    while(1){
        colorPrint("Hello, <y>%s...\n</y>Would you like to <b>play, "
                   "check</b> your info or ,<b>back</b> to Player Center? ", user.name);
        char *userIn = scanner(0);
        wordLower(userIn);
        if(stringCmp(userIn, "play") != -1){
            game_in();
        }
        else if(stringCmp(userIn, "check") != -1){
            displayInfo(user);
        }
        else if(stringCmp(userIn, "back") != -1){
            break;
        }
        else{
            colorPrint("<r>[-]Wrong Input!!!</r>\n");
        }
    }
}

/*
 * RETURN: 0 if there is a same name in database, 1 if it is unique
 * */
int isNameUnique(char *str){
    int unique = true;
    for(int i=0; i < global_idx; i++){
        if(stringCmp(str, player_info[i].name) != -1){
            unique = false;
            break;
        }
    }
    return unique;
}

/*
 * RETURN: 0 if there is a same Email in database, 1 if it is unique
 * */
int isEmailUnique(char *str){
    int unique = true;
    for(int i=0; i < global_idx; i++){
        if(stringCmp(str, player_info[i].email) != -1){
            unique = false;
            break;
        }
    }
    return unique;
}

/*
 * RETURN: 0 if not email format, 1 if it is an email.
 * */
int isMailValid(char *email, char *r_pattern){ // Format: xxxx@[A-Za-z0-9].com
    int indexA = indexOf(email, "@");
    int indexB = indexOf(email, ".com");
    if(indexA == -1 || indexB == -1)
        return 0;
    char *emailName = subString(email, 0, indexA);
    if(emailName == NULL)
        return 0;
    if(!regExpress(emailName, "a-z0-9.-.")){
        colorPrint("<r>An email shouldn't contain any special or upper case characters.</r>\n");
        return 0;
    }
    if(stringCount(emailName, ".") >= 2){
        colorPrint("<r>An email shouldn't contain \".\" more than once!!</r>\n");
        return 0;
    }
    if(email[indexB+4] != '\0')
        return 0;
    char *domain = subString(email, indexA+1,indexB);
    if(domain == NULL)
        return 0;
    if(regExpress(domain, r_pattern))
        return 1;
    else
        return 0;
}

/*
 * RETURN: 0 if there are any spaces, 1 if it is valid
 * */
int isPassValid(char *password){
    if(regExpress(password, "!-~"))
        return 1;
    else
        return 0;
}

void registration_corner(){
    printTitle("Registration Corner", '*');
    while(1){
        colorPrint("Enter a <y>name</y>: ");
        char *name = scanner(0);
        wordLower(name);
        if(isNameUnique(name)){
            stringCopy(name, player_info[global_idx].name);
            break;
        }
        else{
            colorPrint("<r>[-]Someone chose the same name, please enter a different name.</r>\n");
        }

    }
    printf("Welcome %s, ", player_info[global_idx].name);
    colorPrint("Enter your <y>age</y>: ");
    scanf("%d", &player_info[global_idx].age);
    colorPrint("Enter your <y>ID</y>: ");
    char *id = scanner(0);
    stringCopy(id, player_info[global_idx].id);
    while(1){
        colorPrint("Enter your <y>Email</y>: ");
        char *email = scanner(0);
        wordLower(email);
        if(isMailValid(email, "a-z0-9")){ //r_pattern : A-Za-z >> all possible email domains
            if(isEmailUnique(email)){
                stringCopy(email, player_info[global_idx].email);
                break;
            }
            else
                colorPrint("<r>[-]This email has already existed.\n</r>");
        }
        else{
            colorPrint("<r>[-]Please, Enter a valid email format</r>\n");
        }
    }
    while(1){
        colorPrint("Enter a <y>password</y>    : ");
        char *first_pass = scanner(1);
        if(isPassValid(first_pass)){
            colorPrint("Re-type the <y>password</y>: ");
            char *second_pass = scanner(1);
            if((stringCmp(first_pass, second_pass) != -1) && isPassValid(second_pass)){
                stringCopy(first_pass, player_info[global_idx].pass);
                break;
            }
            else{
                colorPrint("<r>[-]Passwords not matched!!!</r>\n");
            }
        }
        else{
            colorPrint("<r>[-]A password shouldn't contain space!\n");
        }
    }
    colorPrint("<b>Your registration succeeded.</b> <a>Thank you</a>\n");
    global_idx++;
    player_info = (db*)(realloc(player_info, (global_idx + 1) * sizeof(db)));
}

void loadingDataFromFile(){
    printTitle("Loading Data", '/');
    FILE *fp = fopen("player_info.dat", "r");
    if(fp != NULL){                     //WTF C, brilliant format
        while(fscanf(fp, "\n%[^,]%*c %d %s %s %s", player_info[global_idx].name, &player_info[global_idx].age,
                     player_info[global_idx].id, player_info[global_idx].email, player_info[global_idx].pass) != EOF){
            wordLower(player_info[global_idx].name);
            wordLower(player_info[global_idx].email);
            global_idx++;
            player_info = (db*)(realloc(player_info, (global_idx + 1) * sizeof(db)));
        }
    }
    fclose(fp);
}

void savingDataToFile(){
    printTitle("Saving Data", '.');
    FILE *fp = fopen("player_info.dat", "w");
    if(fp != NULL){
        for(int i=0; i < global_idx; i++){
            fprintf(fp, "%s,%d %s %s %s\n", player_info[i].name, player_info[i].age,
                                            player_info[i].id, player_info[i].email,
                                            player_info[i].pass);
        }
        printf("Saving Data to FILE is complete...\n");
    }
    else{
        colorPrint("<r>[-]Error! Saving Data to FILE</r>\n");
    }
    fclose(fp);
}

void printTitle(char *title, char frame){
    int space = 30;
    int end_cap = 1;
    /************************************/
    int tit_len = stringLen(title);
    int tot_len = tit_len + 2*space + 2*end_cap;
    char frames[tot_len+1];
    char spaces[space+1];
    for(int i=0; i<tot_len; i++) frames[i] = frame;
    frames[tot_len] = '\0';
    for(int i=0; i<space; i++) spaces[i] = 32;

    spaces[space] = '\0';
    colorPrint("<b>%s\n%c</b>",frames,frame);
    colorPrint("<p>%s%s%s</p>",spaces,title,spaces);
    colorPrint("<b>%c\n%s\n</b>",frame,frames);
}

