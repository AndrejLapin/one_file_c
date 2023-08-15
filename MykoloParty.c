#include <stdlib.h>
               
O = 4, I = 2 << 4;
const l = 32 << 2;
void main()
{
    char b[l] = "PARTYtnheq\"3.1:> dnhas=#edjr !]70n=#bbpqes\"*a(udy!/y Sqfkp\"0r!390!$Fong#oo.#Bbteif.#lfvv btwy\"$#&";
    char* x = b + O;
    while (*(++x))
    {
        *(x) -= (x - b) % O;
    }
    b[29] = 27;
    do
    {
        x = b + 46;
        system(x);
        x = b + 67;
        for(int i = 0; i < I; i++)
        {
            b[I] = 49 + (i % 7);
            b[44] = x[i];
            b[46] = 0;
            system(b + 5);
            b[46] = 's';
        }
    }while(O);
}