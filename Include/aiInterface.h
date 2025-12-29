#ifndef AIINTERFACE_H
#define AIINTERFACE_H
#include <stdint.h>

#ifdef AIINTERFACE_EXPORTS
#define AI_API __declspec(dllexport)
#else
#define AI_API __declspec(dllimport)
#endif

extern "C" {

struct AIMove {
    int8_t fromX, fromY;
    int8_t toX, toY;
};

struct AIBoard {
    int8_t cells[64];   
    int8_t sideToMove;  
};

struct AIMoveList {
    const AIMove* moves;  
    int32_t count;
};

AI_API int chooseMoveFromList(const AIBoard* board, const AIMoveList* list, AIMove* outMove);
AI_API int chooseMoveFromListAggressive(const AIBoard* board, const AIMoveList* list, AIMove* outMove);

}


#endif