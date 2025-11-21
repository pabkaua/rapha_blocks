#include "raylib.h"

void krjMenu(int screenHeight, int screenWidth){
    Texture logo = LoadTexture("./textures/logo.png");
    float posXLogo = (screenWidth - logo.width)/2;
    
    char* text = "Bem vindo! Selecione a opção desejada";
    float posXText = (screenWidth - MeasureText(text, 20))/2;
    
    Rectangle start = {
        (screenWidth/2 - 300),
        (screenHeight/2 + 300),
        300,
        100
    };
    
    DrawRectangle(start.x, start.y, start.width, start.height,  BLACK);
    DrawText(text, posXText, 400, 20, BLACK);
    DrawTexture(logo, posXLogo, 100, WHITE);

}

int main(void)
{
    // Iniciando janela e variaveis --------------------------------------------------------
    
    const int screenWidth = 1900;
    const int screenHeight = 900;   

    InitWindow(screenWidth, screenHeight, "KRJ BLOCKS VERSÃO 0.0.1");
    //ToggleBorderlessWindowed(); // tela cheia
    Image icon = LoadImage("./textures/logobg.png");
    SetWindowIcon(icon);
   
    
    SetTargetFPS(60);
 
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())    // Detecta ESC ou X 
    {

        // Update --------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw ----------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            krjMenu(screenHeight, screenWidth);
            
            DrawText("Um jogo por: Kauã, Renan e João", 15, screenHeight - 15, 5, GRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }


    // De-Initialization -------------------------------------------------------------------
    CloseWindow();        // Fecha janela
    //--------------------------------------------------------------------------------------

    return 0;
}