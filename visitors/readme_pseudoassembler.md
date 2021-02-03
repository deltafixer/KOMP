### Pseudoassembler

- program ima stek na kojem imamo dve operacije:
  - `PUSH broj` - stavlja na vrh steka broj
  - `PUSH ID` - stavlja na vrh steka vrednost iz memorijske lokacije na kojoj se čuva promenljiva sa identifikatorom `ID`
  - `POP ID` - izbacuje vrednost sa vrha steka i smešta je u memoriju gde se čuva promenljiva sa identifikatorom `ID`
- binarni operatori su `ADD, SUB, MUL, DIV, AND, OR, XOR` i rade na sledeći način:
  - na vrhu steka je vrednost desnog operanda
  - ispod njega je vrednost levog operanda
  - rade `POP` i smeštaju vrednost sa vrha steka u registar `desniReg`
  - rade `POP` i smeštaju vrednost sa vrha steka u registar `leviReg`
  - izvode binarnu operaciju gde je u `leviReg` levi operand a u `desniReg` desni operand
  - rezultat operacije se smešta na vrh steka, rade `PUSH`
- unarni operatori su `NEG` i `NOT` i rade na sledeći način:
  - na vrhu steka je vrednost operanda
  - rade `POP` i smeštaju vrednost sa vrha steka u registar `leviReg`
  - izvode unarnu operaciju gde je u `leviReg` spešten operand
  - rezultat operacije se smešta na vrh steka, rade `PUSH`
- operacije skakanja na određenu liniju koda:
  - `JMP labela` - bezuslovno ide na liniju sa labelom `labela`
  - `JMPZERO labela` - ukoliko je rezultat poslednje operacije `0` izvršavanje se nastavlja od labele `labela`,  rezultat se uvek skida sa vrha steka, radi se `POP`
- binarni operatori upoređivanja su `CMPLT, CMPLE, CMPGT, CMPGE, CMPEQ` i rade slično kao i binarni operatori, samo:
  - rezultat upoređivanja je `1` ako je upoređivanje tačno, odnosno `0` ako je netačno
  - na primer, `CMPLT` na vrh steka stavlja `1` ako je levi operand strogo manji od desnog operanda, odnosno `0` ako to nije tačno

### Šabloni instrukcija

- generisani kod koji vraća neki rezultat, npr. izrazi, treba da na vrhu steka ostavi konačan rezultat

- binarni operator `lexp OP rexp`
    ```
        kod za lexp
        kod za rexp
        OP
    ```
- unarni operator `OP exp`
    ```
        kod za exp
        OP
    ```
- dodeljivanje `ID = exp`
    ```
        kod za exp
        POP ID
    ```
- konstanta `v`
    ```
        PUSH v
    ```
- petlja `for (preFor; condFor; postFor) stmt`
    ```
        kod za preFor
    FOR_COND:
        kod za condFor
        JMPZERO FOR_END
        kod za stmt
        kod za postFor
        JMP FOR_COND
    FOR_END:
    ```
- identifikator `ID`
    ```
        PUSH ID
    ```
- grananje `if (exp) stmt`
    ```
        kod za exp
        JMPZERO IF_END
        kod za stmt
    IF_END:
    ```
- grananje `if (exp) stmtIF else stmtELSE`
    ```
        kod za exp
        JMPZERO IF_ELSE
        kod za stmtIF
        JMP IF_END
    IF_ELSE:
        kod za stmtELSE
    IF_END:
    ```
- petlja `while (exp) stmt`
    ```
    WHILE_BEGIN:
        kod za exp
        JMPZERO WHILE_END
        kod za stmt
        JMP WHILE_BEGIN
    WHILE_END:
    ```
- naredba `PRINT` - uzima sa vrha steka vrednost i štampa je, radi `POP` te vrednost