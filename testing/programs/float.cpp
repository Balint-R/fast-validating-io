#include "../../Input.h"

int main(){
    Input::IdenticalReader in(stdin);
    int n = in.readInt(); in.readNl();
    for(int i = 0; i < n; i++){
        double a = in.readFloat(); in.readNl();
        printf("%.25f\n", a);
    }
    in.readEOF();
}
