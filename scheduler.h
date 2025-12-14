#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* task durumlari */
typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_SUSPENDED,
    TASK_TERMINATED
} TaskState_t;

/* pcb yapisi */
typedef struct {
    int id;                 // gorev id
    int arrivalTime;        // gelis zamani
    int burstTime;          // calisma suresi
    int priority;           // baslangic onceligi
    
    int remainingTime;      // kalan sure
    int currentPriority;    // guncel oncelik
    TaskState_t state;      // durum
    int startTime;          // baslama ani
    int finishTime;         // bitis ani (tablo icin gerekli)

    TaskHandle_t taskHandle;
    int color;              // renk
} PCB;

/* renkler */
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"

/* fonksiyonlar */
void vSchedulerInit(void);
void vSchedulerStart(void);

#endif