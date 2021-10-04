#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VSTUP "skoky.txt"
#define VYSTUP "vystup.html"
#define VELIKOST 150

typedef struct {
    char prijmeni[50];
    char jmeno[50];
    char datum[20];
    char stat[5];
    int delka;
    char tym[5];
} SKOK;

void printdata(SKOK*skokan, int pocet){
    int vtymech = 0;
    int nejskok[2] = {0,0};
    int celkemdelka = 0;
    for (int i = 0; i < pocet; i++){
        if (strcmp(skokan[i].tym, "NE")==0){
            vtymech++;
        } else {
            celkemdelka += skokan[i].delka;
        }
        if (skokan[i].delka > nejskok[0]){
            nejskok[0] = skokan[i].delka;
            nejskok[1] = i;
        }
    }
    printf("Pocet zavodniku v tymech %d\n", vtymech);
    printf("Nejdelsi skok udelal %s %s z %s a to %d metru\n", skokan[nejskok[1]].jmeno , skokan[nejskok[1]].prijmeni , skokan[nejskok[1]].stat , skokan[nejskok[1]].delka);
    printf("Prumnerny skok byl %.2f metru\n", ((float)celkemdelka / (float)(pocet - vtymech)));
}

void converttime(SKOK *skokan, int pocet, char datumdb[pocet][20]) {
    int year;
    int month;
    int day;
    char *token;
    char datum[150] = "";
    for (int i = 0; i < pocet; i++){
        int type = 0;
        token = strtok(skokan[i].datum, ".");
        while(token != NULL){
            switch(type){
                case 0:
                    day = atoi(token);
                    break;
                case 1:
                    month = atoi(token);
                    break;
                case 2:
                    year = atoi(token);
                    break;
            }
            type++;
            token = strtok(NULL, "\n.");
        }
        sprintf(datum, "%04d-%02d-%02d", year, month, day);
        strcpy(datumdb[i], datum);
    }
}

int main() {
    FILE *soubor = fopen(VSTUP, "r");
    if (soubor==NULL){
        printf("Soubor neotevren");
    }
    char text[VELIKOST];
    SKOK *skokan = NULL;
    char *token;
    int pocet = 0;
    while(fgets(text, VELIKOST, soubor)){
        if(pocet > 0){
            int rada = 0;
            skokan = (SKOK*)realloc(skokan, pocet*sizeof(SKOK));
            token = strtok(text, ";");
            while (token != NULL){
                switch (rada) {
                    case 0:
                        strcpy(skokan[pocet-1].prijmeni,token);
                        break;
                    case 1:
                        strcpy(skokan[pocet-1].jmeno,token);
                        break;
                    case 2:
                        strcpy(skokan[pocet-1].datum,token);
                        break;
                    case 3:
                        strcpy(skokan[pocet-1].stat,token);
                        break;
                    case 4:
                        skokan[pocet-1].delka = atoi(token);
                        break;
                    case 5:
                        strcpy(skokan[pocet-1].tym,token);
                        break;
                }
                token = strtok(NULL, "\n;");
                rada++;
            }
        }
        pocet++;
    }
    pocet--;
    printf("\nPocet zavodniku je %d\n", pocet);
    if (fclose(soubor)==EOF){
        printf("\nSoubor %s nebyl spravne uzavren/n", VSTUP);
    }
    soubor = fopen(VYSTUP, "w");
    if (soubor==NULL){
        printf("\nSoubor %s nebyl otevren/n", VYSTUP);
    }
    fprintf(soubor,"<h1>Tym ceske republiky</h1><table><tr><th>poradi</th><th>jmeno</th><th>prijmeni</th><th>narozen</th><th>skok</th></tr>");
    printdata(skokan, pocet);
    int poradi = 1;
    char datumdb[pocet][20];
    converttime(skokan, pocet, datumdb);
    for (int i = 0; i < pocet; i++){
        if (strcmp("ANO",skokan[i].tym)==0 && strcmp("CZE",skokan[i].stat)==0){
            fprintf(soubor, "<tr><td>%d.</td><td>%s</td><td>%s</td><td>%s</td><td>%d m</td></tr>", poradi, skokan[i].jmeno, skokan[i].prijmeni, datumdb[i], skokan[i].delka);
            poradi++;
        }
    }
    fprintf(soubor, "</table>");
    if (fclose(soubor)==EOF){
        printf("\nSoubor %s nebyl ulozen\n", VYSTUP);
    } else {
        printf("\nSoubor %s byl uspesne vytvoren\n", VYSTUP);
        return EXIT_SUCCESS;
    }
    return 0;
}
