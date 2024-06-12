
#include <stdio.h>
#include "./raylib/src/raylib.h"
#include "./raylib/src/raymath.h"

static Texture2D loaded_texture;
static Image loaded_image;
static bool dragging;
static Vector2 mouse_anchor;
static Camera2D cam;

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900

#define ZOOM_SPEED 0.1f
#define ROT_DEGREES 90
#define BACKGROUND_COLOR (Color){ 25, 25, 25, 255 }

void handleInput(void);

int main( int argc, char * argv[] ) {

    if ( argc < 2 ) {
        fprintf( stderr, "Usage: %s [IMAGEPATH]\n", argv[0] );
        return 1;
    }

    if ( !FileExists( argv[1] ) ) {
        fprintf( stderr, "Could not find file '%s'\n", argv[1] );
        return 1;
    }

    if ( !IsFileExtension( argv[1], ".png" ) ) {
        fprintf( stderr, "File is not right format '%s'\n", argv[1] );
        return 1;
    }

    InitWindow( WIN_WIDTH, WIN_HEIGHT, "Coculus - Image Viewer" );
    loaded_image = LoadImage( argv[1] );
    cam.offset = (Vector2){ 0, 0 };
    cam.target = (Vector2){ 0, 0 };
    cam.zoom = 1.f;

    loaded_texture = LoadTextureFromImage( loaded_image );

    while( !WindowShouldClose() ) {
        handleInput();

        BeginDrawing();
            BeginMode2D( cam );

                ClearBackground( BACKGROUND_COLOR );
                DrawTexture( loaded_texture, 0, 0, WHITE );

            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void handleInput() {
    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        const Vector2 mouse_pos = GetMousePosition();
        if ( dragging ) {
            const Vector2 delta = Vector2Subtract( mouse_pos, mouse_anchor );
            cam.target = Vector2Subtract( cam.target, Vector2Scale( delta, 1.0f / cam.zoom ) );
            mouse_anchor = mouse_pos;
        } else {
            dragging = true;
            mouse_anchor = mouse_pos;
        }
    } else dragging = false;

    if ( GetMouseWheelMove() != 0 ) {
        if ( GetMouseWheelMove() < 0 ) {
            if ( cam.zoom - ZOOM_SPEED > 0 )
                cam.zoom += GetMouseWheelMove() * ZOOM_SPEED;
        } else {
            cam.zoom += GetMouseWheelMove() * ZOOM_SPEED;
        }
    }

    if ( IsKeyPressed( KEY_R ) ) {
        ImageRotate( &loaded_image, ROT_DEGREES );
        UnloadTexture( loaded_texture );
        loaded_texture = LoadTextureFromImage( loaded_image );
    }

    if ( IsKeyPressed( KEY_V ) ) {
        ImageFlipVertical( &loaded_image );
        UnloadTexture( loaded_texture );
        loaded_texture = LoadTextureFromImage( loaded_image );
    }

    if ( IsKeyPressed( KEY_H ) ) {
        ImageFlipHorizontal( &loaded_image );
        UnloadTexture( loaded_texture );
        loaded_texture = LoadTextureFromImage( loaded_image );
    }

    if ( IsKeyDown( KEY_LEFT_CONTROL ) && IsKeyPressed( KEY_S ) ) {
        ExportImage( loaded_image, "./out.png" );
    }
}

