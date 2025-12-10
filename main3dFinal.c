#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct menu{
    int width;
    int height;
    // BOTÕES
    Rectangle play;
    Rectangle tutorial;
    Rectangle rankings;
    // TAMANHO DOS TEXTOS
    int sizePlay;
    int sizeTut;
    int sizeRankings;
    // LOGO E SONS
    Texture background;
    Texture logo;
    Sound hover;
    Sound click;
    // CONFIGURAÇÕES AUXILIARES DO SOM
    int currentHover;
    int lastHover;

} MenuItems;

// CENTRALIZA TEXTO EM UM RETANGULO -----------------------------------------------------------------------------------------------------------
int centralizeTextRec(Rectangle button, int fontsize, char* text, char axis){
    int pos = 0;
    
    if(axis == 'y') pos = (button.height/2) - (fontsize/2) + button.y;
    else if(axis == 'x') pos = (button.width/2) - (MeasureText(text, fontsize)/2) + button.x;
    
    return pos;
};
// --------------------------------------------------------------------------------------------------------------------------------------------

// IDENTIFICA ONDE FOI CLICADO ----------------------------------------------------------------------------------------------------------------
int menu(MenuItems menu){
    int option = 0;

    Vector2 mousePos = GetMousePosition();

    // CHECA ONDE O MOUSE CLICOU E SE CLICOU. IDENTIFICA O QUE FOI CLICADO E RETORNA A OPÇÃO PARA A MAIN --------------------------------------
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.play)){ // se clicar em jogar
        option = 1;
        PlaySound(menu.click);
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.tutorial)) { // se clica em como jogar
        option = 2;
        PlaySound(menu.click);
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.rankings)) { // se clica em rankings
        option = 3;
        PlaySound(menu.click);
    } 
    // ----------------------------------------------------------------------------------------------------------------------------------------
    return option;
}
// --------------------------------------------------------------------------------------------------------------------------------------------

// PRINTA O MENU ------------------------------------------------------------------------------------------------------------------------------
void screenMenu(MenuItems* menu){
    // FUNDO DA TELA INICIAL -----------------------------
    Vector2 pos = {menu->width, 0};
    DrawTextureEx(menu->background, pos, 90, 1.5, WHITE);
    // ---------------------------------------------------
    
    // LOGO ---------------------------------------------------------------------------------------
    DrawTexture(menu->logo, (menu->play.x + menu->play.width/2 - menu->logo.width/2), 100, WHITE); 
    // --------------------------------------------------------------------------------------------
    
    int posYText, posXText; 
    Vector2 mouse = GetMousePosition();
    
    // BOTÃO E TEXTO JOGAR ------------------------------------------------------------
    posXText = centralizeTextRec(menu->play, menu->sizePlay, "JOGAR", 'x');
    posYText = centralizeTextRec(menu->play, menu->sizePlay, "JOGAR", 'y');
    DrawRectangleRec(menu->play, RAYWHITE);
    DrawRectangleLinesEx(menu->play, 5.0, BLACK);
    DrawText("JOGAR", posXText, posYText, menu->sizePlay, BLACK);
    // --------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO COMO JOGAR -------------------------------------------------------
    posXText = centralizeTextRec(menu->tutorial, menu->sizeTut, "COMO JOGAR", 'x');
    posYText = centralizeTextRec(menu->tutorial, menu->sizeTut, "COMO JOGAR", 'y');
    DrawRectangleRec(menu->tutorial, RAYWHITE);
    DrawRectangleLinesEx(menu->tutorial, 5.0, BLACK);
    DrawText("COMO JOGAR", posXText, posYText, menu->sizeTut, BLACK);
    // --------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO RANKINGS ---------------------------------------------------------
    posXText = centralizeTextRec(menu->rankings, menu->sizeRankings, "RANKINGS", 'x');
    posYText = centralizeTextRec(menu->rankings, menu->sizeRankings, "RANKINGS", 'y');
    DrawRectangleRec(menu->rankings, RAYWHITE);
    DrawRectangleLinesEx(menu->rankings, 5.0, BLACK);
    DrawText("RANKINGS", posXText, posYText, menu->sizeRankings, BLACK);
    // --------------------------------------------------------------------------------

    // VAI FAZER AS ANIMAÇÕES DE TEXTO E EMISSÃO DE SONS CASO MOUSE PASSADO EM CIMA ---
    if(CheckCollisionPointRec(mouse, menu->play)) {
        menu->currentHover = 1; // 1 para jogar
        menu->sizePlay = 50;
    }
    else if(CheckCollisionPointRec(mouse, menu->tutorial)) {
        menu->currentHover = 2; // 2 para como jogar
        menu->sizeTut = 50;
    }
    else if(CheckCollisionPointRec(mouse, menu->rankings)) {
        menu->currentHover = 3; // 3 para rankings
        menu ->sizeRankings = 50;
    }
    else {
        menu->currentHover = 0;
        menu->sizePlay = 40;
        menu->sizeTut = 40;
        menu ->sizeRankings = 40;
    }
    
    if(menu->currentHover != menu->lastHover && menu->currentHover != 0){
        PlaySound(menu->hover);
    }
    menu->lastHover = menu-> currentHover;
    // --------------------------------------------------------------------------------
}
// --------------------------------------------------------------------------------------------------------------------------------------------



