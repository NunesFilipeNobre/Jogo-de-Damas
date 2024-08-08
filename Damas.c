#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define TAMANHO 8

/*Definição de corezinhas*/
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int linhasContadas=0;
char nome1[20];
char nome2[20];
char nomeVez=0;

void printaTabuleiro(char importado[TAMANHO][TAMANHO])
{
    int i,j;
    printf("  +---+---+---+---+---+---+---+---+\n");
    for(i=0;i<TAMANHO;i++)
    {
        printf("%d |",(i-8)*-1);
        for (j=0;j<TAMANHO;j++)
        {
            printf(" %c",importado[i][j]); 
            printf(" |");
        }
        printf("\n");
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    a   b   c   d   e   f   g   h\n");
}
//Ele retornar o &importado[0], mas ele deve retornar o array com 2 dimensoes
//Substitui pra ponteiro pra ele mexer diretamente, sem precisar retornar nada
void geraTabuleiro(char (*importado)[TAMANHO])
{
    int i,j;
    for(i=0;i<TAMANHO;i++)
    {
        for (j=0;j<TAMANHO;j++)
        {
            if (i<=2 && (i+j)%2 == 0)
                importado[i][j] =  'o';
            else if (i>=5 && (i+j)%2 == 0)
                importado[i][j] =  'x';
            else if ((i+j)%2 == 0)
                importado[i][j] =  ' ';
            else
                importado[i][j] = ' ';    
       }
    }
}

//Ele retornar o &importado[0], mas ele deve retornar o array com 2 dimensoes
/*Mesma coisa do de cima, declarei o novo[][] na função main() e usei o ponteiro
pra ela como parametro, sem precisar retornar nada*/
void novoTabuleiro(char (*importado)[TAMANHO], int coordInicX, int coordInicY, int coordFinX, int coordFinY, int vez, int come)
{
    coordInicX = (coordInicX-8)*(-1)+1;
    coordFinX = (coordFinX-8)*(-1)+1;
    if (vez%2 == 0)
    {
        if (come==1)
            importado[coordFinX][coordFinY-2] = ' ';
        else if (come==2)
            importado[coordFinX][coordFinY] = ' ';
        else if (come==3)
            importado[coordFinX-2][coordFinY-2] = ' ';
        else if (come==4)
            importado[coordFinX-2][coordFinY] = ' ';
        if (importado[coordInicX-1][coordInicY-1] == 'O')
            importado[coordFinX-1][coordFinY-1] = 'O';
        else
            importado[coordFinX-1][coordFinY-1] = 'o';
        importado[coordInicX-1][coordInicY-1] = ' ';
        /*Promoção*/
        if (importado[coordFinX-1][coordFinY-1]=='o' && coordFinX-1 == 7)
            importado[coordFinX-1][coordFinY-1]='O';
    }
    else if (vez%2 == 1)
    {
        if (come==1)
            importado[coordFinX][coordFinY-2] = ' ';
        else if (come==2)
            importado[coordFinX][coordFinY] = ' ';
        else if (come==3)
            importado[coordFinX-2][coordFinY-2] = ' ';
        else if (come==4)
            importado[coordFinX-2][coordFinY] = ' ';
        if (importado[coordInicX-1][coordInicY-1] == 'X')
            importado[coordFinX-1][coordFinY-1] = 'X';
        else
            importado[coordFinX-1][coordFinY-1] = 'x';
        importado[coordInicX-1][coordInicY-1] = ' ';
        /*Promoção*/
        if (importado[coordFinX-1][coordFinY-1]=='x' && coordFinX-1 == 0)
            importado[coordFinX-1][coordFinY-1]='X';
    }
}

void registerUser() {
    char username[20];
    char password[20];
    int score=0;

    printf("Insira um nome de usuario: ");
    scanf("%s", username);
    printf("Crie uma senha: ");
    scanf("%s", password);

    FILE *file = fopen("texto.txt", "a");
    if (file == NULL) {
        perror("Erro abrindo arquivo");
        return;
    }

    fprintf(file, "%s:%s:%d\n", username, password, score);
    fclose(file);

    printf(GREEN"Usuario registrado com sucesso\n"RESET);
}

int loginUser() {
    char username[20];
    char password[20];

    printf("Usuario: ");
    scanf("%s", username);
    printf("Senha: ");
    scanf("%s", password);

    switch (checkCredentials(username, password)) 
    {
        case 1:
            printf(GREEN"Login feito com sucesso\n"RESET);
            return 0;
        case 2:
            printf(RED"Usuario ja acessado\n"RESET);
            return 1;
        default:
            printf(RED"Usuario ou senha invalidos\n"RESET);
            return 1;
    }
}

int checkCredentials(const char* username, const char* password) {
    char storedUsername[20];
    char storedPassword[20];
    int storedScore;
    char line[20 + 20 + 10 + 2]; // +2 por causa do ':''
    int contaLinha=1;

    FILE *file = fopen("texto.txt", "r");
    if (file == NULL) {
        perror("Erro abrindo arquivo");
        return 0;
    }
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^:]:%[^:]:%d", storedUsername, storedPassword, &storedScore);
        if (contaLinha==linhasContadas)
        {
            if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
            {
                fclose(file);
                return 2;
            }
        }
        else if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
        {
            fclose(file);
            linhasContadas=contaLinha;
            if (nomeVez)
            {
                sscanf(storedUsername,"%s",nome2);
            }
            else
            {
                sscanf(storedUsername,"%s",nome1);
                nomeVez+=1;
            }
            return 1;
        }
        contaLinha+=1;
    }
    fclose(file);   
    return 0;
}

