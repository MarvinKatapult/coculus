
#include <stdio.h>
#include "./raylib/src/raylib.h"
#include "./raylib/src/raymath.h"

static Texture2D loaded_texture;
static Image loaded_image;
static bool dragging;
static Vector2 image_offset;
static Vector2 mouse_anchor;
static Camera2D cam;

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900

#define ZOOM_SPEED 0.1f
#define BACKGROUND_COLOR (Color){ 25, 25, 25, 255 }

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
    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        if ( dragging ) {
            Vector2 mousePosition = GetMousePosition();
            Vector2 delta = Vector2Subtract( mousePosition, mouse_anchor );
            cam.target = Vector2Subtract( cam.target, Vector2Scale(delta, 1.0f / cam.zoom ) );
            mouse_anchor = mousePosition;
        } else {
            dragging = true;
            mouse_anchor = GetMousePosition();
        }
    } else dragging = false;

    if ( GetMouseWheelMove() != 0 ) {
        cam.zoom += GetMouseWheelMove() * ZOOM_SPEED;
        if ( cam.zoom < 0 ) cam.zoom = 0.1f;
    }

    if ( IsKeyPressed( KEY_R ) ) {
        ImageRotate( &loaded_image, 90 );
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
    loaded_image = LoadImage( argv[1] );
    cam.target = (Vector2){ (float)loaded_image.width / 2, (float)loaded_image.height / 2 };
    cam.offset = (Vector2){ 0, 0 };
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
