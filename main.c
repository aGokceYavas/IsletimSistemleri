#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "scheduler.h"

/* freertos hook fonksiyonlari */
void vApplicationTickHook( void )
{
    // her tickte (1ms) burasi calisir
}

void vApplicationIdleHook( void )
{
    // islemci bosta kalinca burasi calisir
}

void vApplicationMallocFailedHook( void )
{
    printf("HATA: Bellek yetersiz (malloc failed)\n");
    exit(1);
}

/* main fonksiyonu */
int main( void )
{
    printf("Sistem başlatılıyor...\n");

    // 1. scheduler hazirlik (listeyi oku vs)
    vSchedulerInit();

    // 2. simulasyonu baslat
    vSchedulerStart();

    return 0;
}