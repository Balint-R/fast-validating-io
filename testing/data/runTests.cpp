#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <tuple>
#include <set>
using namespace std;

const int IO_SZ = 1 << 16;
char buf1[IO_SZ], buf2[IO_SZ];

const string DATA_DIR = "data";
const string PROG_DIR = "programs";
const string TMP_FILE = "tmp.x";

vector<pair<string, vector<pair<string, string>>>> TESTS = {
      {"float.cpp", {
            {"float1.in", "float1.out"},
            {"float2.in", "float2.out"}
      }},
      {"int_idn.cpp", {
            {"int1.in", "int1.out"},
            {"int2.in", "int2.out"},
            {"int3.in", "int3.out"},
            {"int4.in", "int4.out"},
            {"int5.in", "int5.out"},
            {"int6.in", "int6.out"},
            {"int7.in", "int7_idn.out"},
            {"int8.in", "int8_idn.out"},
            {"int9.in", "int9_idn.out"}
      }},
      {"int_std.cpp", {
            {"int1.in", "int1.out"},
            {"int2.in", "int2.out"},
            {"int3.in", "int3.out"},
            {"int4.in", "int4.out"},
            {"int5.in", "int5.out"},
            {"int6.in", "int6.out"},
            {"int7.in", "int7_std.out"},
            {"int8.in", "int8_std.out"},
            {"int9.in", "int9_std.out"}
      }}
};

int main(){
    for(auto [prog, cases] : TESTS){
        system(("g++ " + PROG_DIR + "/" + prog + " -std=c++20 -o " + TMP_FILE).c_str());

        for(auto [in, out] : cases){
            printf("Case: %s %s %s: ", prog.c_str(), in.c_str(), out.c_str());
            fflush(stdout);

            FILE *proc = popen(("./" + TMP_FILE + " <" + DATA_DIR + "/" + in).c_str(), "r");
            int nb1 = fread(buf1, 1, IO_SZ, proc);
            int exitCode = WEXITSTATUS(pclose(proc));

            FILE *ref = fopen((DATA_DIR + "/" + out).c_str(), "r");
            int nb2 = fread(buf2, 1, IO_SZ, ref);
            fclose(ref);

            // String representation of expected exit code
            string reqStr = {buf2[nb2-3], buf2[nb2-2]};
            nb2 -= 3;
            int reqCode;
            if(reqStr == "AC") reqCode = 0;
            else if(reqStr == "WA") reqCode = 1;
            else if(reqStr == "PE") reqCode = 2;
            else {
                printf("Unknown required status '%s'\n", reqStr.c_str());
                continue;
            }

            if(exitCode != reqCode){
                printf("Got exit code %d but expected %d\n", exitCode, reqCode);
                continue;
            }

            if(nb1 != nb2 || memcmp(buf1, buf2, nb1)){
                buf1[nb1] = buf2[nb2] = 0;
                printf("Output doesn't match.\nActual:\n```\n%s```\nExpected:\n```\n%s```\n", buf1, buf2);
                continue;
            }

            printf("ok\n");
        }
    }
}
