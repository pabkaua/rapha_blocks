#include <raylib.h>
#include <string.h>

typedef struct menu{
    Rectangle play;
    Rectangle tutorial;
    Rectangle rankings;

    Texture logo;
    Sound hover;
    Sound click;

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

// IDENTIFICA ONDE FOI CLICADO----- -----------------------------------------------------------------------------------------------------------
int menu(MenuItems menu){
    int option = 0;

    Vector2 mousePos = GetMousePosition();

    // CHECA ONDE O MOUSE CLICOU E SE CLICOU. IDENTIFICA O QUE FOI CLICADO E RETORNA A OPÇÃO PARA A MAIN --------------------------------------
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.play)){ // se clicar em jogar
        option = 1;
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.tutorial)) { // se clica em como jogar
        option = 2;
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, menu.rankings)) { // se clica em rankings
        option = 3;
    } 
    // ----------------------------------------------------------------------------------------------------------------------------------------

    return option;
}
// --------------------------------------------------------------------------------------------------------------------------------------------

void screenMenu(MenuItems* menu){
    // TRIANGULO DA TELA INICIAL --------------------------------------------------------------------------------------------------------------
    Vector2 v1 = {0, 0};
    Vector2 v2 = {0, 500};
    float xV3 = v2.y/0.531709432; // descobrindo a posição do ultimo ponto para deixar alinhado com a logo
    Vector2 v3 = {xV3, 0};
    DrawTriangle(v1, v2, v3, LIGHTGRAY); // desenha o triangulo
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // LOGO -----------------------------------------------------------------------------------------------------------------------------------
    DrawTexture(menu->logo, 120, 80, WHITE); // desenha a logo
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    int posYText, posXText, fontsize = 40; 
    Vector2 mouse = GetMousePosition();
    
    // BOTÃO E TEXTO JOGAR --------------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->play, fontsize, "JOGAR", 'x');
    posYText = centralizeTextRec(menu->play, fontsize, "JOGAR", 'y');
    DrawText("JOGAR", posXText, posYText, fontsize, BLACK);
    DrawRectangleRoundedLinesEx(menu->play, 1.0, 10, 1.0, GRAY);
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO COMO JOGAR ---------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->tutorial, fontsize, "COMO JOGAR", 'x');
    posYText = centralizeTextRec(menu->tutorial, fontsize, "COMO JOGAR", 'y');
    DrawText("COMO JOGAR", posXText, posYText, fontsize, BLACK);
    DrawRectangleRoundedLinesEx(menu->tutorial, 1.0, 10, 1.0, GRAY);
    // ----------------------------------------------------------------------------------------------------------------------------------------
    
    // BOTÃO E TEXTO RANKINGS -----------------------------------------------------------------------------------------------------------------
    posXText = centralizeTextRec(menu->rankings, fontsize, "RANKINGS", 'x');
    posYText = centralizeTextRec(menu->rankings, fontsize, "RANKINGS", 'y');
    DrawText("RANKINGS", posXText, posYText, fontsize, BLACK);
    DrawRectangleRoundedLinesEx(menu->rankings, 1.0, 10, 1.0, GRAY);
    // ----------------------------------------------------------------------------------------------------------------------------------------

    if(CheckCollisionPointRec(mouse, menu->play)) menu->currentHover = 1; // 1 para jogar
    else if(CheckCollisionPointRec(mouse, menu->tutorial)) menu->currentHover = 2; // 2 para como jogar
    else if(CheckCollisionPointRec(mouse, menu->rankings)) menu->currentHover = 3; // 3 para rankings
    else menu->currentHover = 0;
    
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
    InitAudioDevice();

    SetWindowIcon(logo);
    // ----------------------------------------------------------------------------------------------------------------------------------------
    


    // CONFIGURAÇÕES INICIAIS -----------------------------------------------------------------------------------------------------------------
    int menuChoose = 0;
    float recButtonX = 200;
    float recButtonWidth = 400, recButtonHeight = 80;

    Rectangle play =     {recButtonX, 450, recButtonWidth, recButtonHeight};
    Rectangle tutorial = {recButtonX, 550, recButtonWidth, recButtonHeight};
    Rectangle rankings = {recButtonX, 650, recButtonWidth, recButtonHeight};
    Texture logoMenu = LoadTexture("./textures/logobg.png");
    Sound hover = LoadSound("./sounds/hover_menu.mp3");
    Sound click = LoadSound("./sounds/select_menu.mp3");

    MenuItems menuIt;
        menuIt.play = play;
        menuIt.tutorial = tutorial;
        menuIt.rankings = rankings;
        menuIt.logo = logoMenu;
        menuIt.hover = hover;
        menuIt.click = click;
        menuIt.currentHover = 0;
        menuIt.lastHover = 0;
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
        ClearBackground(WHITE);

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