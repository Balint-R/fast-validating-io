#include "../../Input.h"

int main(){
    Input::StandardReader in(stdin);
    int n = in.readInt(0); in.readNl();
    printf("%d\n", n);
    for(int i = 0; i < n; i++){
        int a = in.readInt(-1e9, 1e9);
        i == n-1 ? in.readNl() : in.readSp();
        printf("%d%c", a, " \n"[i == n-1]);
    }
    in.readEOF();
}
