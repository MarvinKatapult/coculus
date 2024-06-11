
#include <stdio.h>
#include "./raylib/src/raylib.h"
#include "./raylib/src/raymath.h"

static Texture2D loaded_texture;
static bool dragging;
static Vector2 image_offset;
static Vector2 mouse_anchor;
static Camera2D cam;

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900

bool checkFilePath( const char * fpath ) {
    FILE * fp = fopen( fpath, "r" );
    bool ret = false;
    if ( fp ) {
        ret = true;
        fclose( fp );
    }
    return ret;
}

Vector2 getDraggingTextPos() {
    return Vector2Subtract( Vector2Scale( Vector2Add( image_offset, GetMousePosition() ), 1/cam.zoom ), mouse_anchor );
}

void handleInput() {
    if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) ) {
        dragging = true;
        mouse_anchor = Vector2Scale( GetMousePosition(), 1/cam.zoom );
    }
    if ( IsMouseButtonReleased( MOUSE_BUTTON_RIGHT ) ) {
        dragging = false;
        image_offset = getDraggingTextPos();
    }

    if ( GetMouseWheelMove() > 0 || ( GetMouseWheelMove() < 0 && cam.zoom - 0.1f > 0 ) ) cam.zoom += (float)GetMouseWheelMove() * 0.1f;

    if ( IsKeyPressed( KEY_R ) ) {
        Image img = LoadImageFromTexture( loaded_texture );
        ImageRotate( &img, 90 );
        loaded_texture = LoadTextureFromImage( img );
        UnloadImage( img );
    }

    if ( IsKeyPressed( KEY_V ) ) {
        Image img = LoadImageFromTexture( loaded_texture );
        ImageFlipVertical( &img );
        loaded_texture = LoadTextureFromImage( img );
        UnloadImage( img );
    }

    if ( IsKeyPressed( KEY_H ) ) {
        Image img = LoadImageFromTexture( loaded_texture );
        ImageFlipHorizontal( &img );
        loaded_texture = LoadTextureFromImage( img );
        UnloadImage( img );
    }
}

int main( int argc, char * argv[] ) {

    if ( argc < 2 ) {
        fprintf( stderr, "Usage: %s [IMAGEPATH]\n", argv[0] );
        return 1;
    }

    if ( !checkFilePath( argv[1] ) ) {
        fprintf( stderr, "Could not find file '%s'\n", argv[1] );
        return 1;
    }

    InitWindow( WIN_WIDTH, WIN_HEIGHT, "Coculus - Image Viewer" );
    Image loaded_image = LoadImage( argv[1] );
    cam.offset = (Vector2){ WIN_WIDTH/2 - loaded_image.width/2, WIN_HEIGHT/2 - loaded_image.height/2 };
    cam.zoom = 1.f;

    loaded_texture = LoadTextureFromImage( loaded_image );
    UnloadImage( loaded_image );

    Vector2 text_pos = { 0 };
    while( !WindowShouldClose() ) {
        handleInput();
        if ( dragging ) {
            text_pos = getDraggingTextPos();
        } else {
            text_pos = image_offset;
        }

        BeginDrawing();
            BeginMode2D( cam );

                ClearBackground( (Color){ 25, 25, 25, 255 } );
                DrawTextureV( loaded_texture, text_pos, WHITE );

            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