typedef struct cube{
    Vector3 pos;
    Color color;
    
    struct cube* next;
} Cube;

typedef struct menuplay{
    // CONFIGURAÇÕES GERAIS
    bool gameOver;
    int status;
    int difficulty;
    int blocksnumber;
    // CONFIGURAÇÕES DE NICK
    char nick[50];
    Rectangle nickRec;
    Texture nickBg;
    // CONFIGURAÇÕES DE RANKING
    int score;
    int highscore;
    int lifes;

    double timeStart;
    Model cubeModel;

    int userGuess;
    Sound guessUp;
    Sound guessDown;
    Sound guessCorrect;
    Sound guessWrong;

    Camera camera;
    Cube* begin;
} PlayConfigs;

typedef struct {
    int pos;
    char nick[50];
    int score;
} RankItem;

int getNick(PlayConfigs* configs){ // pega o nick do usuário
    Vector2 pos = {1800, 0};
    DrawTextureEx(configs->nickBg, pos, 90, 1.5, WHITE);
    DrawRectangleRec(configs->nickRec, RAYWHITE);
    DrawRectangleLinesEx(configs->nickRec, 5.0, BLACK);

    int posXTitle = (1800 - MeasureText("Digite seu nick e confirme com ENTER", 70))/2;
    DrawText("Digite seu nick e confirme com ENTER", posXTitle, 200, 70, BLACK);

    int posXNick = centralizeTextRec(configs->nickRec, 40, "aaaaaaaaaaaaaaa", 'x');
    int posYNick = centralizeTextRec(configs->nickRec, 40, "AAAAAAAAAAAAAA", 'y');
    DrawText(configs->nick, posXNick, posYNick, 40, BLACK);

    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && strlen(configs->nick) < 15) {
            int len = strlen(configs->nick);
            configs->nick[len] = (char)key;
            configs->nick[len+1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(configs->nick);
        if (len > 0) configs->nick[len-1] = '\0';
    }

    if (strlen(configs->nick) > 3 && IsKeyPressed(KEY_ENTER)) {
        return 0;
    }

    return -1;
}

int getHighScore(char nick[]){ // puxa o recorde com base no nick no arquivo
    int pos, scoreFile;
    int score = 0;
    char nickFile[50];

    FILE* pontArq = fopen("rankings.txt", "r");

    while(fscanf(pontArq, "%d - %s | %d", &pos, nickFile, &scoreFile) == 3){
        if(strcmp(nick, nickFile) == 0){
            score = scoreFile;
        }
    }

    fclose(pontArq);
    return score;
}

void freeList(Cube** beginPtr){ // libera a lista com os blocos a cada nivel
    Cube* current = *beginPtr;
    Cube* nextCube;

    while (current != NULL) {
        nextCube = current->next;
        free(current);
        current = nextCube;
    }

    *beginPtr = NULL;
}

