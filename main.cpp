#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // zawiera definicje makr o minimalnych i maksymalnych wartosciach typow, w szczegolnosci ULONG_MAX

#define MAX_NUMBER_SIZE 5 // mala liczba 5 dla celow testowych
#define NO_BITS 64 // w typedef struct jest unsigned long :O
#define TYPE unsigned long long
#define NO_OF_DIGITS 20
// liczba to struktura
typedef struct
{
    TYPE cyfry[MAX_NUMBER_SIZE];
    // kolejne cyfry (w rozumieniu cyfr w systemie biliardkowym) w liczbie <dluga_liczba>
    // system jest biliardkowy, bo podstawa systemu to 2^64 = 18446744073709551616
    // tutaj unsigned long jest na 64 bitach
    // biliardek = 2^64
    // w systemie dwojkowym cyfry sa od 0 do 2-1
    // w systemie dziesietnym cyfry sa od 0 do 10-1
    // w systemie szesnastkowym cyfry sa od 0 do 16-1
    // w systemie biliardkowym cyfry sa od 0 do 2^64-1 (biliardek - 1)

    int l_cyfr; // liczba cyfr w liczbie <dluga_liczba>
} dluga_liczba; // dluga_liczba --- nazwa typu

dluga_liczba suma;

bool is_carry(TYPE t1, TYPE t2){
    int t1_r = t1%2, t2_r = t2%2, carry;
    if((t1_r == 1) && (t2_r == 1)){
        carry = 1;
    }else{
        carry = 0;
    }
    t1 = t1>>1;
    t2 = t2>>1;
    TYPE sum = t1+t2+carry;
    return ((sum>>(NO_BITS-1)) == 1);
}

dluga_liczba dodaj_dlugie_liczby(dluga_liczba m, dluga_liczba n, int *przen_na_zewnatrz)
{
    dluga_liczba wynik;
    for(unsigned long long id = 0; id < MAX_NUMBER_SIZE; id++){
        wynik.cyfry[id] = 0;
    }
    bool carry = false;
    for(unsigned long long int idx = 0; idx < MAX_NUMBER_SIZE; idx++){
        if(carry) wynik.cyfry[idx] = m.cyfry[idx] + n.cyfry[idx] + 1;
        else wynik.cyfry[idx] = m.cyfry[idx] + n.cyfry[idx];
        carry = is_carry(m.cyfry[idx],n.cyfry[idx]);
    }
    *przen_na_zewnatrz = carry ? 1 : 0;
    wynik.l_cyfr = 0;
    while(wynik.cyfry[wynik.l_cyfr] != 0 && (wynik.l_cyfr<MAX_NUMBER_SIZE)){
        wynik.l_cyfr++;
    }
    return wynik;
}
bool check_if_not_zeros(char s[]){
    unsigned long int idx = 0;
    while(s[idx]!='\0'){
        if(s[idx] != '0') return 1;
        idx++;
    }
    return 0;
}

bool check_if_last_odd(char s[]){
    // zal: nie ma wartosci nienumerycznych
    unsigned long int idx = 0;
    bool last_odd = false;
    while(s[idx]!='\0'){
        last_odd = (s[idx]%2 == 0);
        idx++;
    }
    return last_odd;
}

dluga_liczba string_2_dluga_liczba(char s[])
{
    // zalozenia s w systemie dziesiętnym
    dluga_liczba wynik;
    for(unsigned long long id = 0; id < MAX_NUMBER_SIZE; id++){
        wynik.cyfry[id] = 0;
    }
    wynik.l_cyfr = 0;
    TYPE temp = 0;
    TYPE mul = 1;
    int offset = 48; // asci '0'
    int temp_div = 0;
    while(check_if_not_zeros(s)){
        temp = 0;
        mul = 1;
        for(long int idx = 0; idx<NO_BITS; idx++ ){
            if(!check_if_last_odd(s)) temp+=mul;
            mul*=2;
            // dzielenie dziesietne przez 2
            long int idx_div = 0;
            while(s[idx_div]!='\0'){
                temp_div += s[idx_div] - offset;
                s[idx_div] = (temp_div/2) + offset;
                temp_div = (temp_div%2)*10;
                idx_div++;
            }
            temp_div = 0;
        }
        wynik.l_cyfr++;
        if(MAX_NUMBER_SIZE<wynik.l_cyfr) return wynik;
        wynik.cyfry[wynik.l_cyfr-1] = temp;
    }
    return wynik;
}

dluga_liczba string_2_dluga_liczba_b(char s[])
{
    // zal: liczba s w systemie biliardkowym tzn 1 zapisywane jako 0...01
    dluga_liczba wynik;
    for(unsigned long long id = 0; id < MAX_NUMBER_SIZE; id++){
        wynik.cyfry[id] = 0;
    }
    wynik.l_cyfr = 0;
    int idx = 0;
    TYPE temp = 0;
    TYPE mul = 1;
    int id = 0;
    while(s[idx] != '\0'){
        while(s[idx] != '\0'){
            idx++;
        }
        if(idx>NO_OF_DIGITS){
            char* end = nullptr;
            wynik.cyfry[id] = std::strtoull(s+idx-NO_OF_DIGITS, &end, 10);
            *(s+idx-NO_OF_DIGITS) = '\0';
            id++;
        }else{
            char* end = nullptr;
            wynik.cyfry[id] = std::strtoull(s, &end, 10);
            id++;
            *(s) = '\0';
        }
        idx=0;
    }
    wynik.l_cyfr = 0;
    while(wynik.cyfry[wynik.l_cyfr] != 0 && (wynik.l_cyfr<MAX_NUMBER_SIZE)){
        wynik.l_cyfr++;
    }
    return wynik;
}

void wypisz_dluga_liczba(dluga_liczba m)
{
    for(long long int idx = m.l_cyfr-1; idx >= 0; idx--){
        printf("%llu ",m.cyfry[idx]);
    }
    printf("\n");
}

int main()
// kazda liczba jest reprezentowana jako MAX_NUMBER_SIZE-elementowa tablica (unsigned-longow)
{
    dluga_liczba m, n;

    // kazda cyfra reprezentowana 64-bitowym longiem daje ok 20 cyfr dziesietnych: 64/(log_2(10))
    // char sm[MAX_NUMBER_SIZE * 20], sn[MAX_NUMBER_SIZE * 20];
    // czytamy liczby z wejscia do stringow sm, sn
    // scanf("%s %s", sm, sn);
    // przykladowy sm[] = "1834674407370955161518446744073709551615184467440737095516151844674407370955161518446744073709551615";
    char sm[] = "1834674407370955161518446744073709551615184467440737095516151844674407370955161518446744073709551615";
    char sn[] = "1834674407370955161518446744073709551615184467440737095516151844674407370955161518446744073709551615";
    // konwertujemy stringi do typy <dluga_liczba>
    m = string_2_dluga_liczba_b(sm);
    n = string_2_dluga_liczba_b(sn);
    wypisz_dluga_liczba(m);
    // opakowanie tablicy w strukure to sposob, aby przekazac tablice w calosci przez wartosc!
    int przeniesienie;
    // konwertujemy stringi do typy <dluga_liczba>
    dluga_liczba suma = dodaj_dlugie_liczby(m, n, &przeniesienie);
    if (przeniesienie != 0)
        printf("BYLO PRZENIESIENIE!\n");

    wypisz_dluga_liczba(suma);

    return 0;
}