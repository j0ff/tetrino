/* ========================================================================== */
/*                          STC - SIMPLE TETRIS CLONE                         */
/* -------------------------------------------------------------------------- */
/*   Gamebuino platform implementation.                                       */
/*   Based on SDL game implementation.                                        */
/*                                                                            */
/*   Copyright (c) 2011 Laurens Rodriguez Oscanoa.                            */
/*   Copyright (c) 2014 Joffrey Carlier.                                      */
/*   This code is licensed under the MIT license:                             */
/*   http://www.opensource.org/licenses/mit-license.php                       */
/* -------------------------------------------------------------------------- */

#include "gb_platform.h"

namespace stc
{

// Encoded bitmaps 
const byte bitmapScoreTitle[] PROGMEM = {8,32,0x4c,0xda,0xfe,0xfe,0x76,0x0,0x7c,0xfa,0xc6,0xc6,0x82,0x0,0x7c,0xfa,0xc6,0xc6,0xfe,0x7c,0x0,0x7c,0xfa,0xfe,0x32,0x72,0xfe,0xdc,0x0,0x7c,0xfa,0xd6,0xc6,0x44,};
const byte bitmapNextTitle[] PROGMEM = {16,15,0xc6,0x7c,0xea,0xfa,0x7e,0x7c,0x7c,0x10,0xee,0x7c,0xc6,0xfe,0x0,0x7c,0x4,0x0,0xa,0x7c,0xe,0xfa,0xfe,0xd6,0xfe,0xd6,0xe,0xc6,0xe,0x44,0x4,0x0,};
const byte bitmapLevelTitle[] PROGMEM = {16,15,0x0,0x7c,0x38,0xfa,0xe0,0xfe,0xe0,0xe0,0x38,0xe0,0x0,0x60,0x0,0x0,0x70,0x70,0xa8,0xa8,0xa8,0xa8,0x90,0x90,0x0,0x0,0xc2,0x0,0xfe,0x0,0x80,0x0,};
const byte bitmapGameOver[] PROGMEM = {16,24,0xf1,0xe0,0xfd,0xf8,0xc5,0x8,0xc5,0x48,0xfd,0xd8,0x0,0x0,0x3d,0xe0,0x41,0xf8,0x80,0x48,0xf0,0x78,0x7d,0xe0,0x0,0x0,0xf1,0xe0,0xfd,0xf8,0x94,0x8,0x95,0xf8,0x84,0x8,0x1,0xf8,0x0,0x0,0xfd,0xe0,0xf5,0xf8,0x15,0x28,0x1d,0x28,0xf1,0x8,};
const byte bitmap0[] PROGMEM = {8,6,0x7c,0xfa,0x86,0x86,0xfe,0x7c,};
const byte bitmap1[] PROGMEM = {8,6,0x4,0xa,0xe,0xfe,0xfe,0x7c,};
const byte bitmap2[] PROGMEM = {8,6,0xc4,0xe2,0xf6,0xfe,0xde,0xcc,};
const byte bitmap3[] PROGMEM = {8,6,0x44,0xc2,0xd6,0xd6,0xfe,0x7c,};
const byte bitmap4[] PROGMEM = {8,6,0x1c,0x1a,0x18,0xfe,0xfe,0x18,};
const byte bitmap5[] PROGMEM = {8,6,0xdc,0xda,0xde,0xd6,0xf6,0x64,};
const byte bitmap6[] PROGMEM = {8,6,0xfc,0xfa,0x9e,0x96,0xf6,0x64,};
const byte bitmap7[] PROGMEM = {8,6,0x4,0x2,0xe6,0xf6,0x1e,0xe,};
const byte bitmap8[] PROGMEM = {8,6,0x7c,0xfa,0xd6,0xd6,0xfe,0x7c,};
const byte bitmap9[] PROGMEM = {8,6,0x1c,0xda,0xd6,0xd6,0xfe,0x7c,};
const byte* spritesNumbers[10] = {bitmap0, bitmap1, bitmap2, bitmap3, bitmap4, bitmap5, bitmap6, bitmap7, bitmap8, bitmap9};

// Sound effects
// Generated with nice tool from yodasvideoarcade: http://gamebuino.com/forum/viewtopic.php?f=17&t=1018
const int soundfx[4][8] =
{
  {0,45,26,1,0,1,7,10}, // sound 0
  {0,33,53,1,0,5,7,3},  // sound 1
  {0,14,46,6,0,0,7,11}, // sound 2
  {1,1,0,0,0,0,7,2},    // sound 3
};

// Initializes platform, if there are no problems returns ERROR_NONE.
int PlatformGB::init(Game *game)
{
     // Initialize the random number generator
     mGb->pickRandomSeed();
    
    // Initialize button states
    for (int i = 0; i < NUM_BTN; i++)
    {
        mButtonStates[i] = 0;
    }
    
    // Initialise game
    mGame = game;
    playedGameOverSnd = false;
    
    // Don't show batery indicator
    mGb->battery.show = false;
    
    return Game::ERROR_NONE;
}

// Return the current system time in milliseconds
long PlatformGB::getSystemTime()
{
  return millis();
}

// Polls for button press & release events
// Returns true as soon as a button event found or false when no new button event found
// Function should be called until it returns False.
boolean PlatformGB::pollButtonEvent(GB_ButtonEvent* event)
{
    static uint8_t index = 0;
    boolean isPressed, isReleased;
    boolean retVal = false;
    
    // Poll all buttons
    for( ; index < NUM_BTN; index++)
    {    
        isPressed = mGb->buttons.pressed(index);
        isReleased = mGb->buttons.released(index);
        if ( isPressed && 1 != mButtonStates[index] )
        {
            // Button press event
            mButtonStates[index] = 1;
            event->type = GB_KEYDOWN;
            retVal = true;
        }
        else if (isReleased && 0 != mButtonStates[index] )
        {
            // Button release event
            mButtonStates[index] = 0;
            event->type = GB_KEYUP;
            retVal = true;
        }
        if (retVal)
        {
            // Return event
            event->key = index;
            return true;
        }
    }
    // Reached this far, no event found
    index = 0;
    event->type = GB_NOEVENT;
    event->key = 0; // Don't care 
    return false;
}

// Process events and notify game
void PlatformGB::processEvents()
{
    GB_ButtonEvent event;

    // Grab events in the queue
    while (pollButtonEvent(&event))
    {
        switch (event.type)
        {
        // On key pressed
        case GB_KEYDOWN:
            switch (event.key)
            {
            case BTN_B:
                mGame->onEventStart(Game::EVENT_QUIT);
                break;
            // Rotated Gamebuino: LEFT->DOWN, UP->LEFT, DOWN->RIGHT  
            case BTN_LEFT:
                mGame->onEventStart(Game::EVENT_MOVE_DOWN);
                break;
            case BTN_C:
                mGame->onEventStart(Game::EVENT_ROTATE_CW);
                if ( !mGame->isOver() )
                {
                    playSoundFX(SND_FX_ROTATE, SND_FX_CHANNEL_1);
                }
                break;
            case BTN_UP:
                mGame->onEventStart(Game::EVENT_MOVE_LEFT);
                break;
            case BTN_DOWN:
                mGame->onEventStart(Game::EVENT_MOVE_RIGHT);
                break;
            case BTN_RIGHT:
                mGame->onEventStart(Game::EVENT_DROP);
                break;
//            case SDLK_F5:
//                mGame->onEventStart(Game::EVENT_RESTART);
//                break;
            case BTN_A:
                mGame->onEventStart(Game::EVENT_PAUSE);
                break;
//            case SDLK_F2:
//                mGame->onEventStart(Game::EVENT_SHOW_NEXT);
//                break;
#ifdef STC_SHOW_GHOST_PIECE
            case SDLK_F3:
                mGame->onEventStart(Game::EVENT_SHOW_SHADOW);
                break;
#endif // STC_SHOW_GHOST_PIECE
            default:
                break;
            }
            break;
        // On key released
        case GB_KEYUP:
            switch (event.key)
            {
            case BTN_LEFT:
                mGame->onEventEnd(Game::EVENT_MOVE_DOWN);
                break;
            case BTN_UP:
                mGame->onEventEnd(Game::EVENT_MOVE_LEFT);
                break;
            case BTN_DOWN:
                mGame->onEventEnd(Game::EVENT_MOVE_RIGHT);
                break;
#ifdef STC_AUTO_ROTATION
            case SDLK_w:
            case SDLK_UP:
                mGame->onEventEnd(Game::EVENT_ROTATE_CW);
                break;
#endif // STC_AUTO_ROTATION
            default:
                break;
            }
        default:
            break;
        }
    }
}

// Play Gambuino sound effect
void PlatformGB::playSoundFX(int fxno, int channel)
{
  mGb->sound.command(0,soundfx[fxno][6],0,channel); // set volume
  mGb->sound.command(1,soundfx[fxno][0],0,channel); // set waveform
  mGb->sound.command(2,soundfx[fxno][5],-soundfx[fxno][4],channel); // set volume slide
  mGb->sound.command(3,soundfx[fxno][3],soundfx[fxno][2]-58,channel); // set pitch slide
  mGb->sound.playNote(soundfx[fxno][1],soundfx[fxno][7],channel); // play note
}

// Draw a tile from a tetromino
// TODO shadow not supported
void PlatformGB::drawTile(int x, int y, int tile, bool shadow)
{
    // tile type not used as we only have one type of tile
    mGb->display.drawRect(x, y, TILE_SIZE, TILE_SIZE);
}

// Draw a number on the given position
void PlatformGB::drawNumber(int x, int y, long number, int length, int color)
{
    int pos = 0;
    int dy;
    
    do
    {
        dy = y + NUMBER_WIDTH * (length - 1 - pos);
        mGb->display.drawBitmap(x, dy, spritesNumbers[number % 10]);
        number /= 10;
    } while (++pos < length);
}

// Render the state of the game using platform functions
void PlatformGB::renderGame()
{
    int i, j;
    
    // Check if the game state has changed, if so redraw
    if (mGame->hasChanged())
    {
        // Gamebuino library bug? battery show false overriden... so we must enforce it...
        mGb->battery.show = false;

        // Draw background
        mGb->display.drawFastVLine(0, 1, GRID_WIDTH);
        mGb->display.drawFastHLine(0, 0, GRID_HEIGHT);
        mGb->display.drawFastHLine(0, GRID_WIDTH+1, GRID_HEIGHT);
        // Draw Score title
        mGb->display.drawBitmap(SCORE_TITLE_X, SCORE_TITLE_Y, bitmapScoreTitle);
        // Draw Next title
        mGb->display.drawBitmap(NEXT_TITLE_X, NEXT_TITLE_Y, bitmapNextTitle);
        // Draw Level title
        mGb->display.drawBitmap(LEVEL_TITLE_X, LEVEL_TITLE_Y, bitmapLevelTitle);

        // Draw preview block
        if (mGame->showPreview())
        {
            for (i = 0; i < Game::TETROMINO_SIZE; ++i)
            {
                for (j = 0; j < Game::TETROMINO_SIZE; ++j)
                {
                    if (mGame->nextBlock().cells[i][j] != Game::EMPTY_CELL)
                    {
                        // Rotated Gamebuino, we use following transform: y -> x and x -> TETROMINO_SIZE - y
                        drawTile(PREVIEW_X + (TILE_SIZE * (Game::TETROMINO_SIZE - j)),
                                 PREVIEW_Y + (TILE_SIZE * i),
                                 mGame->nextBlock().cells[i][j], false);
                    }
                }
            }
        }
        
#ifdef STC_SHOW_GHOST_PIECE
        // Draw shadow tetromino
        if (mGame->showShadow() && mGame->shadowGap() > 0)
        {
            for (i = 0; i < Game::TETROMINO_SIZE; ++i)
            {
                for (j = 0; j < Game::TETROMINO_SIZE; ++j)
                {
                    if (mGame->fallingBlock().cells[i][j] != Game::EMPTY_CELL)
                    {
                        drawTile(BOARD_X + (TILE_SIZE * (mGame->fallingBlock().x + i)),
                                 BOARD_Y + (TILE_SIZE * (mGame->fallingBlock().y + mGame->shadowGap() + j)),
                                 mGame->fallingBlock().cells[i][j], true);
                    }
                }
            }
        }
#endif

        // Draw the cells in the board
        for (i = 0; i < Game::BOARD_TILEMAP_WIDTH; ++i)
        {
            for (j = 0; j < Game::BOARD_TILEMAP_HEIGHT; ++j)
            {
                if (mGame->getCell(i, j) != Game::EMPTY_CELL)
                {
                    drawTile(BOARD_X - (TILE_SIZE * (j + 1)),
                             BOARD_Y + (TILE_SIZE * i),
                             mGame->fallingBlock().cells[i][j], false);
                }
            }
        }

        // Draw falling tetromino
        for (i = 0; i < Game::TETROMINO_SIZE; ++i)
        {
            for (j = 0; j < Game::TETROMINO_SIZE; ++j)
            {
                if (mGame->fallingBlock().cells[i][j] != Game::EMPTY_CELL)
                {
                    drawTile(BOARD_X - (TILE_SIZE * (mGame->fallingBlock().y + j + 1)),
                             BOARD_Y + (TILE_SIZE * (mGame->fallingBlock().x + i)),
                             mGame->fallingBlock().cells[i][j], false);
                }
            }
        }


        // Draw game statistic data
        if (!mGame->isPaused())
        {
            drawNumber(LEVEL_X, LEVEL_Y, mGame->stats().level, LEVEL_LENGTH, Game::COLOR_WHITE);
//            drawNumber(LINES_X, LINES_Y, mGame->stats().lines, LINES_LENGTH, Game::COLOR_WHITE);
            drawNumber(SCORE_X, SCORE_Y, mGame->stats().score, SCORE_LENGTH, Game::COLOR_WHITE);

//            drawNumber(TETROMINO_X, TETROMINO_L_Y, mGame->stats().pieces[Game::TETROMINO_L], TETROMINO_LENGTH, Game::COLOR_ORANGE);
//            drawNumber(TETROMINO_X, TETROMINO_I_Y, mGame->stats().pieces[Game::TETROMINO_I], TETROMINO_LENGTH, Game::COLOR_CYAN);
//            drawNumber(TETROMINO_X, TETROMINO_T_Y, mGame->stats().pieces[Game::TETROMINO_T], TETROMINO_LENGTH, Game::COLOR_PURPLE);
//            drawNumber(TETROMINO_X, TETROMINO_S_Y, mGame->stats().pieces[Game::TETROMINO_S], TETROMINO_LENGTH, Game::COLOR_GREEN);
//            drawNumber(TETROMINO_X, TETROMINO_Z_Y, mGame->stats().pieces[Game::TETROMINO_Z], TETROMINO_LENGTH, Game::COLOR_RED);
//            drawNumber(TETROMINO_X, TETROMINO_O_Y, mGame->stats().pieces[Game::TETROMINO_O], TETROMINO_LENGTH, Game::COLOR_YELLOW);
//            drawNumber(TETROMINO_X, TETROMINO_J_Y, mGame->stats().pieces[Game::TETROMINO_J], TETROMINO_LENGTH, Game::COLOR_BLUE);

//            drawNumber(PIECES_X, PIECES_Y, mGame->stats().totalPieces, PIECES_LENGTH, Game::COLOR_WHITE);
        }

        // Is game over?
        if ( mGame->isOver() )
        {
            if (!playedGameOverSnd)
            {
                playSoundFX(SND_FX_GAME_OVER, SND_FX_CHANNEL_1);
                playedGameOverSnd = true;
            }
            // Draw game over bitmap
            mGb->display.setColor(WHITE); 
            mGb->display.fillRect(GAME_OVER_TITLE_X-3, GAME_OVER_TITLE_Y-3, GAME_OVER_TITLE_WIDTH, GAME_OVER_TITLE_HEIGHT);
            mGb->display.setColor(BLACK,WHITE); 
            mGb->display.drawBitmap(GAME_OVER_TITLE_X, GAME_OVER_TITLE_Y, bitmapGameOver);
        }
        
        // Inform the game that we are done with the changed state
//        mGame->onChangeProcessed();
    }
}

// Return a random positive integer number
int PlatformGB::random()
{
  return (int) ::random(0, GB_MAX_INT - 1);
}

void PlatformGB::onLineCompleted()
{
    playSoundFX(SND_FX_LINE_COMPLETED, SND_FX_CHANNEL_2);
}

void PlatformGB::onPieceDrop()
{
    playSoundFX(SND_FX_PIECE_DROP, SND_FX_CHANNEL_1);
}

// Release platform allocated resources
void PlatformGB::end()
{
    // Show batery indicator
    mGb->battery.show = true;

  // No resource to release on Gamebuino
}
}
