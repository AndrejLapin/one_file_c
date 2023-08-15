#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

struct vec2 
{
    int x;
    int y;
};

#define Vec2(name,xc,yc) struct vec2 name; name.x = xc; name.y = yc;
#define R(r) rand() % r

struct buffer 
{
    struct vec2 dimensions;
    char* data;
};

void initBuffer(struct buffer* writeBuff, int width, int height)
{
    writeBuff->dimensions.x = width;
    writeBuff->dimensions.y = height;
    writeBuff->data = malloc(width * height * sizeof(char));
}

void fillBuffer(struct buffer* writeBuff, char data)
{
    int size = writeBuff->dimensions.x * writeBuff->dimensions.y;
    for(int i = 0; i < size; i++)
    {
        writeBuff->data[i] = data;
    }
}

void writeToBuffer(struct buffer* outBuffer, struct buffer* inBuffer, int xOffset, int yOffset)
{
    for(int y = 0; y < inBuffer->dimensions.y; y++)
    {
        int outY = y + yOffset;
        if(outY < 0 || outY >= outBuffer->dimensions.y)
        continue;

        for (int x = 0; x < inBuffer->dimensions.x; x++)
        {
            if(inBuffer->data[y * inBuffer->dimensions.x + x] == ' ')
            continue;

            int outX = x + xOffset;
            if(outX < 0 || outX >= outBuffer->dimensions.x)
            continue;

            outBuffer->data[outY * outBuffer->dimensions.x + outX] = inBuffer->data[y * inBuffer->dimensions.x + x];
        }
    }
}

void drawBuffer(struct buffer* outBuff)
{
    clear();
    for(int y = 0; y < outBuff->dimensions.y; y++)
    {
        for (int x = 0; x < outBuff->dimensions.x; x++)
        {
            addch(outBuff->data[y * outBuff->dimensions.x + x]);
        }
        addch('\n');

    }
    refresh();
}

void clearBuffer(struct buffer* outBuff)
{
    fillBuffer(outBuff, ' ');
}

void drawLine(struct buffer* outBuff, struct vec2* pointA, struct vec2* pointB, char symbol)
{
    int steps;
    double incX, incY;
    {
        int dx = abs(pointA->x - pointB->x);
        int dy = abs(pointA->y - pointB->y);
        if(dx > dy)
            steps = dx;
        else
            steps = dy;
        incX = dx * 1.0 / steps;
        incY = dy * 1.0 / steps;
    }

    double x = pointA->x;
    double y = pointA->y;
    for(int step = 0; step <= steps; step++)
    {
        if(x < outBuff->dimensions.x && y < outBuff->dimensions.y)
        {
            int drawIndex = round(y) * outBuff->dimensions.x + round(x);
            outBuff->data[drawIndex] = symbol; 
        }
        x += incX;
        y += incY;
    }
}

void moveBuff(struct buffer* outBuff, struct vec2* move)
{
    int size = outBuff->dimensions.x * outBuff->dimensions.y;
    memcpy(outBuff->data, outBuff->data - (outBuff->dimensions.x * move->y) - move->x, size);
    int y = move->y > 0 ? 0 : outBuff->dimensions.y + move->y;
    int yEnd = move->y > 0 ? move->y : outBuff->dimensions.y; 
    
    // should be just a one dimensional for loop that goes through entire buffer and fills with ' ' using a condition
    for(; y < yEnd; ++y)
    {
        for(int x = 0; x < outBuff->dimensions.x; ++x)
        {
            outBuff->data[y * outBuff->dimensions.x + x] = ' ';
        }
    }

    int xStart = move->x > 0 ? 0 : outBuff->dimensions.x + move->x;
    int xEnd = move->x > 0 ? move->x : outBuff->dimensions.x;
    for(int y = 0; y < outBuff->dimensions.y; ++y)
    {
        for(int x = xStart; x < xEnd; ++x)
        {
            outBuff->data[y *outBuff->dimensions.x + x] = ' ';
        }
    }
}

struct inputModif
{
    bool lock;
    struct vec2 playerPos;
    bool run;
};

void lock(struct inputModif* modif)
{
    while(modif->lock)
    {
        usleep(1000);
    }
    modif->lock = true;
}

void unlock(struct inputModif* modif)
{
    modif->lock = false;
}

void* processInput(void* data)
{
    bool run = true;
    char in;
    struct inputModif* modif = (struct inputModif*)data;
    while(run)
    {
        in = getch();

        if(in == 'w')
        {
            lock(modif);
            modif->playerPos.y -= 1;
            unlock(modif);
        }
        else if (in == 's')
        {
            lock(modif);
            modif->playerPos.y += 1;
            unlock(modif);
        }
        else if (in == 'a')
        {
            lock(modif);
            modif->playerPos.x -= 1;
            unlock(modif);
        }
        else if (in == 'd')
        {
            lock(modif);
            modif->playerPos.x += 1;
            unlock(modif);
        }
        else if(in == ':')
        {
            run = false;
            lock(modif);
            modif->run = 0;
            unlock(modif);
        }
    }
    return NULL;
}

