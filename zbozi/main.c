#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VELIKOST 150
#define VSTUP "zbozi.txt"
#define VYSTUP "vystup.html"

typedef struct{
    char dodavatel[5];
    char nazev[50];
    char nazevcl[50];
    int hmotnost;
    int pocet;
    float cena;
}ZBOZI;

void removeunder(ZBOZI *produkt, int i){
    char result[50] = "";
    char *buff;
    char name[50];
    strcpy(name, produkt[i].nazev);
    buff = strtok(name, "_");
    while(buff!=NULL){
        strcat(strcat(result, " "), buff);
        buff = strtok(NULL, "\n_");
    }
    strcpy(produkt[i].nazevcl, result);
}

int main() {
    int pocet = 0;
    char *token;
    char text[VELIKOST];
    FILE *soubor = fopen(VSTUP, "r");
    ZBOZI *produkt = NULL;
    if (soubor == NULL){
        printf("Soubor %s se nepodarilo otevrit", soubor);
    }
    while(fgets(text, VELIKOST, soubor)!=NULL){
        if (pocet > 0){
            int rada = 0;
            produkt = (ZBOZI*)realloc(produkt, pocet*sizeof(ZBOZI));
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
                        produkt[pocet - 1].cena = atoi(token);
                        break;
                }
                token = strtok(NULL, "\n;");
                rada++;
            }
        }
        pocet++;
    }
    if (fclose(soubor)==EOF){
        printf("Soubor %s nebyl spravne zavren", soubor);
    }
    soubor = fopen(VYSTUP, "w");
    if (soubor == NULL){
        printf("Soubor %s nebyl spravne otevren", soubor);
    }
    printf("Dodavatel            Nazev Zbozi     Hmotnost  Kusu      Cena (1ks)     Cena Celkem\n");
    float cenacelkem = 0;
    int vaha = 0;
    float nejdrazsi[2] = {0, 0};
    for (int i = 0; i < pocet - 1; i++){
        removeunder(produkt, i);
        printf("%9s %22s %10d g %5d %11.2f Kc %13.2f Kc\n", produkt[i].dodavatel, produkt[i].nazevcl, produkt[i].hmotnost, produkt[i].pocet, produkt[i].cena, (produkt[i].cena * produkt[i].pocet));
        cenacelkem += (produkt[i].cena * produkt[i].pocet);
        vaha += produkt[i].hmotnost;
        if(nejdrazsi[0] < produkt[i].cena) {
            nejdrazsi[0] = i;
            nejdrazsi[1] = produkt[i].cena;
        }
    }
    printf("Cena celkem: %.2f\n", cenacelkem);
    printf("Pocet polozek: %d\n", pocet);
    printf("Celkova vaha je %dkg a %dg\n", vaha / 1000, vaha % 1000);
    printf("Nejdrazsi produkt je %s za cenu: %.02f\n", produkt[(int)nejdrazsi[0]].nazev, produkt[(int)nejdrazsi[0]].cena);
    fprintf(soubor,"<h1>Zbozi od externich dodavatelu</h1><table><tr><th>dodavatel</th><th>nazev</th><th>hmotnost v gramech</th><th>cena jednoho ks</th></tr>");
    for (int i = 0; i < (pocet - 1); i++){
        if (strcmp(produkt[i].dodavatel, "OP")!= 0){
            fprintf(soubor, "<tr><td>%s</td><td>%s</td><td>%d</td><td>%.2f</td></tr>", produkt[i].dodavatel, produkt[i].nazevcl, produkt[i].hmotnost, produkt[i].cena);
        }
    }
    fprintf(soubor,"</table>");
    if (fclose(soubor)==EOF){
        printf("Nepodarilo se ulozit HTML soubor");
    } else {
        printf("\nHTML soubor by vytvoren");
    }
    return 0;
}
