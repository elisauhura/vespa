#include "query.h"
#include "player.h"
#include "element.h"
#include "out.h"
#include "object.h"
#include "button.h"
#include "place.h"
#include "list.h"
#include "str.h"

#include <stdio.h>
#include <stdlib.h>

/* Internal data structures */

struct state {
    char _auto;
    MObj * player;
    MObj * intro;
    struct {
        MObj * patient_2;
        MObj * patient_3;
        MObj * corridor;
        MObj * recovery;
        MObj * elevator;
        MObj * control;
        MObj * preparation;
        MObj * staff;
        MObj * deposit;
        MObj * memory_management;
    } rooms;
    int state;
};

MObj * helper[200];

/* Define methods names */

static char * methods[2] = {
    "eval", NULL
};

/* Define methods itselves */

static void * _eval (void * _self, void * _arg) {
    MObj * self = _self;
    MObj * place;
    struct state * state = self->_state;
    switch(state->state) {
        case 1:
            printf("%s\n\n", (char *) Mcall(state->intro, "get name", NULL));
            printf("%s\n\n", (char *) Mcall(state->intro, "get Desc", NULL));
        break;
        case 2:
        case 7:
        case 14:
        case 22:
        case 28:
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            MObj * list = Mcall(place, "get List", NULL);
            puts("Lista de objetos:");
            int i = 0;
            for(struct MList_node * p = Mcall(list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get visibility", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há objetos");
            puts("");
        break;
        case 3:
        case 5:
        case 12:
        case 19:
        case 23:
        case 26:
        case 29:
            puts("% saídas");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            list = Mcall(place, "get paths", NULL);
            puts("Lista de saídas:");
            i = 0;
            for(struct MList_node * p = Mcall(list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get enabled", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há saídas");
            puts("");
        break;
        case 4:
            puts("% apertar o botão");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            /*later replace it with: get object: botao*/
            Mcall(helper[0], "set state", Mpack(&i_true, "int", NULL));
            puts("");
        break;
        case 6:
            puts("% sair pela porta");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            Mcall(state->player, "set location", Mpack(Mcall(helper[1], "get out", NULL), "Obj", NULL));
            puts("");
        break;
        case 8:
            puts("% examinar o defunto");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            /*Create a trigger to make examination of the defunct enables the visibility of a card*/
            printf("Observação: %s\n", (char *) Mcall(helper[3], "get Desc", NULL));
            puts("Você observa algo novo!");
            Mcall(helper[4], "set visibility", Mpack(&i_true, "int", NULL));
            list = Mcall(place, "get List", NULL);
            puts("Lista de objetos:");
            i = 0;
            for(struct MList_node * p = Mcall(list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get visibility", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há objetos");
            puts("");

        break;
        case 9:
            puts("% pegar o defunto");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            /*Add personalized reply field later*/
            if(Mcall(helper[3], "get carry", NULL) == &true) {
                puts("Isso não vai acontecer, hahaahhah");
            } else {
                puts("Você não quis dizer \"pergar\" o defunto?");
            }
            puts("");
        break;
        case 10:
            puts("% \"pegar\" o defunto");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            /*Add personalized reply field later*/
            puts("Isso foi meio errado...");
            puts("");
        break;
        case 11:
            puts("% pegar o cartão");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            list = Mcall(place, "get List", NULL);
            MObj * player_list = Mcall(state->player, "get List", NULL);
            Mcall(player_list, "insert", Mpack(Mcall(list, "remove", Mpack("cartao", "str", NULL)), "MObj", Mpack("cartao", "str", NULL)));
            /*Add personalized reply field later*/
            puts("Lista de objetos do jogador:");
            i = 0;
            for(struct MList_node * p = Mcall(player_list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get visibility", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há objetos");
            puts("");
        break;
        case 13:
            puts("% sair pela saída da direita");
            /*Add trigger when the player leave the corridor*/
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            Mcall(state->player, "set location", Mpack(Mcall(helper[5], "get out", NULL), "Obj", NULL));
            puts("");
        break;
        case 15:
            puts("% examinar o laser");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            printf("Observação: %s\n", (char *) Mcall(helper[6], "get Desc", NULL));
            puts("");
        break;
        case 16:
            puts("% examinar a secretária");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            printf("Observação: %s\n", (char *) Mcall(helper[7], "get Desc", NULL));
            puts("");
        break;
        case 17:
            puts("% escutar a secretária");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            /*later replace it with: get object: botao*/
            Mcall(helper[7], "set state", Mpack(&i_true, "int", NULL));
            Mcall(helper[4], "set visibility", Mpack(&i_true, "int", NULL));
            list = Mcall(place, "get List", NULL);
            puts("Lista de objetos:");
            i = 0;
            for(struct MList_node * p = Mcall(list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get visibility", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há objetos");
            puts("");
        break;
        case 18:
            puts("% pegar o laser");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            list = Mcall(place, "get List", NULL);
            player_list = Mcall(state->player, "get List", NULL);
            Mcall(player_list, "insert", Mpack(Mcall(list, "remove", Mpack("laser", "str", NULL)), "MObj", Mpack("laser", "str", NULL)));
            /*Add personalized reply field later*/
            puts("Lista de objetos do jogador:");
            i = 0;
            for(struct MList_node * p = Mcall(player_list, "get head", NULL); p != NULL; p = p->next) {
                MObj * element = p->value;
                if(Mcall(element, "get visibility", NULL) == &true) {
                    i++;
                    printf("%d\t%s %s\n", i, (char *) Mcall(element, "get ai", NULL), (char *) Mcall(element, "get name", NULL));
                }
            }
            if(i == 0) puts("Não há objetos");
            puts("");
        break;
        case 20:
            puts("% sair pelo elevador");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            Mcall(state->player, "set location", Mpack(Mcall(helper[8], "get out", NULL), "Obj", NULL));
            puts("");
        break;
        case 21:
            /*Need to add out triggers, this will be hardcoded for now*/
            puts("Você escuta alguns passos, mas as portas do elevador já se fecharam");
            puts("");
        break;
        case 24:
            puts("% examinar a trava");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            printf("Observação: %s\n", (char *) Mcall(helper[9], "get Desc", NULL));
            puts("");
        break;
        case 25:
            puts("% usar o cartão");
            puts("trava desbloqueada");
            Mcall(helper[9], "set visibility", Mpack(&i_false, "int", NULL));
            Mcall(helper[11], "set enabled", Mpack(&i_true, "int", NULL));
            puts("");
        break;
        case 27:
            puts("O defunto aparece do elevador, você sente medo e corre para o laboratório");
            puts("");
            Mcall(state->player, "set location", Mpack(Mcall(helper[11], "get out", NULL), "Obj", NULL));

        break;
        case 30:
            puts("% examinar a capsula vermelha");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            printf("Observação: %s\n", (char *) Mcall(helper[12], "get Desc", NULL));
            puts("");
        break;
        case 31:
            puts("Há um barulho na porta por onde você veio");
            puts("");
        break;
        case 32:
            puts("% examinar a capsula azul");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            printf("Observação: %s\n", (char *) Mcall(helper[13], "get Desc", NULL));
            puts("");
        break;
        case 33:
            puts("% entrar na capsula azul");
            place = Mcall(state->player, "get location", NULL);
            printf("Local: %s\nDescrição: %s\n", (char *) Mcall(place, "get name", NULL), (char *) Mcall(place, "get Desc", NULL));
            puts("");
            puts("Você não consegue entrar na capsula");
            puts("");
        break;
        case 34:
            puts("O defunto entrou na mesma sala que você, o medo te congela, ...");
            puts("Continua na proxima fase...");
            puts("");
        case 200:
            return &false;
        default:
        break;
    }
    state->state = state->state + 1;

    return &true;
}

static void * (* action[1]) (void * self, void * arg) = {
    _eval
};

static char * class = "Query";

static void _free (void * _self) {
    MObj * self = (MObj *) _self;
    struct state * state = self->_state;

    /*Add Mfree for the objects inside Query*/
    
    free(state);
    free(self);
}

MObj * MQuery (MObj * self, MPack * arg) {
    struct state * state = malloc(sizeof(struct state));
    if(state == NULL) {
        puts("Err MElement");
        exit(EXIT_FAILURE);
    }

    state->_auto = Mpack_bool(arg);
    state->state = 0;

    /* This is a big bit, but is the bit that contains the game story itself*/
    state->intro = Mnew(MElement, Mpack("Introdução", "str", Mpack("F", "str", Mpack("", "str", Mpack("– Droga Katie, você tinha mesmo que tentar roubar a droga da fórmula?! Você deve saber o que vai acontecer com você agora…\nLogo depois, João olhou bem nos olhos dela com uma expressão triste, deu-lhe um abraço e receitou algumas palavras em seu ouvido. Já deixando a sala ele ordenou:\n– Limpem a memória dela… Me desculpe.\n– João! Não, não fui eu! Não foi minha culpa, acredita em mim! Por favor, não me faça esquecer tudo…\nMas ele já havia deixado a sala. Katie foi levada ao complexo de controle de memória e teve suas memórias apagadas por uma droga. Isso foi tudo o que ele conseguia se lembrar.", "str", NULL)))));
    state->player = Mnew(MPlayer, Mpack("Katie", "str", Mpack("F", "str", Mpack("...", "str", Mpack("Não me lembro de muita coisa...", "str", NULL)))));

    state->rooms.patient_2 = Mnew(MPlace, Mpack("Quarto 1", "str", Mpack("M", "str", Mpack("apenas um quarto de descanso", "str", Mpack("Apenas um quarto, não há muita coisa além da cama e um botão", "str", NULL)))));
    state->rooms.patient_3 = Mnew(MPlace, Mpack("Quarto 3", "str", Mpack("M", "str", Mpack("apenas um quarto de descanso", "str", Mpack("Apenas um quarto, não há muita coisa além da cama e um botão", "str", NULL)))));
    state->rooms.corridor = Mnew(MPlace, Mpack("Corredor", "str", Mpack("M", "str", Mpack("um corredor bem grande", "str", Mpack("Um corredor bem grande, com várias portas trancadas e duas saídas", "str", NULL)))));
    state->rooms.recovery = Mnew(MPlace, Mpack("Sala de recuperação", "str", Mpack("F", "str", Mpack("com algumas macas e equipamentos estranhos", "str", Mpack("Contém algumas macas e equipamentos estranhos, pode ter alguma coisa escondida nela", "str", NULL)))));
    state->rooms.elevator = Mnew(MPlace, Mpack("Sala de elevadores", "str", Mpack("F", "str", Mpack("tem dois elevadores", "str", Mpack("Uma sala com dois elevadores, porém uma das portas está suja de sangue", "str", NULL)))));
    state->rooms.control = Mnew(MPlace, Mpack("Sala de controle", "str", Mpack("F", "str", Mpack("tem vários botões", "str", Mpack("Tem vários botões na sala, alguns monitores quebrados e um barulho de computador", "str", NULL)))));
    state->rooms.preparation = Mnew(MPlace, Mpack("Sala de preparação", "str", Mpack("F", "str", Mpack("tem vários cabos saindo do teto", "str", Mpack("Vários cabos saem do teto, todos em volta de uma mesa bem grande", "str", NULL)))));
    state->rooms.staff = Mnew(MPlace, Mpack("Sala dos funcionários", "str", Mpack("F", "str", Mpack("uma mesinha de reunião e alguns armários", "str", Mpack("Com vários armários e uma mesa de runião, não tem muito o que falar", "str", NULL)))));
    state->rooms.deposit = Mnew(MPlace, Mpack("Depósito", "str", Mpack("M", "str", Mpack("várias caixas", "str", Mpack("Várias caixas, mas a maioria parece estar vazia", "str", NULL)))));
    state->rooms.memory_management = Mnew(MPlace, Mpack("Laboratório", "str", Mpack("M", "str", Mpack("tem duas capsulas", "str", Mpack("Não dá pra descrever muito bem, mas tem duas capsulas e é um local bem frio", "str", NULL)))));
    Mcall(state->player, "set location", Mpack(state->rooms.patient_3, "Obj", NULL));

    /*patient_3 outs*/
    MObj * outs = Mcall(state->rooms.patient_3, "get paths", NULL);

    MObj * out = Mnew(MOut, Mpack(state->rooms.corridor, "Obj", Mpack("porta", "str", Mpack("F", "str", Mpack("vai para o corredor", "str", Mpack("vai para o corredor", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    /*patient_3 objects*/
    MObj * list = Mcall(state->rooms.patient_3, "get List", NULL);

    MObj * button = Mnew(MButton, Mpack("botão", "str", Mpack("M", "str", Mpack("é um botão", "str", Mpack("É um botão, talvez de pra apertar", "str", NULL)))));
    Mcall(button, "set trigger", Mpack("Uma porta se abriu", "str", Mpack(out, "Obj", Mpack("set enabled", "str", Mpack(&i_true, "int", NULL)))));
    Mcall(button, "set state", Mpack(&i_false, "int", NULL));
    Mcall(button, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(button, "MObj", Mpack("button", "str", NULL)));
    helper[0] = button;
    helper[1] = out;

    /*corridor outs*/
    outs = Mcall(state->rooms.corridor, "get paths", NULL);

    out = Mnew(MOut, Mpack(state->rooms.patient_3, "Obj", Mpack("porta do quarto 3", "str", Mpack("F", "str", Mpack("vai para o quarto 3", "str", Mpack("vai para o quarto 3", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.recovery, "Obj", Mpack("saída da direita", "str", Mpack("F", "str", Mpack("vai para a sala de recuperação", "str", Mpack("vai para a sala de recuperação", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));
    helper[5] = out;

    out = Mnew(MOut, Mpack(state->rooms.control, "Obj", Mpack("saída da esquerda", "str", Mpack("F", "str", Mpack("vai para a sala de controle", "str", Mpack("vai para a sala de controle", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.patient_2, "Obj", Mpack("porta do quarto 2", "str", Mpack("F", "str", Mpack("vai para o quarto 2", "str", Mpack("vai para o quarto 2", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    /*corridor objects*/
    list = Mcall(state->rooms.corridor, "get List", NULL);

    MObj * obj = Mnew(MObject, Mpack("defunto", "str", Mpack("M", "str", Mpack("um corpo que aparenta estar morto", "str", Mpack("Apesar de parecer estar morto, com as roupas sujas de sangue, apresenta uma respiração bem lenta", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("defunto", "str", NULL)));
    helper[3] = obj;

    obj = Mnew(MObject, Mpack("cartão", "str", Mpack("M", "str", Mpack("um cartão", "str", Mpack("Um cartão, deve servir pra abir alguma coisa", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_false, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("cartao", "str", NULL)));
    helper[4] = obj;

    /*recovery outs*/
    outs = Mcall(state->rooms.recovery, "get paths", NULL);

    out = Mnew(MOut, Mpack(state->rooms.corridor, "Obj", Mpack("entrada do corredor", "str", Mpack("F", "str", Mpack("vai para o corredor", "str", Mpack("vai para o corredor", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.elevator, "Obj", Mpack("sala dos elevadores", "str", Mpack("F", "str", Mpack("vai para os elevadores", "str", Mpack("vai para os elevadores", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.preparation, "Obj", Mpack("elevador", "str", Mpack("M", "str", Mpack("vai para o andar de baixo", "str", Mpack("vai para o andar de baixo", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));
    helper[8] = out;

    /*later add B out to the preparation, the staircase*/

    /*recovery objects*/
    list = Mcall(state->rooms.recovery, "get List", NULL);

    obj = Mnew(MObject, Mpack("maca 1", "str", Mpack("F", "str", Mpack("uma maca vazia", "str", Mpack("Um local de repouso para pessoas debilitadas", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("maca 1", "str", NULL)));

    obj = Mnew(MObject, Mpack("maca 2", "str", Mpack("F", "str", Mpack("uma maca vazia", "str", Mpack("Um local de repouso para pessoas debilitadas", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("maca 2", "str", NULL)));

    obj = Mnew(MObject, Mpack("maca 3", "str", Mpack("F", "str", Mpack("uma maca vazia", "str", Mpack("Um local de repouso para pessoas debilitadas", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("maca 3", "str", NULL)));

    button = Mnew(MButton, Mpack("secretária eletrônica", "str", Mpack("F", "str", Mpack("tem 1 mensagem gravada", "str", Mpack("tem 1 mensagem gravada", "str", NULL)))));
    Mcall(button, "set trigger", Mpack("Eric, o negócio está feio aqui, não saia dai até que as coisas se acalmem, cuide da Katie... A secretária quebrou?!", "str", Mpack(button, "Obj", Mpack("set visibility", "str", Mpack(&i_false, "int", NULL)))));
    Mcall(button, "set state", Mpack(&i_false, "int", NULL));
    Mcall(button, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(button, "MObj", Mpack("secretária", "str", NULL)));
    helper[7] = button;

    button = Mnew(MButton, Mpack("laser", "str", Mpack("M", "str", Mpack("acho que é uma arma", "str", Mpack("tem cara de ser uma arma laser, mas vai saber...", "str", NULL)))));
    Mcall(button, "set trigger", Mpack("Ta pegando fogo bicho", "str", Mpack(NULL, "Obj", Mpack("", "str", NULL))));
    Mcall(button, "set state", Mpack(&i_false, "int", NULL));
    Mcall(button, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(button, "MObj", Mpack("laser", "str", NULL)));
    helper[6] =  button;

    /*preparation outs*/
    outs = Mcall(state->rooms.preparation, "get paths", NULL);

    out = Mnew(MOut, Mpack(state->rooms.recovery, "Obj", Mpack("elevador", "str", Mpack("M", "str", Mpack("vai para o andar de cima", "str", Mpack("vai para o andar de cima", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.staff, "Obj", Mpack("sala dos funcionários", "str", Mpack("F", "str", Mpack("vai para a sala dos funcionários", "str", Mpack("vai para a sala dos funcionários", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_true, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.memory_management, "Obj", Mpack("laboratório", "str", Mpack("M", "str", Mpack("vai para o laboratório", "str", Mpack("vai para o laboratório", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));
    helper[11] = out;

    /*preparation objects*/
    list = Mcall(state->rooms.preparation, "get List", NULL);

    obj = Mnew(MObject, Mpack("trava", "str", Mpack("F", "str", Mpack("utilize o cartão para desbloquear", "str", Mpack("Uma trava digital, utilize o cartão para desbloquear", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("lock", "str", NULL)));
    helper[9] = obj;

    obj = Mnew(MObject, Mpack("cabo", "str", Mpack("M", "str", Mpack("um monte de cabo pendurado no teto", "str", Mpack("Tem um monte de cabo saindo do teto, pode dar choque", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("cable", "str", NULL)));
    helper[10] = obj;

    /*m_m outs*/
    outs = Mcall(state->rooms.memory_management, "get paths", NULL);

    out = Mnew(MOut, Mpack(state->rooms.staff, "Obj", Mpack("sala dos funcionários", "str", Mpack("F", "str", Mpack("vai para a sala dos funcionários", "str", Mpack("vai para a sala dos funcionários", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    out = Mnew(MOut, Mpack(state->rooms.preparation, "Obj", Mpack("sala de preparação", "str", Mpack("M", "str", Mpack("vai para a sala de preparação", "str", Mpack("vai para a sala de preparação", "str", NULL))))));
    Mcall(out, "set enabled", Mpack(&i_false, "int", NULL));
    Mcall(outs, "insert", Mpack(out, "MObj", Mpack("door", "str", NULL)));

    /*m_m objects*/
    list = Mcall(state->rooms.memory_management, "get List", NULL);

    obj = Mnew(MObject, Mpack("capsula vermelha", "str", Mpack("F", "str", Mpack("uma capsula vermelha", "str", Mpack("Cabe uma pessoa dentro, é grande e bem vermelha, não da pra ver o que tem dentro", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("red", "str", NULL)));
    helper[12] = obj;

    obj = Mnew(MObject, Mpack("capsula azul", "str", Mpack("F", "str", Mpack("uma capsula azul", "str", Mpack("Cabe uma pessoa dentro, é grande e bem azul, não tem nada dentro", "str", NULL)))));
    Mcall(obj, "set visibility", Mpack(&i_true, "int", NULL));
    Mcall(list, "insert", Mpack(obj, "MObj", Mpack("blue", "str", NULL)));
    helper[13] = obj;

    /*convetion*/
    Mpack_free(arg);

    self->_methods = methods;
    self->_action = action;

    self->_state = (void *) state;

    self->_class = class;

    self->_Super = NULL;
    self->_Self = self;
    self->_Child = NULL;

    self->_free = _free;

    return self;
}