void givePoints(PlayConfigs* configs){ // dá pontos e/ou tira vidas
    if(configs->blocksnumber == configs->userGuess){
        configs->score += 100;
    } else if((configs->blocksnumber == (configs->userGuess+1)) || (configs->blocksnumber == (configs->userGuess-1))){
        configs->score += 50;
        (configs->lifes)--;
    } else {
        (configs->lifes)--;
    }
}

float getYposition(float x, float z, Cube* start){ // empilha os cubos
    float y = 1;
    Cube* current = start;
    while(current != NULL){
        if(current->pos.x == x && current->pos.z == z){
            y ++;
        }
        current = current->next;
    }
    return y;
}

void printCubes(PlayConfigs* configs, Color color){ // printa os cubos 3d
    Cube* current = configs->begin;

    while(current!= NULL){
        DrawModel(configs->cubeModel, current->pos, 1, color);
        current = current->next;
    }
}

void updateRankings(char* nick, int points){

    RankItem ranks[20]; // mais que 10 para não estourar ao inserir
    int count = 0;

    FILE* file = fopen("rankings.txt", "r");

    // 1. LER O ARQUIVO EXISTENTE ------------------------------------------
    if(file != NULL){
        while(fscanf(file, "%d - %s | %d", 
                     &ranks[count].pos, 
                     ranks[count].nick, 
                     &ranks[count].score) == 3)
        {
            count++;
            if(count >= 19) break;
        }
        fclose(file);
    }

    // 2. ADICIONAR O NOVO RESULTADO --------------------------------------
    strcpy(ranks[count].nick, nick);
    ranks[count].score = points;
    count++;

    // 3. ORDENAR POR SCORE (DESC) ----------------------------------------
    for(int i = 0; i < count-1; i++){
        for(int j = i+1; j < count; j++){
            if(ranks[j].score > ranks[i].score){
                RankItem temp = ranks[i];
                ranks[i] = ranks[j];
                ranks[j] = temp;
            }
        }
    }

    // 4. REESCREVER O ARQUIVO (TOP 10) -----------------------------------
    file = fopen("rankings.txt", "w");
    if(file == NULL) return;

    int limit = (count < 10) ? count : 10;

    for(int i = 0; i < limit; i++){
        fprintf(file, "%d - %s | %d\n", i+1, ranks[i].nick, ranks[i].score);
    }

    fclose(file);
}

