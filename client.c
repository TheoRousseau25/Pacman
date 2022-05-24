#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#define maxY 10
#define maxX 6
#define PORT 6000
#define MAX_BUFFER 1000

const char *EXIT = "exit";


void lireMessage(char tampon[]) {
    printf("Choisir une direction : haut (z), bas (s), droite (d) ou gauche (q) \n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}

void afficherTableau(int t[][maxX]){

    int i,j, tab[maxY][maxX];

    for(i=0; i < maxY; i++)
    {
        for(j=0; j < maxX; j++)
        {
            if(t[i][j]==1){
                printf("| C |");
            }
            else if(t[i][j]==2){
                printf("| * |");
            }
            else if(t[i][j]==3) {
                printf("| X |");
            }
            else if(t[i][j]==4) {
                printf("| X |");
            }
            else{
                printf("|   |" );
            }
        }
        printf("\n");
    }
}

int verifierPerdu(int t[][maxX]){
    
    int i,j;
    int booleen = 0;
    
    for(i=0; i < maxY; i++)
    {
        for(j=0; j < maxX; j++)
        {
            if(t[i][j]==1){
                booleen = 1;
            }
        }
    }
    return booleen;     
}

int verifierPoints(int t[][maxX]){
    
    int i,j;
    int booleen = 0;
    
    for(i=0; i < maxY; i++)
    {
        for(j=0; j < maxX; j++)
        {
            if(t[i][j]==2){
                booleen = 1;
            }
        }
    }
    return booleen;     
}

int main(int argc , char const *argv[]) {
    int fdSocket;
    int nbRecu;
    struct sockaddr_in coordonneesServeur;
    int longueurAdresse;
    char tampon[MAX_BUFFER];
    int t[maxY][maxX];
    int grille[maxY][maxX];
    int score =0;
    char bufScore[10] = "";

    fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (connect(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("connexion ok\n");

    while (1) {

        recv(fdSocket, t, sizeof(t), 0);
        recv(fdSocket, bufScore, sizeof(bufScore), 0);
        sscanf(bufScore, "%d", &score);
        afficherTableau(t);
        printf("Score : %d\n", score);
        
        lireMessage(tampon);
        
        if(verifierPoints(t)==0 || verifierPerdu(t)==0){
            
            if(verifierPoints(t)==0){
                printf("Bravo, vous avez gagné !\n"); 
            } else {
                printf("Désolé, vous avez perdu !\n");
            }
            printf("Voulez-vous continuer à jouer ('o' pour oui, 'n' pour non) ?\n");
            fgets(tampon, MAX_BUFFER, stdin);
            strtok(tampon, "\n");
            
            send(fdSocket, tampon, strlen(tampon), 0);
             
            recv(fdSocket, t, sizeof(t), 0);
            recv(fdSocket, bufScore, sizeof(bufScore), 0);
            sscanf(bufScore, "%d", &score);
            afficherTableau(t);
            printf("Score : %d\n", score);
            
            if(strcmp(tampon,"n")==0){
                break;
            }
        }

        if (testQuitter(tampon)) {
            send(fdSocket, tampon, strlen(tampon), 0);
            break; // on quitte la boucle
        }

        // on envoie le message au serveur
        send(fdSocket, tampon, strlen(tampon), 0);

        /*// on attend la réponse du serveur
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);*/


    }

    close(fdSocket);

    return EXIT_SUCCESS;
}
