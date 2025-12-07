#include <raylib.h>
#include <string.h>
#include <stdlib.h>

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
    int status;
    int difficulty;
    int blocksnumber;
    double timeStart;
    Model cubeModel;

    Camera camera;
    Cube* begin;
} PlayConfigs;



void freeList(Cube** beginPtr){
    Cube* current = *beginPtr;
    Cube* nextCube;

    while (current != NULL) {
        nextCube = current->next;
        free(current);
        current = nextCube;
    }

    *beginPtr = NULL;
}

int getYposition(float x, float z, Cube* start){ // empilha os cubos
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

void printCubes(PlayConfigs* configs){
    Cube* current = configs->begin;

    while(current!= NULL){
        //if(current->pos.x < 0){}
        DrawModel(configs->cubeModel, current->pos, 1, WHITE);
        current = current->next;
    }
}

void screenPlay(PlayConfigs* configs){

    if(configs->status == 0){ // configuras os cubos e cria a lista
        freeList(&configs->begin); // limpa a lista

        int min, max;
        float posX, posZ, posY; // configurando a quantidade de cubos 

        min = (configs->difficulty/3)+1;
        max = (configs->difficulty/2)+2;
        configs->blocksnumber = GetRandomValue(9, 10);

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

    if(configs->status == 1){ // faz a contagem de 5s
        double elapsedTime = 5 - (GetTime() - configs->timeStart);
        if(elapsedTime <= 0){
            configs->status = 2;
            configs->timeStart = GetTime();
        } else {
            DrawText(TextFormat("%.0f", elapsedTime), 900 - MeasureText("0", 100)/2, 100 , 100, BLACK);
        }
    }
    
    if(configs->status == 2){ // pergunta e aparece os cubos por 3 segundos
        DrawText("Quantos cubos há?", 900 - MeasureText("Quantos cubos há?", 100)/2, 100 , 100, BLACK);
        DrawText(TextFormat("%d", configs->blocksnumber), 100, 100, 100, BLACK);

        /*double elapsedTime = 3 - (GetTime() - configs->timeStart);
        
        if(elapsedTime <= 0){
            configs->status = 3;
        } else {*/

            BeginMode3D(configs->camera);
                DrawGrid(6, 1);
                printCubes(configs);
            EndMode3D(); 
        //}

        if(IsKeyReleased(KEY_SPACE)){
            configs->status = 0;
        }
    }

}









int main()
{
    // INICIANDO JANELA -----------------------------------------------------------------------------------------------------------------------
    int screenWidth = 1800;
    int screenHeight = 900;
    Image logo = LoadImage("./textures/logobg.png"); // mudando o icon do jogo
    
    InitWindow(screenWidth, screenHeight, "KRJ CUBES - Version 1.0.0.6");
    SetWindowIcon(logo);

    InitAudioDevice();
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    
    // CONFIGURAÇÕES INICIAIS -----------------------------------------------------------------------------------------------------------------
    int menuChoose = 0;
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
        menuIt.tutorial = tutorial;
        menuIt.rankings = rankings;

        menuIt.logo = logoMenu;
        menuIt.background = background;
        menuIt.hover = hover;
        menuIt.click = click;

        menuIt.currentHover = 0;
        menuIt.lastHover = 0;
        menuIt.sizePlay = 40;
        menuIt.sizeTut = 40;
        menuIt.sizeRankings = 40;

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

    PlayConfigs playIt;
        playIt.difficulty = 1;
        playIt.blocksnumber = 0;
        playIt.begin = NULL;
        playIt.status = 0;
        playIt.camera = game;
        playIt.cubeModel = cube;
        
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // ATUALIZANDO ------------------------------------------------------------------------------------------------------------------------
        if(menuChoose == 0){
            menuChoose = menu(menuIt);
        } 
        // ------------------------------------------------------------------------------------------------------------------------------------

        // DESENHANDO -------------------------------------------------------------------------------------------------------------------------
        ClearBackground(RAYWHITE);

        BeginDrawing();
        

            if(menuChoose == 0){
                screenMenu(&menuIt);
            }
            else if(menuChoose == 1){
                screenPlay(&playIt);
            }
            else if (menuChoose == 2){
                DrawText("B", 100, 100, 20, BLACK);
                // screenTutorial();
            }
            else if (menuChoose == 3){
                DrawText("C", 100, 100, 20, BLACK);
                // screenRankings();
            }

            DrawText("Um jogo por: Kauã, Renan e João", 15, screenHeight - 15, 5, GRAY);
        
        EndDrawing();
        // ------------------------------------------------------------------------------------------------------------------------------------
    }

    UnloadImage(logo);
    UnloadSound(hover);
    UnloadSound(click);
    UnloadTexture(logoMenu);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}