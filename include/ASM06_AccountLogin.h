// Created by : Peter Oo
// Created on : 1/18/2023 (1:01 PM).
// About      : Functions for AccLogin Program


#ifndef NCC_CLASS_ASM06_ACCOUNTLOGIN_H
#define NCC_CLASS_ASM06_ACCOUNTLOGIN_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#define true  1
#define false 0

void setColor(char color);
void resetColor();
void wordUpper(char *word);
void wordLower(char *word);
int stringLen(const char *str); // note this function is only available for char array or string not for int array
int stringCmp(char *str1, char *str2);
void stringCopy(char *origin, char *buff);
void stringConcat(char **main, char *sub);
int indexOf(char *str, char *idxStr);
char *subString(char *str, int idxA, int idxB);
int regExpress(char *str, char *pattern);
int stringCount(char *str, char *idxStr);
char *scanner(int pass);
unsigned short curColor;

/* Windows attribute control section */
void setColor(char color){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &info);
    curColor = info.wAttributes;
    switch(color){
        case 'r':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_RED |
                                     FOREGROUND_INTENSITY);
            break;
        case 'g':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_GREEN |
                                     FOREGROUND_INTENSITY);
            break;
        case 'b':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_BLUE |
                                     FOREGROUND_INTENSITY);
            break;
        case 'p':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_BLUE |
                                     FOREGROUND_RED |
                                     FOREGROUND_INTENSITY);
            break;
        case 'y':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_GREEN |
                                     FOREGROUND_RED |
                                     FOREGROUND_INTENSITY);
            break;
        case 'a':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_GREEN |
                                     FOREGROUND_BLUE |
                                     FOREGROUND_INTENSITY);
            break;
        case 'w':
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),
                                     FOREGROUND_GREEN |
                                     FOREGROUND_BLUE |
                                     FOREGROUND_RED |
                                     FOREGROUND_INTENSITY);
            break;
        default:
            printf("[-] any of such color code found!\n"); resetColor();
            break;
    }
}
void resetColor(){
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), curColor);
}
/***********************************************************/

void wordUpper(char *word){
    int i = 0;
    while(word[i] != '\0'){
        if(word[i] >= 97 && word[i] <= 122) word[i] -= 32;
        i++;
    }
}

void wordLower(char *word){
    int i = 0;
    while(word[i] != '\0'){
        if(word[i] >= 65 && word[i] <= 90) word[i] += 32;
        i++;
    }
}

/* Note: This func is only valid for char array, not for int array */
int stringLen(const char *str){
    int n=0;
    while(str[n++] != '\0');
    return n-1;
}

/* RETURN: -1 if not match , char count value if match*/
int stringCmp(char *str1, char *str2){
    int cnt=0;
    if(stringLen(str1) == stringLen(str2)){ //if length same, pass OR terminate func and return -1
        while(str1[cnt] != '\0'){
            if(str1[cnt] == str2[cnt])
                cnt++;
            else
                return -1;
        }
        return cnt;  //if every character is matched, return cnt
    }
    else
        return -1;
}

/*
 * Copy the content of origin to buff
 * */
void stringCopy(char *origin, char *buff){
    int len = stringLen(origin);
    for(int i=0; i<len; i++){
        buff[i] = origin[i];
    }
    buff[len] = '\0';
}

/*
 * combination of main string and sub string in such a way that sub join at the end of main*/
void stringConcat(char **main, char *sub){
    int main_len = stringLen(*main);
    int sub_len = stringLen(sub);
    int total_len = main_len+sub_len;
    char *temp = (char*)(malloc((total_len+1)*sizeof(char)));
    for(int i=0; i < main_len;i++){
        temp[i] = *(*main+i); //or main[0][i],but, can't do like this *main[i], haha, WTF "C"
    }
    for(int i=0; i<sub_len; i++){
        temp[main_len+i] = sub[i];
    }
    temp[total_len] = '\0';
    *main = temp;
}

// RETURN: -1 if indexString longer than the string or not found,and index position if found
// NOTE: this function will only work on first occurrence of the index.
int indexOf(char *str, char *idxStr){
    int cnt=0; int found=0;
    int str_len = stringLen(str);
    int idxStr_len = stringLen(idxStr);
    if(idxStr_len <= str_len){
        for(;cnt < (str_len+idxStr_len); cnt++){
            if(idxStr[found] == str[cnt]){
                found++;
                if(found == idxStr_len)
                    break;
            }
            else if(found){
                cnt--;
                found=0;
            }
        }
        if(found == idxStr_len)
            return cnt-(found-1);
        else
            return -1; // if not found
    }
    else{
        printf("Index length longer than the string!\n");
        return -1;
    }
}

/* continuous characters trimming in the string
 * continuous characters started from idxA upto idxB excluding character at idxB
 * RETURN: NULL if error indexing
 * */
char *subString(char *str, int idxA, int idxB){
    int length = stringLen(str); int x = 0;
    char *result = (char*)(malloc(length*sizeof(char)));
    if(idxA >= length || idxB > length || idxA >= idxB){
        printf("[ERROR!] indexing invalid!!!\n");
        return NULL;
    }
    for(int i=idxA; i<idxB; i++){
        result[x++] = str[i];
    }
    result[x] = '\0';
    return result;
}

