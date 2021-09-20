#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define VSTUP "knihy.txt"
#define VYSTUP "vysledek.html"
#define VELIKOST 150


typedef struct {
    int id;
    char jmeno[50];
    char nosic[50];
    int rok;
    int vaha;
    char isbn[50];
    char isbnint[50]
}KNIHA;

void celkovavaha (int vaha, int zbytek){
    static int cvaha = 0;
    cvaha += vaha;
    if (!zbytek){
        printf("Celkova vaha je %dkg a %dg\n", cvaha / 1000, cvaha % 1000);
    }
}

void novakniha(KNIHA kniha, int zbytek){
    static char nazev[50];
    static int rok = 0;
    if (kniha.rok > rok){
        rok = kniha.rok;
        strcpy(nazev, kniha.jmeno);
    }
    if(!zbytek){
        printf("Nejnovejsi kniha je %s, napsana roku %d\n", nazev, rok);
    }
}

void knihanebodvd (KNIHA kniha, int pocet, int i){
    static int knih = 0;
    static int dvd = 0;
    if (strcmp(kniha.nosic, "DVD")!=0){
        knih++;
        celkovavaha(kniha.vaha, (pocet - (knih + dvd)));
        novakniha(kniha, (pocet - (knih + dvd)));
    } else {
        dvd++;
    }
    if ((pocet-1)==(knih+dvd)){
        printf("Knih je %d\nDVD je %d\n", knih, dvd);
    }
}

int main() {
    char text[VELIKOST];
    KNIHA *knihy = NULL;
    int pocet = 0;
    char *token;
    int rada;
    FILE *soubor = fopen(VSTUP, "r");
    if (soubor == NULL){
        printf("Nastala chyba pri otevirani souboru %s", VSTUP);
        return EXIT_FAILURE;
    }
    while(fgets(text, VELIKOST, soubor)!=NULL){
        if (pocet > 0){
            rada = 0;
            knihy=(KNIHA*)realloc(knihy, pocet*sizeof(KNIHA));
            token = strtok(text, "\n;");
            while(token!=NULL) {
                switch (rada) {
                    case 0:
                        knihy[pocet - 1].id = atoi(token);
                        break;
                    case 1:
                        strcpy(knihy[pocet - 1].jmeno, token);
                        break;
                    case 2:
                        strcpy(knihy[pocet - 1].nosic, token);
                        break;
                    case 3:
                        knihy[pocet - 1].rok = atoi(token);
                        break;
                    case 4:
                        knihy[pocet - 1].vaha = atoi(token);
                        break;
                    case 5:
                        strcpy(knihy[pocet - 1].isbn, token);
                        break;
                }
                rada++;
                token = strtok(NULL, "\n;");
            }
        }
        pocet++;
    }
    if (fclose(soubor)==EOF){
        printf("Soubor %s se nopodarilo uzavrit", VSTUP);
    }
    soubor = fopen(VYSTUP, "w");
    if (soubor == NULL){
        printf("Soubor %s se nepodarilo otevrit", VYSTUP);
    }
    printf("Celkovy pocet zaznamu je %d\n", pocet-1);
    for (int i = 0; i < pocet; i++){
        char isbnint[20] = "";
        char *tokentwo;
        if (strlen(knihy[i].isbn) > 5){
            tokentwo = strtok(knihy[i].isbn, "-");
            while(tokentwo!=NULL) {
                strcat(isbnint, tokentwo);
                tokentwo = strtok(NULL, "\n-");
            }
        }
        strcpy(knihy[i].isbnint, isbnint);
        knihanebodvd(knihy[i], pocet, i);
    }
    fprintf(soubor, "<h1>Seznam knih s ISBN-10</h1><table><tr><th>cislo</th><th>nazev</th><th>nosic</th><th>rok vydani</th><th>hmotnost</th><th>ISBN</th></tr>");
    for (int i = 0; i < (pocet-1); i++){
        if (strcmp(knihy[i].nosic, "DVD")!=0 && knihy[i].rok < 2007) {
            fprintf(soubor, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>", knihy[i].id, knihy[i].jmeno, knihy[i].nosic, knihy[i].rok, knihy[i].vaha, knihy[i].isbnint);
        }
    }
    fprintf(soubor,"</table>");
    fprintf(soubor, "<h1>Seznam knih s ISBN-13</h1><table><tr><th>cislo</th><th>nazev</th><th>nosic</th><th>rok vydani</th><th>hmotnost</th><th>ISBN</th></tr>");
    for (int i = 0; i < (pocet - 1); i++){
        if (strcmp(knihy[i].nosic, "DVD")!=0 && knihy[i].rok >= 2007) {
            fprintf(soubor, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>", knihy[i].id, knihy[i].jmeno, knihy[i].nosic, knihy[i].rok, knihy[i].vaha, knihy[i].isbnint);
        }
    }
    fprintf(soubor, "</table>");
    return 0;
}