201321040 조원민
assembly 만들기

main.c 실행하면 scanf()로 .txt파일 주소를 입력받습니다.
result.txt로 binary 파일이 나옵니다.

함수설명
int find_opcode(opcode[] ,char* name, int len) :: opcode리스트에서 name(ex: add,addi)의opcode keyword를 인자로 받아서 index를 반환합니다.

int find_label(label[] ,char* name,int len) :: symbol table에서 해당하는 label의 index를 반환합니다.

int convertDecimal(char* ) :: string 16진수를 10진수로 변환합니다.

void first_substring(char* ) :: string의 첫 글자를 제거합니다.

int* write_binary(int num,int len,FILE* file) :: 파일에 num을 len의 비트만큼 2진수로 출력합니다. 해당하는 2진수 배열을 반환합니다.

