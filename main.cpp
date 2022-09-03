#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <algorithm>
using namespace std;

typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

const int AC = 0, WA = 1, PE = 2, IE = 4, PARTIAL = 7;
#define FEEDBACK true

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
    template <typename... Ts> void assertWa(bool expr, Ts... msg){if(!expr) isJudge ? quitf(IE, msg...) : exit(WA);}

    void fillBuffer(){
        if(iEnd == bufSz) buf = (char*) realloc(buf, bufSz <<= 1);
        assertIe(buf, "Bad alloc");
        iEnd += fread(buf + iEnd, 1, bufSz - iEnd, file);
    }

    bool isDelim(char ch){return ch <= ' ';}
    bool isSpace(char ch){return ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r' || ch == ' ';}
    bool isLineSep(char ch){return ch == '\n' || ch == '\r';}
    void incPtr(int am = 1){ii += am;}

    char peekChar(){
        if constexpr(isInteractive){
            while(ii == iEnd){
                if(feof(file)) return -1;
                fillBuffer();
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
        while(isSpace(peekChar())) hasNl |= isLineSep(getChar());
        assertPe(hasNl == nlFlag, "Incorrect line separation");
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

    Input(FILE *inFile, int defaultBufSize = 5 << 20) : file(inFile), bufSz(defaultBufSize), ii(), nlFlag() {
        buf = (char*) malloc(bufSz);
        assertIe(buf, "Bad alloc");
        iEnd = fread(buf, 1, bufSz, file);

        if constexpr(!isInteractive){
            while(!feof(file)) fillBuffer();
            if(iEnd == bufSz){
                buf = (char*) realloc(buf, ++bufSz);
                assertIe(buf, "Bad alloc");
            }
            buf[iEnd] = -1;
        }

        if constexpr(!isIdentical) while(isSpace(peekChar())) incPtr();
    }

    template <typename T = ll, typename U = ull>
    requires(is_integral_v<T> && is_integral_v<U> && is_unsigned_v<U> && sizeof(U) >= sizeof(T))
    T readInt(T minValid = numeric_limits<T>::min(), T maxValid = numeric_limits<T>::max()){
        if constexpr(!isIdentical) eatSpace();
        bool sign = false;
        if constexpr(is_signed_v<T>) sign = peekChar() == '-', incPtr(sign);

        if(peekChar() == '0'){
            assertPe(!sign && isDelim(peekChar()), "Invalid integer");
            assertWa(0 >= minValid && 0 <= maxValid, "Integer starting with 0 out of bounds");
            incPtr();
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
        T res;
        if constexpr(is_signed_v<T>) res = sign ? -x : x; // safe since C++20
        else res = x;
        assertWa(res >= minValid && res <= maxValid, "Integer out of bounds");
        return res;
    }
    template <typename T, typename U> requires (!is_same_v<T, U> || is_floating_point_v<T>)
    ll readInt(T minValid, U maxValid){return readInt<ll>((ll) minValid, (ll) maxValid);}

    static constexpr int MDGT = 20;
    static constexpr ull PWS[MDGT] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, (ull) 1e8, (ull) 1e9, (ull) 1e10,
            (ull) 1e11, (ull) 1e12, (ull) 1e13, (ull) 1e14, (ull) 1e15, (ull) 1e16, (ull) 1e17, (ull) 1e18, (ull) 1e19};
    template <typename T = double, typename U = ull>
    requires (is_floating_point_v<T> && is_integral_v<U> && is_unsigned_v<U> && numeric_limits<U>::digits10 < MDGT)
    T readFloat(int reqPrec = -1){
        assertIe(reqPrec <= numeric_limits<U>::digits10, "Required precision is more than can be parsed");
        if constexpr(!isIdentical) eatSpace();
        bool sign = peekChar() == '-'; incPtr(sign);

        T res;
        if(peekChar() == '0') res = 0;
        else {
            assertPe(peekChar() >= '0' && peekChar() <= '9', "Invalid float");
            U x = getChar()-'0';

            while(peekChar() >= '0' && peekChar() <= '9'){
                if(x >= numeric_limits<U>::max()/10){
                    res = x;
                    while(peekChar() >= '0' && peekChar() <= '9') res = res*10 + (getChar()-'0');
                    goto finishedParsingWhole; // skips res = x
                }
                x = x*10 + getChar()-'0';
            }

            res = x;
            finishedParsingWhole:;
        }

        if(peekChar() == '.'){
            incPtr();
            assertPe(peekChar() >= '0' && peekChar() <= '9', "Invalid float");
            U y = getChar()-'0';
            int prec = 1;
            while(peekChar() >= '0' && peekChar() <= '9'){
                if(prec < numeric_limits<U>::digits10) y = y*10 + peekChar()-'0', prec++;
                incPtr();
            }
            assertPe(reqPrec < 0 || reqPrec == prec, "Float must have exactly %d digits after the decimal", reqPrec);
            res += (T) y/PWS[prec];
        }
        else assertPe(reqPrec <= 0, "Float must have exactly %d digits after the decimal", reqPrec);

        assertPe(isDelim(peekChar()), "Invalid float");
        return sign ? -res : res;
    }

    string readStr(){
        if constexpr(!isIdentical) eatSpace();
        int beg = ii;
        while(!isDelim(peekChar())) incPtr();
        return {buf + beg, ii - beg};
    }

    string readLine(){
        if constexpr(!isIdentical) eatSpace();
        int beg = ii;
        while(peekChar() >= ' ') incPtr();
        readNl();
        return {buf + beg, ii - beg};
    }

    template <typename T = ll> requires is_integral_v<T>
    T readSingleInt(T minValid = numeric_limits<T>::min(), T maxValid = numeric_limits<T>::max()){
        T x = readInt(minValid, maxValid);
        readNl();
        return x;
    }
    template <typename T, typename U> requires (!is_same_v<T, U> || is_floating_point_v<T>)
    ll readSingleInt(T minValid, U maxValid){return readSingleInt<ll>(minValid, maxValid);}

    template <typename T = ll> requires is_integral_v<T>
    pair<T, T> readIntPair(T minValid = numeric_limits<T>::min(), T maxValid = numeric_limits<T>::max()){
        T x = readInt(minValid, maxValid); readSp();
        T y = readInt(minValid, maxValid); readNl();
        return {x, y};
    }
    template <typename T, typename U> requires (!is_same_v<T, U> || is_floating_point_v<T>)
    pair<ll, ll> readIntPair(T minValid, U maxValid){return readIntPair<ll>(minValid, maxValid);}

    template <int sz, typename T = ll> requires (sz > 0 && is_integral_v<T>)
    array<T, sz> readIntTuple(T minValid = numeric_limits<T>::min(), T maxValid = numeric_limits<T>::max()){
        array<T, sz> res{readInt<T>(minValid, maxValid)};
        for(int i = 1; i < sz; i++) readSp(), res[i] = readInt<T>(minValid, maxValid);
        readNl();
        return res;
    }
    template <int sz, typename T, typename U> requires (sz > 0 && (!is_same_v<T, U> || is_floating_point_v<T>))
    array<ll, sz> readIntTuple(T minValid, U maxValid){return readIntTuple<sz, ll>(minValid, maxValid);}

    template <typename T, typename U, typename V> requires is_integral_v<T>
    void readIntArray(T *ptr, int sz, U minValid = numeric_limits<T>::min(), V maxValid = numeric_limits<T>::max()){
        assertIe(sz > 0, "Non-positive array size");
        ptr[0] = readInt<T>(minValid, maxValid);
        for(int i = 1; i < sz; i++) readSp(), ptr[i] = readInt<T>(minValid, maxValid);
        readNl();
    }
};

int arr[100];

int main(){
    //return !printf("%hhd\n%hhd\n%hhd\n%hhd\n%hhd\n%hhd\n", '\t', '\n', '\v', '\f', '\r', ' ');

    Input<false, false, false> inp(stdin);
    int n = inp.readInt(); inp.readNl();
    int x = inp.readInt<__int128, unsigned __int128>(1, 30); inp.readNl();

    inp.readSingleInt(1, 1000LL);
    inp.readIntArray(arr, 3, -1e2, 1e2);
    for(int i = 0; i < 3; i++) printf("%d%c", arr[i], " \n"[i == n-1]);
    inp.readEOF();
}

// reviewed whole thing
