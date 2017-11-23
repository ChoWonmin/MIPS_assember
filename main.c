#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct label{
    int index;
    char* name;
    int value;
}label;

label scan_label(char* inst, int* labelNum);
int main(int argc, char* argv[])
{
    int instNum=100;
    char inst_list[instNum][128];

    char file_name[64] = "test01.txt";
    printf("enter the file name :: ");
    scanf("%s",file_name);

    int pc=0;

    label label_table[20];
    int labelNum = 0;
    float res[32] = {0.0f};

    FILE* fin = fopen(file_name,"r");
    if(fin==NULL){
        printf("file updload fail\n");
        return -1;
    }

    int i=0;
    while (fgets(inst_list[i], 128, fin)) {
        scan_label(inst_list[i++],&labelNum);
    }
    instNum=i;

    for(i=0;i<instNum;i++){
        printf("%s",inst_list[i]);
    }

    printf("\n");
    fclose(fin);

    return 0;
}
label scan_label(char* inst, int* labelNum){

    label res;
    char temp[128];
    strcpy(temp,inst);

    if(strchr(temp,':')!=NULL){
        res.index = labelNum;
        res.name = strtok(temp,":");
        strcpy(temp,inst);

        char* temp2 = strstr(temp,".word");
        if(temp2){
            temp2 = strchr(temp2,' ');
            trim(temp2);
            res.value = atoi(temp2);
        }
    }

    labelNum++;
    return res;
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





