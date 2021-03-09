## Světelný řadek
### Marin Kopecký V4
---
### Date: 9.3.2021
---
### Písmo
* formát zápisu znaků
  * ```int A[] = {hodnoty sloupců v big edian}```
* Použit generátor (***[dotmatrixtool.com](http://dotmatrixtool.com/)***)
```cpp
int A[] = {0x00, 0x7f, 0xff, 0xcc, 0xcc, 0xff, 0x7f, 0x00};
int G[] = {0x00, 0x7e, 0xff, 0xc3, 0xcb, 0xef, 0x6e, 0x00};
int E[] = {0x00, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0x00};
int QUE[] = {0x00, 0x00, 0x00, 0xfb, 0xfb, 0x00, 0x00, 0x00};
int B[] = {0x00, 0xff, 0xff, 0x99, 0x99, 0xff, 0x66, 0x00};
int C[] = {0x00, 0x7e, 0xff, 0xc3, 0xc3, 0xe7, 0x66, 0x00};
int O[] = {0x00, 0x7e, 0xff, 0xc3, 0xc3, 0xff, 0x7e, 0x00};
int S[] = {0x00, 0x73, 0xfb, 0x99, 0x99, 0xdf, 0xce, 0x00};
int T[] = {0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xc0, 0x00};
```

### **Registry P2**
* R0 = 0x00 *//resetující hodnota*
* R1 = 0x01 *(1 << 0)*
* R2 = 0x02 *(1 << 1)*
* R3 = 0x04 *(1 << 2)*
* R4 = 0x08 *(1 << 3)*
* R5 = 0x10 *(1 << 4)*
* R6 = 0x20 *(1 << 5)*
* R8 = 0x80 *(1 << 7)* *//sloupcový registr*

### **Sloupce P1**
*  S0 = 0x01 *(1 << 0)*
*  S1 = 0x02 *(1 << 1)*
*  S2 = 0x04 *(1 << 2)*
*  S3 = 0x08 *(1 << 3)*
*  S4 = 0x10 *(1 << 4)*
*  S5 = 0x20 *(1 << 5)*
*  S6 = 0x40 *(1 << 6)*
*  S7 = 0x80 *(1 << 7)*

### **Návod**
1. reset Reg R8 (0xff, P1) -> (0x80, P2)
    ```cpp
    ZapisNaReg(R8, 0);
    Spi(RegidRef);
    UlozHodnotu();
    ```
2. zvolení Reg R1 -> R6 (0x--, P2)
3. zapsání hodnot na Reg (0xff ^ hod, P1)
    ```cpp
    ZapisNaReg(R1, G[i]);
    ZapisNaReg(R2, O[i]);
    ZapisNaReg(R3, O[i]);
    ZapisNaReg(R4, S[i]);
    ZapisNaReg(R5, E[i]);
    ZapisNaReg(R6, QUE[i]);

    void ZapisNaReg(int Reg,int Hodnota){
        outb(Reg, P2);
        outb(NastavHodnotu(Hodnota), P1);
        UlozHodnotu(); //4.
    }
    ```
4. potvrzení Reg (0x00, P2) -> (0xff, P1) *//reset hodnoty*
   ```cpp
   void UlozHodnotu() {
        Spi(Refresh);
        outb(R0, P2);
        Spi(RegidRef);
        outb(Max, P1);
    }
   ```
5. zobrazení sloupce R8 (sloupec, P1) -> (0x80, P2)
    ```cpp
    Spi(Refresh);
    ZapisNaReg(R8, Sloupce[i]);
    ```
6. reset registru R8
    ```cpp
    Spi(RegidRef);
    ZapisNaReg(R8, 0);
    ```
7. Potvrdit hodnoty registrů
    ```cpp
    Spi(RegidRef);
    UlozHodnotu();
    ```
8. GOTO 2 && sloupec++ *//for cyklus*

### **Porty**

```cpp
    #define P1 0x300 /*out*/
    #define P2 0x301 /*out*/
    #define P3 0x300 /*in*/
    #define P4 0x301 /*in*/
````