void screenPlay(PlayConfigs* configs){

    if(configs->status == -1){ // pega o nick e puxa o recorde
        configs->status = getNick(configs);
        configs->highscore = 0;

        if(configs->status == 0){
            configs->highscore = getHighScore(configs->nick);
        }
    }

    if(configs->status == 0){ // configuras os cubos e cria a lista

        freeList(&configs->begin); // limpa a lista

        int min, max;
        float posX, posZ, posY; // configurando a quantidade de cubos 

        min = (configs->difficulty/3)+1;
        max = (configs->difficulty/2)+2;
        configs->blocksnumber = GetRandomValue(min, max);

        Cube* current = configs->begin;
        int blocksConfigured = 0;

        while(blocksConfigured < configs->blocksnumber){ // cria a lista com os cubos
            Cube* newCube = malloc(sizeof(Cube));
            posX = GetRandomValue(-2, 3) - 0.5;
            posZ = GetRandomValue(-2, 3) - 0.5;
            posY = getYposition(posX, posZ, configs->begin) - 0.5;
            newCube->pos = (Vector3){posX, posY, posZ};
            newCube->color = WHITE;
            newCube->next = NULL;

            if(configs->begin == NULL){
                configs->begin = newCube;
                current = configs->begin;
            }
            else {
                current->next = newCube;
                current = newCube;
            }
            
            blocksConfigured ++;
        }

        configs->status = 1;
        configs->timeStart = GetTime();
    }
    
    if(configs->status == 1){ // faz a contagem de 3s
        double elapsedTime = 3 - (GetTime() - configs->timeStart);
        if(elapsedTime <= 0){
            configs->status = 2;
            configs->timeStart = GetTime();
        } else {
            DrawText(TextFormat("%.0f", elapsedTime), 900 - MeasureText("0", 100)/2, 100 , 100, BLACK);
        }
    }
    
    if(configs->status == 2){ // aparece os cubos por 2 segundos
        DrawText("Quantos cubos há?", 900 - MeasureText("Quantos cubos há?", 100)/2, 100 , 100, BLACK);
        //DrawText(TextFormat("%d", configs->blocksnumber), 100, 100, 100, BLACK);
        
        double elapsedTime = 2 - (GetTime() - configs->timeStart);
        if(elapsedTime <= 0){
            configs->status = 3;
        } else {
            BeginMode3D(configs->camera);
                DrawGrid(6, 1);
                printCubes(configs, WHITE);
            EndMode3D(); 
        }
    }
    
    if(configs->status == 3){ // pega a entrada do usuário
        if((IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) && configs->userGuess > 0){
            configs->userGuess--;
            PlaySound(configs->guessDown);
        } 
        else if(IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT)){
            configs->userGuess++;
            PlaySound(configs->guessUp);
        } 

        if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
            givePoints(configs);
            configs->status = 4;

            if(configs->userGuess == configs->blocksnumber){
                PlaySound(configs->guessCorrect);
            } else {
                PlaySound(configs->guessWrong);
            }
            configs->timeStart = GetTime();
        }

        DrawText("Quantos cubos há?", 900 - MeasureText("Quantos cubos há?", 100)/2, 100 , 100, BLACK);
        DrawText(TextFormat("< %d >", configs->userGuess), 900 - MeasureText(TextFormat("< %d >", configs->userGuess), 50)/2, 750, 50, BLACK);
        DrawText(TextFormat("Vidas: %d", configs->lifes), 200, 750, 50, BLACK);

        BeginMode3D(configs->camera);
            DrawGrid(6, 1);     
        EndMode3D(); 
    }

    if(configs->status == 4){ // printa o resultado

        double elapsedTime = 2 - (GetTime() - configs->timeStart);

        if(elapsedTime <= 0){
            configs->status = 5;
        } 
        else {
            Color color = (configs->userGuess == configs->blocksnumber) ? GREEN : RED;
    
            DrawText("Quantos cubos há?", 900 - MeasureText("Quantos cubos há?", 100)/2, 100 , 100, BLACK);
            DrawText(TextFormat("%d", configs->blocksnumber), 900 - MeasureText("0", 50)/2, 750, 50, BLACK);

            BeginMode3D(configs->camera);
                DrawGrid(6, 1);  
                printCubes(configs, color);
            EndMode3D();
        }
    }

    if(configs->status == 5){ // reseta tudo ou dá game over
        configs->userGuess = 0;
        configs->difficulty++;

        if(configs->lifes == 0){

            DrawText("Game over", 900 - MeasureText("Game over", 100)/2, 100 , 100, BLACK);
            DrawText("Pressione ENTER", 900 - MeasureText("Pressione ENTER", 20)/2, 200 , 20, BLACK);
            DrawText(TextFormat("Pontuação: %d", configs->score), 900 - MeasureText(TextFormat("Pontuação: %d", configs->score), 50)/2, 750, 50, BLACK);
            
            if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
                updateRankings(configs->nick, configs->score);
                configs->gameOver = true;
                freeList(&configs->begin);
            } 
        } 
        else configs->status = 0;
    }
}