/* "word Checker whether each character in the string existed in the patterns or not"
 * patterns => [start char] - [end char]
 * example  => a-z a,b,c,....,x,y,z
 * example  => 0-9 0,1,2,....,8,9
 * you can put any sequences from ascii table
 * RETURN: 1 if each character existed in patterns else 0
 * */
int regExpress(char *str, char *pattern){
    char pat_char[100] = {'\0'}; int pat_idx = 0;
    int pat_length = stringLen(pattern);
    int str_length = stringLen(str);
    for(int index=1; index < pat_length;){
        if(pattern[index] == '-'){
            if(pattern[index-1] <= pattern[index+1]){
                for(char ch=pattern[index-1]; ch<=pattern[index+1]; ch++){
                    pat_char[pat_idx++] = ch;
                }
            }
            else
                printf("[-] Found pattern index error!!![pattern excluded]\n");
        }
        else{
            printf("[-] Found strange patten!!![pattern excluded]\n");
        }
        index += 3;
    }
    int patChar_length = stringLen(pat_char);
    for(int i=0; i<str_length; i++){
        int found = false;
        for(int j=0; j<patChar_length; j++){
            if(str[i] == pat_char[j]){
                found = true;
                break;
            }
        }
        if(!found)
            return 0;
    }
    return 1;
}

/* number of occurrence of a substring "x" or "x.."
 * RETURN:-1 if error, 0 if not found, count of the occurrence if found
 * */
int stringCount(char *str, char *idxStr){
    int cnt=0; int found=0; int n_occur = 0;
    int str_len = stringLen(str);
    int idxStr_len = stringLen(idxStr);
    if(idxStr_len > str_len){
        printf("[Error!] index > string\n");
        return -1; // if error
    }
    for(;cnt < (str_len+idxStr_len); cnt++){
        if(idxStr[found] == str[cnt]){
            found++;
            if(found == idxStr_len){ //not to exceed idxStr len
                n_occur++;
                found=0;
            }
        }
        else if(found){
            cnt--;
            found=0;
        }
    }
    return n_occur;
}

/* Implemented User Input Function
 * if typing password use pass = 1, if not, use pass = 0
 * */
char *scanner(int pass){
    char *buff =(char*)(malloc(sizeof(char)));
    int ch;
    int idx = 0;
    while((ch=getch()) != '\r'){
        if(ch >= 32 && ch <= 126){
            if(pass){
                if(ch != 32){
                    buff[idx++] = (char)ch;
                    printf("*");
                    buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
                }
            }
            else{
                buff[idx++] = (char)ch;
                printf("%c", ch);
                buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
            }
        }
        else if(ch == '\b'){
            if(idx>0){
                buff[--idx] = '\0';
                printf("\b%c\b", 32);
                buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
            }
        }
        else if(ch == '\t'){
            for(int i=0; i<4; i++){
                buff[idx++] = 32;
                printf("%c", 32);
                buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
            }
        }
        else if(ch == 127){
            if(buff[idx-1] == 32){
                while(buff[idx-1] == 32 && idx > 0){
                    buff[--idx] = '\0';
                    printf("\b%c\b", 32);
                    buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
                }
            }
            else{
                while(buff[idx-1] != 32 && idx > 0){
                    buff[--idx] = '\0';
                    printf("\b%c\b", 32);
                    buff = (char*)(realloc(buff, (idx+1)*sizeof(char)));
                }
            }
        }
        else if(ch == 3)
            exit(0);
        buff[idx] = '\0';
    }
    buff[idx] = '\0';
    printf("\n");
    return buff;
}

/*  str => is any string in addition to your favourite 7 colors in the string
 *  you may your string like "<r>Hi<\r> <b>Hello<\b> <p>friends</p>
 *  r => red, g => green, b=> blue, w=>while, p=>purple, y=>yellow, a=>aqua
 *  <x> is set the color, and </x> is the color reset. So, be careful not to forget </x>
 * */
void colorPrint(char *fmt,...){
    int size = 256;
    char *buff = (char*)(malloc(size*sizeof(char)));
    va_list vl;
    va_start(vl,fmt);
    vsprintf(buff,fmt,vl);
    va_end(vl);
    size = stringLen(buff)+1;
    buff = (char*)(realloc(buff, size*sizeof(char)));
    int idx = 0;
    while(buff[idx] != '\0'){
        if(buff[idx] == '<'){
            char color = buff[idx+1];
            int end_cap = indexOf(buff, "</");
            setColor(color);
            printf("%s", subString(buff,idx+3,end_cap));
            resetColor();
            int len = stringLen(buff);
            end_cap += 4;
            if(end_cap < len){
                buff = subString(buff, end_cap,len);
                idx = 0;
            }
            else
                break;
        }
        else{
            idx = indexOf(buff, "<");
            if(idx != -1){
                printf("%s", subString(buff, 0, idx));
            }
            else{
                printf("%s", buff);
                break;
            }
        }
    }
    free(buff);
}

#endif //NCC_CLASS_ASM06_ACCOUNTLOGIN_H
