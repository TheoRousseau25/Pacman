#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#define maxY 10
#define maxX 6
#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_CLIENTS 3

const char *EXIT = "exit";

//Fonction qui retourne un nombre au hasard.
int my_rand (void)
{
    return (rand ()%4 + 1);
}

//Sert à vérifier si une case contient une étoile ou non.
int verifierCase(int x, int y,  int t[][maxX]){

    int i,j, tab[maxY][maxX];

    if(t[y][x]==2){
        return 1;
    }
    
    return 0;

}

void initialiserTableau(int t[][maxX]){
    
    int i,j;
    srand(time(0));

    for(i=0; i < maxY; i++)
        {
            for(j=0; j < maxX; j++)
            {
                t[i][j]=0;
            }
        }
        

        for(i=0; i < 21; i++){
            int xAl=  1 + rand() % (maxX-1);
            int yAl= 1 + rand() % (maxY-1);
            t[yAl][xAl] = 2;
        }

        int x = 0;
        int y = 0;

        int a = maxX;
        int b = maxY;

        int c = maxX;
        int d = maxY;

        t[y][x] = 1;
        t[b][a] = 3;
        t[d][c] = 4;
}

int main(int argc, char const *argv[]) {
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesServeur;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    int nbRecu;
    int longueurAdresse;
    int pid;
    int score = 0;
    char bufScore[10] = "";
    int i,j,t[maxY][maxX];
    srand(time(0));
    char saisie = 's';
    initialiserTableau(t);
    int test;
    int test2;
    
    int x = 0;
    int y = 0;

    int a = 3;
    int b = 3;

    int c = 4;
    int d = 4;

    fdSocketAttente = socket(PF_INET, SOCK_STREAM, 0);

    if (fdSocketAttente < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (bind(fdSocketAttente, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("erreur de bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocketAttente, 5) == -1) {
        printf("erreur de listen\n");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    int nbClients = 0;

    while (nbClients < MAX_CLIENTS) {
        printf("En attente de connexion...\n");

        if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,
                                            &tailleCoord)) == -1) {
            printf("erreur de accept\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté - %s:%d\n",
               inet_ntoa(coordonneesAppelant.sin_addr),
               ntohs(coordonneesAppelant.sin_port));

        if ((pid = fork()) == 0) {
            close(fdSocketAttente);
            
        while (1) {
               
            sprintf( bufScore, "%d", score);

            send(fdSocketCommunication,t,sizeof(t),0); //j'envoie le tableau
            send(fdSocketCommunication,bufScore,sizeof(bufScore),0);

            // on attend le message du client
            // la fonction recv est bloquante
            nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0); //nombre de caractères

            if (nbRecu > 0) {
                tampon[nbRecu] = 0;
                printf("Recu de %s:%d : %s\n",
                        inet_ntoa(coordonneesAppelant.sin_addr),
                        ntohs(coordonneesAppelant.sin_port),
                        tampon);
                printf("%d",nbRecu);
            }

            t[y][x] = 0;
            
            if(test == 2){
                 t[b][a] = 2;
            } else {
               t[b][a] = 0;
            }
            
            if(test2 == 2){
                t[d][c] = 2;
            } else {
                t[d][c] = 0;
            }
            
            test = 0;
            test2 = 0;

            if (strcmp(tampon,"z")==0){
                y -= 1;
                if (my_rand()==1){
                    b += 1;
                    d -=1;
                }
                else if(my_rand()==2){
                    b -= 1;
                    c -= 1;
                }
                else if (my_rand()==3){
                    a += 1;
                    d += 1;
                }
                else {
                    a -= 1;
                    d -= 1;
                }
            }

            if (strcmp(tampon,"s")==0){
                y += 1;
                if (my_rand()==1){
                    b += 1;
                    c += 1;
                }
                else if(my_rand()==2){
                    b -= 1;
                    c -= 1;
                }
                else if (my_rand()==3){
                    a += 1;
                    d += 1;
                }
                else {
                    a -= 1;
                    c -= 1;
                }
            }

            if (strcmp(tampon,"q")==0){
                x -= 1;
                if (my_rand()==1){
                    b += 1;
                    c +=1;
                }
                else if(my_rand()==2){
                    b -= 1;
                    c -= 1;
                }
                else if (my_rand()==3){
                    a += 1;
                    d += 1;
                }
                else {
                    a -= 1;
                    d -= 1;
                }
            }

            if (strcmp(tampon,"d")==0){
                x += 1;
                if (my_rand()==1){
                    b += 1;
                    c +=1;
                }
                else if(my_rand()==2){
                    b -= 1;
                    c -= 1;
                }
                else if (my_rand()==3){
                    a += 1;
                    d += 1;
                }
                else {
                    a -= 1;
                    d -= 1;
                }
            }
                    
            if (strcmp(tampon,"o")==0){
                initialiserTableau(t);
                        
                x = 0;
                y = 0;

                a = 3;
                b = 3;

                c = 4;
                d = 4;
                        
                t[y][x] = 0;
                t[b][a] = 0;
                t[d][c] = 0;
                score = 0;
            }

            if (x < 0) {
                x += 1;
            }
                    
            if (a < 0) {
                a += 1;
            }
                    
            if (c < 0) {
                c += 1;
            }

            if (x >= maxX) {
                x -= 1;
            }
                    
            if (a >= maxX) {
                a -= 1;
            }
                    
            if (c >= maxX) {
                c -= 1;
            }

            if (y < 0) {
                y += 1;
            }
                    
            if (b < 0) {
                b += 1;
            }
                    
            if (d < 0) {
                d += 1;
            }

            if (y >= maxY){
                y -= 1;
            }
                    
            if (b >= maxY){
                b -= 1;
            }
                    
            if (d >= maxY){
                d -= 1;
            }
                    
            if (verifierCase(x, y, t)==1){
                score +=1;
            }
                    
            sprintf( bufScore, "%d", score);
            
            if (verifierCase(a, b, t)==1){
                test=2;
            }
            
            if (verifierCase(c, d, t)==1){
                test2=2;
            }

            t[y][x] = 1;
            t[b][a] = 3;
            t[d][c] = 4;


            nbClients++;
        }
            
    }

    close(fdSocketCommunication);
    close(fdSocketAttente);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    printf("Fin du programme.\n");
    return EXIT_SUCCESS;
   }
}
