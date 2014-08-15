/* ========================================================================== */
/*                          STC - SIMPLE TETRIS CLONE                         */
/* -------------------------------------------------------------------------- */
/*   Constants and definitions for Gamebuino implementation.                  */
/*                                                                            */
/*   Copyright (c) 2011 Laurens Rodriguez Oscanoa.                            */
/*   Copyright (c) 2014 Joffrey Carlier.                                      */
/*   This code is licensed under the MIT license:                             */
/*   http://www.opensource.org/licenses/mit-license.php                       */
/* -------------------------------------------------------------------------- */

#include "game.h"
#include <Gamebuino.h>

#ifndef STC_GB_PLATFORM_HPP_
#define STC_GB_PLATFORM_HPP_

namespace stc
{

#define GB_MAX_INT 32767

// Gamebuina keyboard event types
enum GB_EventType
{
  GB_NOEVENT = 0,
  GB_KEYDOWN,
  GB_KEYUP,
};
  
// Gamebuino keyboard event structure
typedef struct
{
  GB_EventType type;
  uint8_t key;
} GB_ButtonEvent;

// Gamebuino platform implementation
class PlatformGB : public Platform
{

    // UI layout (quantities are expressed in pixels)

    // Screen size for Gamebuino in Portrait mode
    static const int SCREEN_WIDTH  = LCDHEIGHT;
    static const int SCREEN_HEIGHT = LCDWIDTH;

    // Size of square tile
    static const int TILE_SIZE = 3;
    
    // Board up-left corner coordinates
    static const int BOARD_X = TILE_SIZE*Game::BOARD_TILEMAP_HEIGHT + 1;
    static const int BOARD_Y = 1;

    // Board down-right corner
    static const int BOARD_Y2 = BOARD_Y + TILE_SIZE*Game::BOARD_TILEMAP_WIDTH;

    // Background grid width and height
    static const int GRID_HEIGHT = (Game::BOARD_TILEMAP_HEIGHT-2)*TILE_SIZE;
    static const int GRID_WIDTH  = Game::BOARD_TILEMAP_WIDTH*TILE_SIZE;

    // Score Title up-left corner coordinates
    static const int SCORE_TITLE_X = LCDWIDTH + 1 - 8; // 8 = title height
    static const int SCORE_TITLE_Y = 0;
    
    // Next Title up-left corner coordinates
    static const int NEXT_TITLE_X = BOARD_X - 10 - 16; // 16 = title height
    static const int NEXT_TITLE_Y = BOARD_Y2 + 2;

    // Preview tetromino position
    static const int PREVIEW_X = NEXT_TITLE_X - 20;
    static const int PREVIEW_Y = BOARD_Y2 + ( (SCREEN_WIDTH - BOARD_Y2 - TILE_SIZE*Game::TETROMINO_SIZE + 1) >> 1);

    // Size of number
    // TODO: redraw smaller number bitmaps to increase displayed score & level length? 
    static const int NUMBER_WIDTH  = 7;
    static const int NUMBER_HEIGHT = 9;

    // Score position and length on screen
    static const int SCORE_X      = SCORE_TITLE_X - NUMBER_WIDTH - 1;
    static const int SCORE_Y      = SCORE_TITLE_Y;
    // TODO: redraw smaller number bitmaps to increase score length? 
    static const int SCORE_LENGTH = 7;

    // Level Title up-left corner coordinates
    static const int LEVEL_TITLE_X = NUMBER_HEIGHT + 1;
    static const int LEVEL_TITLE_Y = NEXT_TITLE_Y;
    
    // Level position and length on screen
    static const int LEVEL_X      = 1;
    static const int LEVEL_Y      = NEXT_TITLE_Y;
    static const int LEVEL_LENGTH = 2;
    
    // Game over title up-left corner coordinates
    static const int GAME_OVER_TITLE_X      = 30;
    static const int GAME_OVER_TITLE_Y      = 4;
    static const int GAME_OVER_TITLE_WIDTH  = 20;
    static const int GAME_OVER_TITLE_HEIGHT = 30;
    
    // Sound effects
    static const int SND_FX_LINE_COMPLETED = 0;
    static const int SND_FX_ROTATE         = 1;
    static const int SND_FX_GAME_OVER      = 2;
    static const int SND_FX_PIECE_DROP     = 3;
    static const int SND_FX_CHANNEL_1      = 0;
    static const int SND_FX_CHANNEL_2      = 1;
public:

    // Initializes platform
    virtual int init(Game *game);

    // Clear resources used by platform
    virtual void end();

    // Read input device and notify game
    virtual void processEvents();

    // Render the state of the game
    virtual void renderGame();

    // Return the current system time in milliseconds
    virtual long getSystemTime();

    // Return a random positive integer number
    virtual int random();

    // Events
    virtual void onLineCompleted();
    virtual void onPieceDrop();
    
    // We need a reference to Gamebuino object
    void setGamebuinoRef(Gamebuino* gb){mGb = gb;}

private:

    Game* mGame;
    Gamebuino* mGb;

    // Stores buttons state, 1 = pressed, 0 = released
    uint8_t mButtonStates[NUM_BTN];
    
    // Only play game over sound FX once
    bool playedGameOverSnd;
    
    void drawTile(int x, int y, int tile, bool shadow);
    void drawNumber(int x, int y, long number, int length, int color);
    boolean pollButtonEvent(GB_ButtonEvent* event);
    void playSoundFX(int fxno, int channel);
};
}

#endif // STC_GB_PLATFORM_HPP_

