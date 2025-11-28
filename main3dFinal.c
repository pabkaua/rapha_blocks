#include <raylib.h>
#include <string.h>

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

void screenMenu(MenuItems* menu){
    // FUNDO DA TELA INICIAL ------------------------------------------------------------------------------------------------------------------
    Vector2 pos = {menu->width, 0};
    DrawTextureEx(menu->background, pos, 90, 1.5, WHITE);

    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // LOGO -----------------------------------------------------------------------------------------------------------------------------------
    DrawTexture(menu->logo, (menu->play.x + menu->play.width/2 - menu->logo.width/2), 100, WHITE); // desenha a logo
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    int posYText, posXText; 
    Vector2 mouse = GetMousePosition();
    
    // BOTÃO E TEXTO JOGAR --------------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->play, menu->sizePlay, "JOGAR", 'x');
    posYText = centralizeTextRec(menu->play, menu->sizePlay, "JOGAR", 'y');
    DrawRectangleRec(menu->play, RAYWHITE);
    DrawRectangleLinesEx(menu->play, 5.0, BLACK);
    DrawText("JOGAR", posXText, posYText, menu->sizePlay, BLACK);
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO COMO JOGAR ---------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->tutorial, menu->sizeTut, "COMO JOGAR", 'x');
    posYText = centralizeTextRec(menu->tutorial, menu->sizeTut, "COMO JOGAR", 'y');
    DrawRectangleRec(menu->tutorial, RAYWHITE);
    DrawRectangleLinesEx(menu->tutorial, 5.0, BLACK);
    DrawText("COMO JOGAR", posXText, posYText, menu->sizeTut, BLACK);
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO RANKINGS -----------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->rankings, menu->sizeRankings, "RANKINGS", 'x');
    posYText = centralizeTextRec(menu->rankings, menu->sizeRankings, "RANKINGS", 'y');
    DrawRectangleRec(menu->rankings, RAYWHITE);
    DrawRectangleLinesEx(menu->rankings, 5.0, BLACK);
    DrawText("RANKINGS", posXText, posYText, menu->sizeRankings, BLACK);
    // ----------------------------------------------------------------------------------------------------------------------------------------

    // VAI FAZER AS ANIMAÇÕES DE TEXTO E EMISSÃO DE SONS CASO CLICADO / MOUSE PASSADO EM CIMA -------------------------------------------------
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
}

int main()
{
    // INICIANDO JANELA -----------------------------------------------------------------------------------------------------------------------
    int screenWidth = 1800;
    int screenHeight = 900;
    Image logo = LoadImage("./textures/logobg.png"); // mudando o icon do jogo
    
    InitWindow(screenWidth, screenHeight, "KRJ CUBES - Version 1.0.0.3");
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

    Camera cameramenu();
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
                DrawText("A", 100, 100, 20, BLACK);
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