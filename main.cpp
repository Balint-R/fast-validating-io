#include <stdio.h>
#include <stdlib.h>
#include <numeric>
#include <string>
#include <type_traits>
using namespace std;

typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

const int AC = 0, WA = 1, PE = 2, IE = 4, PARTIAL = 7;
#define FEEDBACK false

template <typename... Ts> void quitf(int code, Ts... msg){
    if(code == IE || FEEDBACK) printf(msg...);
    exit(code);
}

template <typename... Ts> void assertPe(bool expr, Ts... msg){if(!expr) quitf(PE, msg...);}
template <typename... Ts> void assertWa(bool expr, Ts... msg){if(!expr) quitf(WA, msg...);}
template <typename... Ts> void assertIe(bool expr, Ts... msg){if(!expr) quitf(IE, msg...);}

template <bool isJudge, bool isIdentical, bool isInteractive>
struct Input {
    FILE *file;
    char *buf;
    int ii, iEnd, bufSz;
    bool nlFlag; // whether next whitespace group should have nl

    template <typename... Ts> void assertPe(bool expr, Ts... msg){if(!expr) quitf(isJudge ? IE : PE, msg...);}
    template <typename... Ts> void assertWa(bool expr, Ts... msg){if(!expr) quitf(isJudge ? IE : WA, msg...);}

    bool isDelim(char ch){return ch <= ' ';}
    bool isSpace(char ch){return ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r' || ch == ' ';}
    bool isLineSep(char ch){return ch == '\n' || ch == '\r' || ch == -1;}
    void incPtr(int am = 1){ii += am;}

    char peekChar(){
        if constexpr(isInteractive){
            if(feof(file)) return -1;
            if(ii == iEnd){
                if(iEnd == bufSz){
                    buf = (char*) realloc(buf, bufSz <<= 1);
                    assertIe(buf, "Bad alloc");
                }
                buf[iEnd++] = getc(file);
            }
        }
        return buf[ii];
    }

    char getChar(){
        char ch = peekChar();
        incPtr();
        return ch;
    }

    // called for non-identical only
    void eatSpace(){
        assertPe(isSpace(peekChar()) || !isDelim(peekChar()), "Less output than expected or invalid character");
        bool hasNl = false;
        bool atStart = ii == 0;
        while(isSpace(peekChar())) hasNl |= isLineSep(getChar());
        assertPe(atStart || hasNl == nlFlag, "Incorrect line separation");
        nlFlag = false;
    }

    void readEOF(){
        if constexpr(!isIdentical) while(isSpace(peekChar())) incPtr();
        assertPe(peekChar() == -1, "More output than expected");
    }

    void readSp(){
        if constexpr(isIdentical) assertPe(getChar() == ' ', "Whitespace error");
    }

    void readNl(){
        if constexpr(isIdentical) assertPe(getChar() == '\n', "Whitespace error");
        else nlFlag = true;
    }

    Input(FILE *inFile, int defaultBufSize = 5 << 20) : file(inFile), ii(), bufSz(defaultBufSize), nlFlag() {
        buf = (char*) malloc(bufSz);
        assertIe(buf, "Bad alloc");
        if constexpr(!isInteractive){
            while(true){
                iEnd += fread(buf+iEnd, 1, bufSz-iEnd-1, file);
                if(feof(file)) break;
                buf = (char*) realloc(buf, bufSz <<= 1);
                assertIe(buf, "Bad alloc");
            }
            buf[iEnd] = -1;
        }
    }

    template <typename T = ll, typename U = ull> requires(is_integral_v<T> && is_unsigned_v<U> && sizeof(U) >= sizeof(T))
    T readInt(T minValid = numeric_limits<T>::min(), T maxValid = numeric_limits<T>::max()){
        if constexpr(!isIdentical) eatSpace();
        bool sign = false;
        if constexpr(is_signed_v<T>) sign = peekChar() == '-', incPtr(sign);

        if(peekChar() == '0'){
            incPtr();
            assertPe(!sign && isDelim(peekChar()), "Invalid integer");
            assertWa(0 >= minValid && 0 <= maxValid, "Integer out of bounds");
            return 0;
        }

        assertPe(peekChar() >= '0' && peekChar() <= '9', "Invalid integer");
        U x = getChar()-'0';
        while(!isDelim(peekChar())){
            assertPe(peekChar() >= '0' && peekChar() <= '9', "Invalid integer");
            U newX = x*10 + getChar()-'0';
            assertPe(x <= numeric_limits<U>::max()/10 && x < newX, "Integer overflow");
            x = newX;
        }

        if constexpr(!is_same_v<T, U>) assertPe(x <= (U) numeric_limits<T>::max() + sign, "Integer overflow");
        T res = is_signed_v<T> ? (sign ? -x : x) : x; // safe since C++20
        assertWa(res >= minValid && res <= maxValid, "Integer out of bounds");
        return res;
    }
    template <typename T, typename U> requires (!is_same_v<T, U> || is_floating_point_v<T>)
    ll readInt(T minValid, U maxValid){return readInt<ll>((ll) minValid, (ll) maxValid);}

    template <typename T = double> requires (is_floating_point_v<T>)
    T readFloat(int reqPrec = -1){
        if constexpr(!isIdentical) eatSpace();
        bool sign = peekChar() == '-'; incPtr(sign);

        T res;
        if(peekChar() == '0') res = 0;
        else {
            assertPe(peekChar() >= '0' && peekChar() <= '9', "Invalid float");
            res = getChar()-'0';
            while(peekChar() >= '0' && peekChar() <= '9') res = res*10 + (getChar()-'0');
        }

        int prec = 0;
        if(peekChar() == '.'){
            incPtr();
            for(T pw = (T) 1/10; peekChar() >= '0' && peekChar() <= '9'; pw /= 10, prec++) res += (getChar()-'0')*pw;
            assertPe(prec, "Invalid float");
        }
        assertPe(reqPrec < 0 || reqPrec == prec, "Float must have exactly %d digits after the decimal", reqPrec);

        assertPe(isDelim(peekChar()), "Invalid float");
        return sign ? -res : res;
    }

    string readStr(){
        if constexpr(!isIdentical) eatSpace();
        int beg = ii;
        while(!isDelim(peekChar())) incPtr();
        return {buf + beg, (uint) (ii - beg)};
    }

    string readLine(){
        if constexpr(!isIdentical) eatSpace();
        int beg = ii;
        while(!isLineSep(peekChar())) incPtr();
        uint len = ii - beg;
        readNl();
        return {buf + beg, len};
    }
};

using Validator = Input<true, true, false>;
using IdenticalReader = Input<false, true, false>;
using StandardReader = Input<false, false, false>;
using IdenticalInteractor = Input<false, true, true>;
using StandardInteractor = Input<false, false, true>;

int main(int argc, char **argv){
    assertIe(argc > 3, "Not enough arguments");
    Validator judgeIn(fopen(argv[1], "r"));
    IdenticalReader userOut(fopen(argv[2], "r"));
    Validator judgeOut(fopen(argv[3], "r"));
    
    
}
