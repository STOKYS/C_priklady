#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VELIKOST 150
#define VSTUP "zbozi.txt"
#define VYSTUP "vypis_zbozi.html"

typedef struct{
    char dodavatel[5];
    char nazev[50];
    int hmotnost;
    int pocet;
    float cena;
}ZBOZI;

void removeunder(ZBOZI *produkt,int pocet, char nazevcl[pocet][50]){
    char *buff;
    char name[50];
    for (int i = 0; i < pocet; i++){
        strcpy(nazevcl[i], "");
        strcpy(name, produkt[i].nazev);
        buff = strtok(name, "_");
        while(buff!=NULL){
            strcat(strcat(nazevcl[i], " "), buff);
            buff = strtok(NULL, "\n_");
        }
    }
}

int main() {
    int pocet = 0;
    char *token;
    char text[VELIKOST];
    FILE *soubor = fopen(VSTUP, "r");
    ZBOZI *produkt = NULL;
    if (soubor == NULL) {
        printf("Soubor %s se nepodarilo otevrit", soubor);
    }
    while (fgets(text, VELIKOST, soubor) != NULL) {
        if (pocet > 0) {
            int rada = 0;
            produkt = (ZBOZI *) realloc(produkt, pocet * sizeof(ZBOZI));
            token = strtok(text, "\n;");
            while (token != NULL) {
                switch (rada) {
                    case 0:
                        strcpy(produkt[pocet - 1].dodavatel, token);
                        break;
                    case 1:
                        strcpy(produkt[pocet - 1].nazev, token);
                        break;
                    case 2:
                        produkt[pocet - 1].hmotnost = atoi(token);
                        break;
                    case 3:
                        produkt[pocet - 1].pocet = atoi(token);
                        break;
                    case 4:
                        produkt[pocet - 1].cena = atof(token);
                        break;
                }
                token = strtok(NULL, "\n;");
                rada++;
            }
        }
        pocet++;
    }
    pocet--;
    if (fclose(soubor) == EOF) {
        printf("Soubor %s nebyl spravne zavren", soubor);
    }
    soubor = fopen(VYSTUP, "w");
    if (soubor == NULL) {
        printf("Soubor %s nebyl spravne otevren", soubor);
    }
    printf("U C T E N K A\nDodavatel            Nazev Zbozi     Hmotnost  Kusu      Cena (1ks)     Cena Celkem\n");
    float cenacelkem = 0;
    int vaha = 0;
    float nejdrazsi[2] = {0, 0};
    char nazevcl[pocet][50];
    removeunder(produkt, pocet, nazevcl);
    for (int i = 0; i < pocet; i++) {
        printf("%9s %22s %10d g %5d %11.2f Kc %13.2f Kc\n", produkt[i].dodavatel, nazevcl[i],
               produkt[i].hmotnost, produkt[i].pocet, produkt[i].cena, (produkt[i].cena * produkt[i].pocet));
        cenacelkem += (produkt[i].cena * produkt[i].pocet);
        vaha += produkt[i].hmotnost * produkt[i].pocet;
        if (nejdrazsi[1] < produkt[i].cena) {
            nejdrazsi[0] = i;
            nejdrazsi[1] = produkt[i].cena;
        }
    }
    printf("Cena celkem: %.2f Kc.\n", cenacelkem);
    printf("Pocet polozek: %d.\n", pocet);
    printf("Celkova vaha je %d kg a %d g.\n", vaha / 1000, vaha % 1000);
    printf("Nejdrazsi produkt je %s za cenu: %.02f Kc.\n", nazevcl[(int)nejdrazsi[0]],
           produkt[(int) nejdrazsi[0]].cena);
    fprintf(soubor,
            "<h1>Zbozi od externich dodavatelu</h1><table><tr><th>dodavatel</th><th>nazev</th><th>hmotnost v gramech</th><th>cena jednoho ks</th></tr>");
    for (int i = 0; i < pocet; i++) {
        if (strcmp(produkt[i].dodavatel, "OP") != 0) {
            fprintf(soubor, "<tr><td>%s</td><td>%s</td><td>%d</td><td>%.2f</td></tr>", produkt[i].dodavatel,
                    nazevcl[i], produkt[i].hmotnost, produkt[i].cena);
        }
    }
    fprintf(soubor, "</table>");
    if (fclose(soubor) == EOF) {
        printf("Nepodarilo se ulozit HTML soubor");
        return EXIT_FAILURE;
    } else {
        printf("\nByl vytvoren soubor %s.", VYSTUP);
        return EXIT_SUCCESS;
    }
}