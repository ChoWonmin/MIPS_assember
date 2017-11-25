#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct label{
    int index;
    char name[10];
    int value;
}label;

typedef struct opcode{
    char name[10];
    char* code;
    char type;
    char funct[6];
}opcode;

int main(int argc, char* argv[])
{
    int instNum=100;
    char inst_list[instNum][128];

    char file_name[32] = "test01.txt";
    printf("enter the file name :: ");
    scanf("%s",file_name);

    opcode opcode_list[21] = {
                    {"addiu","001001",'i'},{"addu","000000",'r',"100001"},{"and","000000",'r',"100100"},{"andi","001100",'i'},{"beq","000100",'i'},{"bne","000110",'i'},{"j","000010",9,'j'},
                    {"jal","000011",'j'},{"jr","000000",'j',"001000"},{"lui","111111",'i'},{"lw","100011",'i'},{"la","000000",'r',"000000"},{"nor","000000",'r',"100111"},{"or","000000",'r',"100101"},
                    {"ori","001101",'i'},{"sltiu","001011",'i'},{"sltu","000000",'r',"101011"},{"sll","000000",'r',"000000"},{"srl","000000",'r',"000010"},{"sw","101011",'i'},{"subu","000000",'r',"100011"}
                };

    int pc=0;

    int labelNum = 0;
    label symbol_table[20];
    float res[32] = {0.0f};

    FILE* fin = fopen(file_name,"r");
    FILE* fout = fopen("result.txt","w");
    if(fin==NULL){
        printf("file updload fail\n");
        return -1;
    }

    // first pass
    int i=0;
    char line[128];
    while (fgets(line, 128, fin)) {

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
                if(!strncmp(temp2,"0x",2))
                    strcpy(temp2,temp2+2);
                symbol_table[labelNum].value = atoi(temp2);
            }else{
                symbol_table[labelNum].value = -1;
            }

            labelNum++;
        }else {
            trim(line);
            if(line[0]!='.'){
                strcpy(inst_list[i],line);
                i++;
            }
        }
    }
    instNum=i;

    // second pass
    for(i=0;i<labelNum;i++){
        if(!strcmp(symbol_table[i].name,"main")) //label의 name이 main 부터 pc 실행
            break;
    }
    pc=symbol_table[i].index;
    while(pc<instNum){
        strcpy(line,inst_list[pc]);
        int index =find_opcode(opcode_list,strtok(line," "),21);
        opcode findOp = opcode_list[index];
        fputs(findOp.code,fout); // opcode

        strcpy(line,inst_list[pc]);
        char* tmp = strchr(line,' ');
        trim(tmp);

        char* tmp2 = strtok(tmp,",");
        if(tmp2[0]=='$'){ //register 인 경우
            tmp2 = strtok(tmp2,"$");
            fprintf(fout,"%d",atoi(tmp2));
        }
        else{ // label인 경우
            int tmp =find_label(symbol_table,tmp2,labelNum);
            int res = symbol_table[tmp].value;
            if(res==-1)
                res=symbol_table[tmp].index;
            fprintf(fout,"%d",atoi(res));
        }

        strcpy(line,inst_list[pc]);
        tmp = strchr(line,' ');
        trim(tmp);

        tmp2 = strtok(tmp,",");
        //tmp2 = strtok(NULL,",");
        //if(tmp2!=NULL)
            //trim(tmp2);

        printf("%s \n",tmp2);

        if(tmp2[0]=='$'){ //register 인 경우
            tmp2 = strtok(tmp2,"$");
            fprintf(fout,"%d",atoi(tmp2));
        }
        else if(tmp2==NULL || tmp2==""){

            printf("aa");
            fprintf(fout,"%s","000000");
        }
        else{ // label인 경우
            int tmp =find_label(symbol_table,tmp2,labelNum);
            int res = symbol_table[tmp].value;
            if(res==-1)
                res=symbol_table[tmp].index;
            fprintf(fout,"%d",atoi(res));
        }


        strcpy(line,inst_list[pc]);
        tmp = strchr(line,' ');
        trim(tmp);

        tmp2 = strtok(tmp,",");
        //tmp2 = strtok(NULL,",");
        //if(tmp2!=NULL)
            //trim(tmp2);

        printf("%s \n",tmp2);

        if(tmp2[0]=='$'){ //register 인 경우
            tmp2 = strtok(tmp2,"$");
            fprintf(fout,"%d",atoi(tmp2));
        }
        else if(tmp2==NULL || tmp2==""){

            printf("aa");
            fprintf(fout,"%s","000000");
        }
        else{ // label인 경우
            int tmp =find_label(symbol_table,tmp2,labelNum);
            int res = symbol_table[tmp].value;
            if(res==-1)
                res=symbol_table[tmp].index;
            fprintf(fout,"%d",atoi(res));
        }


        pc++;
    }


printf("\n");

    for(i=0;i<instNum;i++){
        //printf("%s",inst_list[i]);
    }

    for(i=0;i<labelNum;i++){
        //printf("%d , %s , %d\n",symbol_table[i].index,symbol_table[i].name, symbol_table[i].value);
    }

    printf("\n");
    fclose(fin);
    fclose(fout);

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
};
int find_opcode(opcode opcode_list[],char *name,int length){
    int i=0;
    for(i=0;i<length;i++){
        if(!strcmp(opcode_list[i].name,name))
            break;
    }

    return i;
}
int find_label(label symbol_table[],char *name,int length){
    int i=0;

    for(i=0;i<length;i++){
        if(!strcmp(symbol_table[i].name,name))
            break;
    }

    return i;
}