int updateScore(const char* username) 
{
    char storedUsername[20];
    char storedPassword[20];
    int storedScore,deu=0;
    char line[20 + 20 + 10 + 2];
    FILE *file = fopen("texto.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) 
    {
        perror("Erro abrindo arquivo");
        return;
    }

    while (fgets(line, sizeof(line), file)) 
    {
        sscanf(line, "%[^:]:%[^:]:%d", storedUsername, storedPassword, &storedScore);
        if (strcmp(username, storedUsername) == 0) 
        {
            fprintf(tempFile, "%s:%s:%d\n", storedUsername, storedPassword, storedScore+1);
            deu=storedScore+1;
        } 
        else 
        {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("texto.txt");
    rename("temp.txt", "texto.txt");
    if (deu)
    {
        return deu;
    }
    
}


int main()
{
    Beep(350,600);Beep(400,600);Beep(450,600);Beep(500,800);
    int opcao=0;
    int logins=0;
    /*Menu*/
    do
    {
        system("cls");
        printf(BLUE"          _____                    _____                    _____                    _____                    _____          \n");
    printf("         /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\         \n");
    printf("        /::\\    \\                /::\\    \\                /::\\____\\                /::\\    \\                /::\\    \\        \n");
    printf("       /::::\\    \\              /::::\\    \\              /::::|   |               /::::\\    \\              /::::\\    \\       \n");
    printf("      /::::::\\    \\            /::::::\\    \\            /:::::|   |              /::::::\\    \\            /::::::\\    \\      \n");
    printf("     /:::/\\:::\\    \\          /:::/\\:::\\    \\          /::::::|   |             /:::/\\:::\\    \\          /:::/\\:::\\    \\     \n");
    printf("    /:::/  \\:::\\    \\        /:::/__\\:::\\    \\        /:::/|::|   |            /:::/__\\:::\\    \\        /:::/__\\:::\\    \\    \n");
    printf("   /:::/    \\:::\\    \\      /::::\\   \\:::\\    \\      /:::/ |::|   |           /::::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\   \n");
    printf("  /:::/    / \\:::\\    \\    /::::::\\   \\:::\\    \\    /:::/  |::|___|______    /::::::\\   \\:::\\    \\    ___\\:::\\   \\:::\\    \\  \n");
    printf(" /:::/    /   \\:::\\ ___\\  /:::/\\:::\\   \\:::\\    \\  /:::/   |::::::::\\    \\  /:::/\\:::\\   \\:::\\    \\  /\\   \\:::\\   \\:::\\    \\ \n");
    printf("/:::/____/     \\:::|    |/:::/  \\:::\\   \\:::\\____\\/:::/    |:::::::::\\____\\/:::/  \\:::\\   \\:::\\____\\/::\\   \\:::\\   \\:::\\____\\\n");
    printf("\\:::\\    \\     /:::|____|\\::/    \\:::\\  /:::/    /\\::/    / ~~~~~/:::/    /\\::/    \\:::\\  /:::/    /\\:::\\   \\:::\\   \\::/    /\n");
    printf(" \\:::\\    \\   /:::/    /  \\/____/ \\:::\\/:::/    /  \\/____/      /:::/    /  \\/____/ \\:::\\/:::/    /  \\:::\\   \\:::\\   \\/____/ \n");
    printf("  \\:::\\    \\ /:::/    /            \\::::::/    /               /:::/    /            \\::::::/    /    \\:::\\   \\:::\\    \\     \n");
    printf("   \\:::\\    /:::/    /              \\::::/    /               /:::/    /              \\::::/    /      \\:::\\   \\:::\\____\\    \n");
    printf("    \\:::\\  /:::/    /               /:::/    /               /:::/    /               /:::/    /        \\:::\\  /:::/    /    \n");
    printf("     \\:::\\/:::/    /               /:::/    /               /:::/    /               /:::/    /          \\:::\\/:::/    /     \n");
    printf("      \\::::::/    /               /:::/    /               /:::/    /               /:::/    /            \\::::::/    /      \n");
    printf("       \\::::/    /               /:::/    /               /:::/    /               /:::/    /              \\::::/    /       \n");
    printf("        \\::/____/                \\::/    /                \\::/    /                \\::/    /                \\::/    /        \n");
    printf("         ~~                       \\/____/                  \\/____/                  \\/____/                  \\/____/         \n");
    printf("                                                                                                                              \n\n"RESET);
        printf("\t\t\t\t\t\t1.Registro\n");
        printf("\t\t\t\t\t\t2.Login\n");
        printf("\t\t\t\t\t\t3.Sair\n");
        fflush(stdin);scanf("%d",&opcao);Beep(300,300);fflush(stdin);
        
        switch (opcao)
        {
            case 1:
            {
                registerUser();
                Beep(400,200);Beep(450,200); break;
            }
            case 2:
                if(loginUser())
                {
                    opcao=4;
                    Beep(250,200);Beep(250,200);
                }
                else if (logins==1)
                {
                    Beep(400,200);Beep(450,200);
                }
                else
                {
                    logins+=1;
                    opcao=4;
                    Beep(400,200);Beep(450,200);
                }
                break;
            case 3:
                printf(RED"\n\nEncerrando o programa\n\n"RESET);exit(0); break;
            default:
                printf(RED"Opcao invalida\n"RESET);
                opcao=4;
        }
    } 
    while (opcao==1||opcao==4);

    int continua = 0;
    int turno = 1;
    int come = 0;
    int i,j,corrigeTurno=1;
    int acabou=1,checaPessaX=0,checaPessaO=0;

    char tabuleiroInic[TAMANHO][TAMANHO];
    char (*ptr_tabuleiro)[TAMANHO] = tabuleiroInic; /*Deve definir o tamanho do array no ponteiro
                                                      quando está mexendo com + de 1 dimensão*/
    int coordenadaInicX,coordenadaInicY,coordenadaFinX,coordenadaFinY;
    char reservaInicY,reservaFinalY;
    int difX,difY,moduloDifX;

    geraTabuleiro(tabuleiroInic);
    system("cls");
    printaTabuleiro(tabuleiroInic);
    do
    {
        printf(YELLOW "\nEscreva as coordenadas no formato \"LetraNumero\" (exemplo: d3)\n\n" RESET);
        if (turno%2)
            printf(CYAN "VEZ DO JOGADOR %s, PECAS X\n\n"RESET,nome1);
        else
            printf(GREEN "VEZ DO JOGADOR %s, PECAS O\n\n"RESET,nome2);
        do
        {
            do
            {
                coordenadaInicX=coordenadaInicY=coordenadaFinX=coordenadaFinY=-1;
                continua=0;
                fflush(stdin);
                printf("Peca a ser movida: "); scanf("%c%d",&reservaInicY,&coordenadaInicX); //pega a coordenada da posição
                fflush(stdin);
                printf("Para onde sera movida: "); scanf("%c%d",&reservaFinalY,&coordenadaFinX); //pega a coordenada da prox jogada
                switch (reservaInicY)
                {
                    case 'a':
                    case 'A': coordenadaInicY=1; break;
                    case 'b':
                    case 'B': coordenadaInicY=2; break;
                    case 'c':
                    case 'C': coordenadaInicY=3; break;
                    case 'd':
                    case 'D': coordenadaInicY=4; break;
                    case 'e':
                    case 'E': coordenadaInicY=5; break;
                    case 'f':
                    case 'F': coordenadaInicY=6; break;
                    case 'g':
                    case 'G': coordenadaInicY=7; break;
                    case 'h':
                    case 'H': coordenadaInicY=8; break;
                }
                switch (reservaFinalY)
                {
                    case 'a':
                    case 'A': coordenadaFinY=1; break;
                    case 'b':
                    case 'B': coordenadaFinY=2; break;
                    case 'c':
                    case 'C': coordenadaFinY=3; break;
                    case 'd':
                    case 'D': coordenadaFinY=4; break;
                    case 'e':
                    case 'E': coordenadaFinY=5; break;
                    case 'f':
                    case 'F': coordenadaFinY=6; break;
                    case 'g':
                    case 'G': coordenadaFinY=7; break;
                    case 'h':
                    case 'H': coordenadaFinY=8; break;
                }
                if ((coordenadaInicX <1 || coordenadaInicX>8)||(coordenadaInicY <1 ||coordenadaInicY >8)||(coordenadaFinX <1 ||coordenadaFinX >8)||(coordenadaFinY <1 || coordenadaFinY>8))
                {
                    printf(RED"Insira coordenadas validas\n"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }
            }
            while (continua);
            
            difX = coordenadaInicX-coordenadaFinX;
            difY = coordenadaInicY-coordenadaFinY;
            if (difX>0)
                moduloDifX=difX;
            else
                moduloDifX=-difX;

            continua=0;

            /*Dama*/
            if(tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]=='X' || tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]=='O')
            {
                /*checa se anda na diagonal*/
                if (difX!=difY && difX!= -difY)
                {
                    printf(RED"MOVIMENTO ILEGAL"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }
                else if ((coordenadaInicX>8 || coordenadaInicX<1)||(coordenadaFinX>8 || coordenadaFinX<1)||(coordenadaInicY>8 || coordenadaInicY<1)||(coordenadaFinY>8 || coordenadaFinY<1))
                {
                    printf(RED"MOVIMENTO ILEGAL \n"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }
                else if (turno%2 && (tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='x'&&tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='X'))
                {    
                    printf(RED"MOVIMENTO ILEGAL \n"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }
                else if ((turno+1)%2 && (tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='o'&&tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='O'))
                {    
                    printf(RED"MOVIMENTO ILEGAL \n"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }
                else if (tabuleiroInic[(coordenadaFinX-8)*-1][coordenadaFinY-1]!=' ')
                {
                    printf(RED"MOVIMENTO ILEGAL \n"RESET);
                    Beep(250,200);Beep(250,200);
                    continua=1;
                }

                for(i=1;i<moduloDifX;i++)
                {
                    if(turno%2)
                    {
                        if (difX<0 && difY>0) /*X sobe, Y esquerda*/
                        {
                            for (j=1;j<moduloDifX;j++)
                            {
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            }
                            if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='O') && continua!=1)
                                if (tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1+(i+1)]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1+(i+1)]=='O')
                                {    
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX<0 && difY<0) /*X sobe, Y direita*/
                        {
                            for (j=1;j<moduloDifX;j++)
                            {
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='O') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1-(i+1)]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1-(i+1)]=='O')
                                {    
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX>0 && difY>0) /*X desce, Y esquerda*/
                        {
                            for (j=1;j<moduloDifX;j++)
                            {
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='O') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1+(i+1)]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1+(i+1)]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX>0 && difY<0) /*X desce, Y direita*/
                        {
                            for (j=1;j<moduloDifX;j++)
                            {
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='O') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1-(i+1)]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1-(i+1)]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=' ';
                                        come=5;
                                    }
                        }
                    }
                    else
                    {
                        if (difX<0 && difY>0) /*X sobe, Y esquerda*/
                        {
                            for (j=1;j<moduloDifX;j++)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='X') && continua!=1)
                                if (tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1+(i+1)]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1+(i+1)]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);    
                                    Beep(250,200);Beep(250,200);  
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1+j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX<0 && difY<0) /*X sobe, Y direita*/
                        {
                            for (j=1;j<moduloDifX;j++)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='X') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1-(i+1)]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+(i+1)][coordenadaFinY-1-(i+1)]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)+j][coordenadaFinY-1-j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX>0 && difY>0) /*X desce, Y esquerda*/
                        {
                            for (j=1;j<moduloDifX;j++)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='X') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1+(i+1)]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1+(i+1)]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1+j]=' ';
                                        come=5;
                                    }
                        }
                        if (difX>0 && difY<0) /*X desce, Y direita*/
                        {
                            for (j=1;j<moduloDifX;j++)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=='O')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='X') && continua!=1)
                                if(tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1-(i+1)]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-(i+1)][coordenadaFinY-1-(i+1)]=='X')
                                {
                                    printf(RED"MOVIMENTO ILEGAL"RESET);
                                    Beep(250,200);Beep(250,200);
                                    continua=1;
                                }
                                else
                                    for (j=1;j<moduloDifX;j++)
                                    {
                                        tabuleiroInic[((coordenadaFinX-8)*-1)-j][coordenadaFinY-1-j]=' ';
                                        come=5;
                                    }
                        }
                    }
                }
            }

            else if(difX*difY==4 || difX*difY==-4) /*Come Peça*/
            {
                if (difX<0 && difY<0) /*negativo X sobe, Y direita*/
                {
                    if ((tabuleiroInic[((coordenadaFinX-8)*-1)+1][coordenadaFinY-2]==tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]) || (tabuleiroInic[((coordenadaFinX-8)*-1)+1][coordenadaFinY-2]==' '))
                    {
                        printf(RED"MOVIMENTO ILEGAL\n"RESET);
                        Beep(250,200);Beep(250,200);
                        continua=1;
                    }
                    else
                        come=1;
                }
                if (difX<0 && difY>0) /*X sobe, Y esquerda*/
                {
                    if ((tabuleiroInic[((coordenadaFinX-8)*-1)+1][coordenadaFinY]==tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]) || (tabuleiroInic[((coordenadaFinX-8)*-1)+1][coordenadaFinY]==' '))
                    {
                        printf(RED"MOVIMENTO ILEGAL\n"RESET);
                        Beep(250,200);Beep(250,200);
                        continua=1;
                    }
                    else
                        come=2;
                }
                if (difX>0 && difY<0) /*X desce, Y direita*/
                {
                    if ((tabuleiroInic[((coordenadaFinX-8)*-1)-1][coordenadaFinY-2]==tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]) || (tabuleiroInic[((coordenadaFinX-8)*-1)-1][coordenadaFinY-2]==' '))
                    {
                        printf(RED"MOVIMENTO ILEGAL\n"RESET);
                        Beep(250,200);Beep(250,200);
                        continua=1;
                    }
                    else
                        come=3;
                }
                if (difX>0 && difY>0) /*X desce, Y esquerda*/
                {
                    if ((tabuleiroInic[((coordenadaFinX-8)*-1)-1][coordenadaFinY]==tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]) || (tabuleiroInic[((coordenadaFinX-8)*-1)-1][coordenadaFinY]==' '))
                    {
                        printf(RED"MOVIMENTO ILEGAL\n"RESET);
                        Beep(250,200);Beep(250,200);
                        continua=1;
                    }
                    else
                        come=4;
                }
                
            }
            /*Checa movimentos*/
            else if (difX*difY != 1 && difX*difY != -1)
            {   
                printf(RED"MOVIMENTO ILEGAL\n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            else if (difX>0 && turno%2 || difX<0 && (turno+1)%2)
            {
                printf(RED"MOVIMENTO ILEGAL\n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            else if ((coordenadaInicX>8 || coordenadaInicX<1)||(coordenadaFinX>8 || coordenadaFinX<1)||(coordenadaInicY>8 || coordenadaInicY<1)||(coordenadaFinY>8 || coordenadaFinY<1))
            {
                printf(RED"MOVIMENTO ILEGAL \n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            else if (turno%2 && (tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='x'&&tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='X'))
            {    
                printf(RED"MOVIMENTO ILEGAL \n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            else if ((turno+1)%2 && (tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='o'&&tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]!='O'))
            {    
                printf(RED"MOVIMENTO ILEGAL \n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            else if (tabuleiroInic[(coordenadaFinX-8)*-1][coordenadaFinY-1]!=' ')
            {
                printf(RED"MOVIMENTO ILEGAL \n"RESET);
                Beep(250,200);Beep(250,200);
                continua=1;
            }
            if (continua)
                come=0;
        }
        while(continua);
        Beep(400,200);
        
        system("cls");

        printf("\n");
        novoTabuleiro(tabuleiroInic,coordenadaInicX,coordenadaInicY,coordenadaFinX,coordenadaFinY,turno,come);
        printaTabuleiro(tabuleiroInic);
        if (come!=0)
        {
            if (tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]=='X' || tabuleiroInic[(coordenadaInicX-8)*-1][coordenadaInicY-1]=='O')
            {
                for(i=1;i<8;i++)
                {
                    if(turno%2)
                    {
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='O') && (((coordenadaFinX-8)*-1)+i)>-1 && (((coordenadaFinX-8)*-1)+i)<8 && ((coordenadaFinY-1)+i)>-1 && (coordenadaFinY-1+i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='O') && (((coordenadaFinX-8)*-1)+i)>-1 && (((coordenadaFinX-8)*-1)+i)<8 && ((coordenadaFinY-1)-i)>-1 && (coordenadaFinY-1-i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='O') && (((coordenadaFinX-8)*-1)-i)>-1 && (((coordenadaFinX-8)*-1)-i)<8 && ((coordenadaFinY-1)+i)>-1 && (coordenadaFinY-1+i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='o' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='O') && (((coordenadaFinX-8)*-1)-i)>-1 && (((coordenadaFinX-8)*-1)-i)<8 && ((coordenadaFinY-1)-i)>-1 && (coordenadaFinY-1-i)<8)
                            corrigeTurno=0;
                    }
                    if(turno%2)
                    {
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+i]=='X') && (((coordenadaFinX-8)*-1)+i)>-1 && (((coordenadaFinX-8)*-1)+i)<8 && ((coordenadaFinY-1)+i)>-1 && (coordenadaFinY-1+i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1-i]=='X') && (((coordenadaFinX-8)*-1)+i)>-1 && (((coordenadaFinX-8)*-1)+i)<8 && ((coordenadaFinY-1)-i)>-1 && (coordenadaFinY-1-i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1+i]=='X') && (((coordenadaFinX-8)*-1)-i)>-1 && (((coordenadaFinX-8)*-1)-i)<8 && ((coordenadaFinY-1)+i)>-1 && (coordenadaFinY-1+i)<8)
                            corrigeTurno=0;
                        if ((tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='x' || tabuleiroInic[((coordenadaFinX-8)*-1)-i][coordenadaFinY-1-i]=='X') && (((coordenadaFinX-8)*-1)-i)>-1 && (((coordenadaFinX-8)*-1)-i)<8 && ((coordenadaFinY-1)-i)>-1 && (coordenadaFinY-1-i)<8)
                            corrigeTurno=0;
                    }
                }
            }
            else
                for(i=-1;i<2;i+=2)
                    for (j=-1;j<2;j+=2)
                        if((tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+j]==tabuleiroInic[(coordenadaFinX-8)*-1][coordenadaFinY-1]) || (tabuleiroInic[((coordenadaFinX-8)*-1)+i][coordenadaFinY-1+j]==' '));
                        else
                            if((tabuleiroInic[((coordenadaFinX-8)*-1)+(2*i)][coordenadaFinY-1+(2*j)]==' ') && (((coordenadaFinX-8)*-1)+(2*i)>-1)&&(((coordenadaFinX-8)*-1)+(2*i)<8) && (coordenadaFinY-1+(2*j)>-1)&&(coordenadaFinY-1+(2*j)<8))
                                corrigeTurno=0;
        }
        if(corrigeTurno!=0)
            turno+=1;
        else
        {
            printf(YELLOW "\nContinue a jogada\n" RESET);
            Beep(600,200);
        }
        come=0;
        corrigeTurno=1;
        checaPessaX=0;
        checaPessaO=0;

        for (i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                if(tabuleiroInic[i][j]=='x' || tabuleiroInic[i][j]=='X')
                    checaPessaX+=1;
                if(tabuleiroInic[i][j]=='o' || tabuleiroInic[i][j]=='O')
                    checaPessaO+=1;
            }
        }

        if (checaPessaX==0)
        {
            acabou=0;
            printf(GREEN "VITORIA DAS O\n+1 ponto\n");
            Beep(400,200);Beep(425,200);Beep(450,200);
            printf("%s: %d pontos",nome2,updateScore(nome2));
        }
        if (checaPessaO==0)
        {
            acabou=0;
            printf(CYAN "VITORIA DAS X\n+1 ponto\n");
            Beep(400,200);Beep(425,200);Beep(450,200);
            printf("%s: %d pontos",nome1,updateScore(nome1));
        }
    } 
    while (acabou);
}