const int c_width = 80;
const int c_height = 24;

int main()
{
    int run = 1;
    char in;

    struct buffer frameBuffer;
    initBuffer(&frameBuffer, c_width, c_height);
    struct buffer bckgTxt;
    initBuffer(&bckgTxt, c_width, c_height);
    fillBuffer(&bckgTxt, '.');
    struct buffer uiBuff;
    initBuffer(&uiBuff, c_width, c_height);
    clearBuffer(&uiBuff);
    {
        Vec2(pointA, 0, 0);
        Vec2(pointB, c_width, 0);
        drawLine(&uiBuff, &pointA, &pointB, '=');
    }
    {
        Vec2(pointA, 0, c_height - 1);
        Vec2(pointB, c_width, c_height - 1);
        drawLine(&uiBuff, &pointA, &pointB, '=');
    }
    {
        Vec2(pointA, 0, 0);
        Vec2(pointB, 0, c_height);
        drawLine(&uiBuff, &pointA, &pointB, '|');
    }
    {
        Vec2(pointA, c_width - 1, 0);
        Vec2(pointB, c_width - 1, c_height);
        drawLine(&uiBuff, &pointA, &pointB, '|');
    }
    struct buffer rainBuff;
    initBuffer(&rainBuff, c_width, c_height);
    clearBuffer(&rainBuff);

    struct buffer coolLine;
    initBuffer(&coolLine, c_width, c_height);
    clearBuffer(&coolLine);
    Vec2(coolLineStart, 0, 0);
    Vec2(coolLineEnd, c_width, 0);
    bool decrease = true;

    struct buffer bottle;
    initBuffer(&bottle, 6, 6);
    bottle.data = "  ||  __||__|____|| XX ||----||____|";

    struct inputModif inputModif;
    inputModif.lock = false;
    inputModif.playerPos.x = 5;
    inputModif.playerPos.y = 5;
    inputModif.run = true;

    initscr();

    pthread_t inputThread;
    pthread_create(&inputThread, NULL, processInput, &inputModif);
    while(run)
    {
        clearBuffer(&frameBuffer);
        //writeToBuffer(&frameBuffer, &bckgTxt, 0, 0);

        lock(&inputModif);
        if(!inputModif.run)
        {
            run = 0;
        }
        writeToBuffer(&frameBuffer, &bottle, inputModif.playerPos.x, inputModif.playerPos.y);
        unlock(&inputModif);

        writeToBuffer(&frameBuffer, &rainBuff, 0, 0);
        writeToBuffer(&frameBuffer, &uiBuff, 0, 0);
        writeToBuffer(&frameBuffer, &coolLine, 0, 0);
        drawBuffer(&frameBuffer);

        clearBuffer(&coolLine);
        if(decrease)
        {
            if(coolLineEnd.y >= (c_height - 1))
            {
                --coolLineEnd.x;
                if(coolLineEnd.x <= 0)
                {
                    decrease = false;
                }
            }
            else
            {
                ++coolLineEnd.y;
            }
        }
        else
        {
            if(coolLineEnd.x >= (c_width - 1))
            {
                --coolLineEnd.y;
                if(coolLineEnd.y <= 0)
                {
                    decrease = true;
                }
            }
            else
            {
                ++coolLineEnd.x;
            }
        }
        drawLine(&coolLine, &coolLineStart, &coolLineEnd, '.');
        coolLine.data[coolLineEnd.y * coolLine.dimensions.x + coolLineEnd.x] = 'X';

        {
            int rnd = R((rainBuff.dimensions.x + rainBuff.dimensions.y));
            rainBuff.data[((rnd <= rainBuff.dimensions.y) * rnd * rainBuff.dimensions.x) + (rnd > rainBuff.dimensions.y) * (rnd - rainBuff.dimensions.y)] = '\\';
        }
        {
            int rnd = R((rainBuff.dimensions.x + rainBuff.dimensions.y));
            rainBuff.data[((rnd <= rainBuff.dimensions.y) * rnd * rainBuff.dimensions.x) + (rnd > rainBuff.dimensions.y) * (rnd - rainBuff.dimensions.y)] = '\\';
        }
        {
            Vec2(mv, 1, 1);
            moveBuff(&rainBuff, &mv);
        }
        usleep(20000);
    }
    endwin();
    pthread_join(inputThread, NULL);
    return 0;
}