int screenRankings(Texture bg){
    DrawTextureEx(bg, (Vector2){1800, 0}, 90, 1.5, WHITE);
    RankItem ranks[10];
    int count = 0;

    FILE* f = fopen("rankings.txt", "r");
    if(f != NULL){
        while(fscanf(f, "%d - %s | %d", &ranks[count].pos, ranks[count].nick, &ranks[count].score) == 3)
        {
            count++;
            if(count >= 10) break;
        }
        fclose(f);
    }

    // Título
    int titleSize = 80;
    char title[] = "RANKING - TOP 10";
    DrawText(title, 900 - MeasureText(title, titleSize)/2, 80, titleSize, BLACK);

    if(count == 0){     // sem rankings
        DrawText("Nenhum ranking registrado ainda!", 900 - MeasureText("Nenhum ranking registrado ainda!", 40)/2, 300, 40, BLACK);

        DrawText("Pressione ENTER para voltar", 900 - MeasureText("Pressione ENTER para voltar", 30)/2, 
                 700, 30, DARKGRAY);

        if(IsKeyPressed(KEY_ENTER)) return 1;
        return 0;
    }

    // Exibe os rankings
    int startY = 220;
    int spacing = 55;

    for(int i = 0; i < count; i++){
        char line[200];
        sprintf(line, "%dº  -  %s  |  %d pontos", i + 1, ranks[i].nick, ranks[i].score);
        DrawText(line, 900 - MeasureText(line, 40)/2, startY + i * spacing, 40, BLACK);
    }

    // Instrução para voltar
    DrawText("Pressione ENTER para voltar", 900 - MeasureText("Pressione ENTER para voltar", 30)/2, 780, 30, DARKGRAY);

    if(IsKeyPressed(KEY_ENTER)) return 1;

    return 0; // ainda na tela
}

int screenTutorial(Texture bg)
{
    DrawTextureEx(bg, (Vector2){1800,0}, 90, 1.5, WHITE);

    if (IsKeyPressed(KEY_ENTER)) return 1; // volta ao menu
    int y = 40; // posição inicial Y
    int spacing = 35; // espaço entre linhas

    // Título
    DrawText("TUTORIAL", 20, y, 40, BLACK);
    y += 60;

    // Como o jogo funciona
    DrawText("Objetivo: observar os cubos e acertar a quantidade total.", 20, y, 25, BLACK);
    y += spacing;

    DrawText("Os cubos aparecem por 2 segundos na tela.", 20, y, 25, BLACK);
    y += spacing;

    DrawText("Depois disso, voce deve responder quantos cubos viu.", 20, y, 25, BLACK);
    y += spacing;

    DrawText("A dificuldade aumenta gradualmente a cada rodada.", 20, y, 25, BLACK);
    y += spacing + 10;

    // Controles
    DrawText("CONTROLES:", 20, y, 30, RED);
    y += 45;

    DrawText("A / <-  : diminui a resposta", 20, y, 25, BLACK);
    y += spacing;

    DrawText("D / ->  : aumenta a resposta", 20, y, 25, BLACK);
    y += spacing;

    DrawText("ENTER   : confirma a resposta", 20, y, 25, BLACK);
    y += spacing + 10;

    // Pontuação
    DrawText("PONTOS:", 20, y, 30, RED);
    y += 45;

    DrawText("Acerto exato: +100 pontos", 20, y, 25, BLACK);
    y += spacing;

    DrawText("Erro por 1: +50 pontos (perde 1 vida)", 20, y, 25, BLACK);
    y += spacing;

    DrawText("Erro maior: 0 pontos (perde 1 vida)", 20, y, 25, BLACK);
    y += spacing + 10;

    // Vidas
    DrawText("Voce possui 5 vidas. Ao zerar, o jogo acaba.", 20, y, 25, BLACK);
    y += spacing + 20;

    // Instrução para voltar
    DrawText("Pressione ENTER para voltar", 20, y, 25, GREEN);
    
    return 0;
}


