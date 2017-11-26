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




symbol table은 label or date name | value 형식으로 이루어졌습니다.
label(ex: main ,loop)은value에 명령어의 index가 들어갑니다. 
data(ex: data1,data2)은 value에 data의 값이 들어갑니다. array 같은 경우 다음 라인에는 label name이 없기때문에 임의 '00'으로 입력 받았습니다.

