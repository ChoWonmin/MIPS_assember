#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct label{
    int index;
    char name[10];
    int value;
}label;

int main(int argc, char* argv[])
{
    int instNum=100;
    char inst_list[instNum][128];

    char file_name[64] = "test01.txt";
    //printf("enter the file name :: ");
    //scanf("%s",file_name);

    int pc=0;

    int labelNum = 0;
    label symbol_table[20];
    float res[32] = {0.0f};

    FILE* fin = fopen(file_name,"r");
    if(fin==NULL){
        printf("file updload fail\n");
        return -1;
    }

    int i=0;
    char line[128];
    while (fgets(line, 128, fin)) {

        strcpy(inst_list[i],line);
        char temp[128];
        strcpy(temp,line);

        if(strchr(temp,':')!=NULL){ //label이 있는 경우
            symbol_table[labelNum].index=i;
            strcpy(symbol_table[labelNum].name,strtok(temp,":"));
            strcpy(temp,line);

            char* temp2 = strstr(temp,".word");
            if(temp2){
                temp2 = strchr(temp2,' ');
                trim(temp2);
                symbol_table[labelNum].value = atoi(temp2);
            }else{
                symbol_table[labelNum].value = -1;
            }
            labelNum++;
        }
        i++;
    }
    instNum=i;

    for(i=0;i<instNum;i++){
        //printf("%s",inst_list[i]);
    }

    for(i=0;i<labelNum;i++){
        printf("%d , %s , %d\n",symbol_table[i].index,symbol_table[i].name, symbol_table[i].value);
    }

    printf("\n");
    fclose(fin);

    return 0;
}
int trim (char *line)
{
        int len = 0;
        char cpTrim[128];
        int xMan = 0;
        int i;

        len = strlen(line);
        if (len >= 128)
        {
                puts ("string too long");
                return -1;
        }

        strcpy(cpTrim, line);

        for (i = 0; i < len; i++)
        {
                if (cpTrim[i] == ' ' || cpTrim[i] == '\t')
                        xMan++;
                else
                        break;
        }

        for (i = len-2; i >= 0; i--)
        {
                if (cpTrim[i] == ' ' || cpTrim[i] == '\t' || cpTrim[i] == '\n')
                        cpTrim [i] = '\0';
                else
                        break;
        }

        strcpy (line, cpTrim+xMan);

        return strlen(line);
}