int main()
{
    // INICIANDO JANELA -----------------------------------------------------------------------------------------------------------------------
    int screenWidth = 1800;
    int screenHeight = 900;
    Image logo = LoadImage("./textures/logobg.png"); // mudando o icon do jogo
    
    InitWindow(screenWidth, screenHeight, "KRJ CUBES - Version 2.0.1.1");
    SetWindowIcon(logo);

    InitAudioDevice();
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    
    // CONFIGURAÇÕES INICIAIS -----------------------------------------------------------------------------------------------------------------
    int menuChoose = -1;
    float recButtonWidth = 400, recButtonHeight = 80;
    float recButtonX = screenWidth/2 - recButtonWidth/2;
    
    Rectangle play =     {recButtonX, 450, recButtonWidth, recButtonHeight};
    Rectangle tutorial = {recButtonX, 550, recButtonWidth, recButtonHeight};
    Rectangle rankings = {recButtonX, 650, recButtonWidth, recButtonHeight};
    Texture logoMenu = LoadTexture("./textures/logobg.png");
    Texture background = LoadTexture("./textures/bg.jpg");
    Sound click = LoadSound("./sounds/select_menu.mp3");
    Sound hover = LoadSound("./sounds/hover_menu.mp3");
    
    MenuItems menuIt;
        menuIt.width = screenWidth;
        menuIt.height = screenHeight;

        menuIt.play = play;
        menuIt.sizePlay = 40;
        menuIt.tutorial = tutorial;
        menuIt.sizeTut = 40;
        menuIt.rankings = rankings;
        menuIt.sizeRankings = 40;

        menuIt.logo = logoMenu;
        menuIt.background = background;

        menuIt.hover = hover;
        menuIt.click = click;
        menuIt.currentHover = 0;
        menuIt.lastHover = 0;

    Camera game;
        game.fovy = 45.0;
        game.position = (Vector3){3, 17, 5};
        game.target = (Vector3){0, 0, 0};
        game.up = (Vector3){0, 1, 0};
        game.projection = CAMERA_PERSPECTIVE;

    Mesh cubeMesh = GenMeshCube(1, 1, 1);
    Model cube = LoadModelFromMesh(cubeMesh);
    Image imgCubeSide = LoadImage("./textures/cube_side.png");
    Texture textureCubeSide = LoadTextureFromImage(imgCubeSide);
    SetMaterialTexture(cube.materials, MATERIAL_MAP_ALBEDO, textureCubeSide);
    Rectangle nick = {recButtonX, 550, recButtonWidth, recButtonHeight};

    Sound guessUp = LoadSound("./sounds/guess_up.mp3");
    Sound guessDown = LoadSound("./sounds/guess_down.mp3");
    Sound guessCorrect = LoadSound("./sounds/guess_correct.mp3");
    Sound guessWrong = LoadSound("./sounds/guess_wrong.mp3");

    PlayConfigs playIt;
        playIt.camera = game;
        playIt.cubeModel = cube;
        
        playIt.guessUp = guessUp;
        playIt.guessDown = guessDown;
        playIt.guessCorrect = guessCorrect;
        playIt.guessWrong = guessWrong;
        playIt.nickRec = nick;
        playIt.nickBg = background;
    
        
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // ATUALIZANDO ------------------------------------------------------------------------------------------------------------------------
        if(menuChoose == -1){
            playIt.difficulty = 1;
            playIt.blocksnumber = 0;
            playIt.begin = NULL;
            playIt.status = -1;
            playIt.userGuess = 0;
            playIt.score = 0;
            playIt.lifes = 5;
            playIt.gameOver = false;
            strcpy(playIt.nick, "\0");

            menuChoose = 0;
        }
        
        if(menuChoose == 0){
            menuChoose = menu(menuIt);
        } 
        // ------------------------------------------------------------------------------------------------------------------------------------

        // DESENHANDO -------------------------------------------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        

            if(menuChoose == 0){
                screenMenu(&menuIt);
            }
            else if(menuChoose == 1){
                if(playIt.gameOver == true) menuChoose = -1;
                else screenPlay(&playIt); 
            }
            else if (menuChoose == 2){
                if(screenTutorial(background) == 1) menuChoose = 0;
            }
            else if (menuChoose == 3){
                if(screenRankings(background) == 1) menuChoose = 0;
            }

            DrawText("Um jogo por: Kauã, Renan e João", 15, screenHeight - 15, 5, GRAY);
        
        EndDrawing();
        // ------------------------------------------------------------------------------------------------------------------------------------
    }

    UnloadImage(logo);
    UnloadImage(imgCubeSide);
    UnloadTexture(logoMenu);
    UnloadTexture(textureCubeSide);
    UnloadSound(hover);
    UnloadSound(click);
    UnloadSound(guessDown);
    UnloadSound(guessUp);
    UnloadSound(guessWrong);
    UnloadSound(guessCorrect);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}