#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct label{
    char name[10];
    int value;
}label;

typedef struct opcode{
    char name[10];
    char* code;
    char type;
    char* funct;
}opcode;

int find_opcode(opcode opcode_list[],char *name,int length);
int find_label(label symbol_table[],char *name,int length);
int convertDecimal(char* hex);
void first_substring(char str[]);
int* write_binary(int num, int len,FILE* file);
int main(int argc, char* argv[])
{
    int instNum=100;
    char inst_list[instNum][128];

    char file_name[32] = "test02.txt";
    printf("enter the file name :: \n");
    scanf("%s",file_name);

    opcode opcode_list[21] = {
                    {"addiu","001001",'i'},{"addu","000000",'r',"100001"},{"and","000000",'r',"100100"},{"andi","001100",'i'},{"beq","000100",'i'},{"bne","000110",'i'},{"j","000010",'j'},
                    {"jal","000011",'j'},{"jr","000000",'j',"001000"},{"lui","111111",'i'},{"lw","100011",'i'},{"la","111111",'s',"001101"},{"nor","000000",'r',"100111"},{"or","000000",'r',"100101"},
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
        if(strchr(temp,':')==NULL) {
            trim(line);
            if(line[0]!='.'){
                strcpy(inst_list[i],line);
                i++;
            }else if(strstr(temp,".word")!=NULL){
                char* temp2 = strstr(temp,".word");
                temp2 = strchr(temp2,' ');
                trim(temp2);
                strcpy(symbol_table[labelNum].name,"00");

                if(!strncmp(temp2,"0x",2))
                    strcpy(temp2,temp2+2);
                symbol_table[labelNum].value = atoi(temp2);

                labelNum++;
            }
        }else if(strchr(temp,':')!=NULL){ //label이 있는 경우
            symbol_table[labelNum].value=i;
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
                symbol_table[labelNum].value = i;
            }

            labelNum++;
        }
    }

    instNum=i;

    // second pass
    for(i=0;i<labelNum;i++){
        if(!strcmp(symbol_table[i].name,"main")) //label의 name이 main 부터 pc 실행
            break;
    }
    pc=symbol_table[i].value+1;


    while(pc<instNum){
        strcpy(line,inst_list[pc]);
        int index =find_opcode(opcode_list,strtok(line," "),21);
        opcode findOp = opcode_list[index];
        fputs(findOp.code,fout); // opcode

        strcpy(line,inst_list[pc]);
        char* tmp = strchr(line,' ');
        trim(tmp);
        if(findOp.type=='r'){
            char* reg=strtok(tmp,",");

            while(reg!=NULL){
                trim(reg);
                if(strncmp(reg,"0x",2)){
                    first_substring(reg);
                    first_substring(reg);
                    write_binary(convertDecimal(reg),5,fout);
                }else{
                    first_substring(reg);
                    write_binary(atoi(reg),5,fout);
                }
                reg=strtok(NULL,",");
            }

            fprintf(fout,"%s","000000"); //shamt
            fprintf(fout,"%s",findOp.funct); // funct
        }else if(findOp.type=='i'){
            char* reg=strtok(tmp,",");

            for(int i=0;i<2;i++){
                trim(reg);
                if(strncmp(reg,"0x",2)){
                    first_substring(reg);
                    first_substring(reg);
                    write_binary(convertDecimal(reg),5,fout);
                }else{
                    first_substring(reg);
                    write_binary(atoi(reg),5,fout);
                }
                reg=strtok(NULL,",");
            }

            if(reg!=NULL){
                trim(reg);
                if(strncmp(reg,"0x",2)){
                    first_substring(reg);
                    first_substring(reg);
                    write_binary(convertDecimal(reg),16,fout);
                }else{
                    first_substring(reg);
                    write_binary(atoi(reg),16,fout);
                }
            }else{
                write_binary(0,16,fout);
            }
        }else if(findOp.type=='j'){
            char* reg=strtok(tmp," ");
            trim(reg);
            int ind=find_label(symbol_table,reg,labelNum);
            label jlabel=symbol_table[ind];
            write_binary(jlabel.value,16,fout);
        }else if(findOp.type=='s'){
            char* reg1=strtok(tmp,",");
            char* reg2=strtok(NULL,",");
            trim(reg1);
            trim(reg2);

            first_substring(reg1);
            write_binary(atoi(reg1),5,fout);
            write_binary(symbol_table[atoi(reg2)].value,16,fout);

            fprintf(fout,"%s","001101"); //ori

            first_substring(reg1);
            write_binary(atoi(reg1),5,fout);
            write_binary(symbol_table[atoi(reg2)].value,16,fout);

        }

        pc++;
    }


printf("\n");
    printf("============ instruction ==============\n");
    for(i=0;i<instNum;i++){
        printf("%s",inst_list[i]);
    }

    printf("============ symbol table ==============\n");

    for(i=0;i<labelNum;i++){
        printf("%s :: %d\n",symbol_table[i].name, symbol_table[i].value);
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
        if(!strncmp(symbol_table[i].name,name,4))
            break;
    }

    return i;
}
int* write_binary(int num, int len,FILE* file){

    int res[len];
    int i=0;
    for(i=0;i<len;i++)
        res[i]=0;

    int index=len-1;
    while(num>0){
        int remainder=num%2;
        num/=2;
        res[index--]=remainder;
    }

    for(i=0;i<len;i++){
        fprintf(file,"%d",res[i]);
    }

    return res;
}
void first_substring(char str[]) {
    str[0]=str[1];
    str[1]=str[2];
    str[2]=str[3];
    /*
    int i=0;
    printf("%s", str);
    while(str[i-1]!='\0'){
        str[i]=str[i+1];
        i++;
    }
    */
}
int convertDecimal(char* hex){
    int decimal = 0;

    int position = 0;
    for (int i = strlen(hex) - 1; i >= 0; i--)
    {
        char ch = hex[i];
        if (ch >= 48 && ch <= 57)
        {
            decimal += (ch - 48) * pow(16, position);
        }
        else if (ch >= 65 && ch <= 70){
            decimal += (ch - (65 - 10)) * pow(16, position);
        }
        else if (ch >= 97 && ch <= 102)
        {
            decimal += (ch - (97 - 10)) * pow(16, position);
        }

        position++;
    }

    return decimal;

}

