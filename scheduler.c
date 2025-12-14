#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- global degiskenler --- */
PCB *jobList = NULL;    // is listesi
int jobCount = 0;       // toplam is
int currentTime = 0;    // zaman sayaci

/* rr icin son indeks */
int lastRRIndex = -1;

/* rastgele renk */
int getRandomColor() {
    static int colorIndex = 0;
    int colors[] = {31, 32, 33, 34, 35, 36};
    int selected = colors[colorIndex % 6];
    colorIndex++;
    return selected;
}

/* --- tablo yazdirma fonksiyonu --- */
void printPerformanceTable() {
    printf("\n\n");
    printf("=========================================================================\n");
    printf("| ID  | Varış | Süre  | Bitiş | Geçen Süre (TAT) | Bekleme Süresi (WT) |\n");
    printf("=========================================================================\n");

    double totalTAT = 0;
    double totalWT = 0;

    for (int i = 0; i < jobCount; i++) {
        // hesaplamalar
        // tat = bitis - varis
        // wt = tat - sure
        int tat = jobList[i].finishTime - jobList[i].arrivalTime;
        int wt = tat - jobList[i].burstTime;

        // eksi deger cikarsa (zaman asimi vs) 0 yapalim goruntu bozulmasin
        if (tat < 0) tat = 0;
        if (wt < 0) wt = 0;

        totalTAT += tat;
        totalWT += wt;

        printf("| %03d | %-5d | %-5d | %-5d | %-16d | %-19d |\n", 
               jobList[i].id, 
               jobList[i].arrivalTime, 
               jobList[i].burstTime, 
               jobList[i].finishTime, 
               tat, 
               wt);
    }
    printf("=========================================================================\n");
    // ortalamalari yazdir
    printf("Ortalama Geçen Süre (Avg TAT): %.2f sn\n", totalTAT / jobCount);
    printf("Ortalama Bekleme Süresi (Avg WT): %.2f sn\n", totalWT / jobCount);
    printf("=========================================================================\n");
}

/* --- dosya okuma --- */
void vLoadJobsFromFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya hatası");
        exit(1);
    }

    char line[100];
    int capacity = 10;
    jobList = (PCB*)malloc(sizeof(PCB) * capacity);
    int id_counter = 0;

    printf("Dosya okunuyor: %s\n", filename);
    printf("--------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        if (jobCount >= capacity) {
            capacity *= 2;
            jobList = (PCB*)realloc(jobList, sizeof(PCB) * capacity);
        }

        int arrival, priority, burst;
        if (sscanf(line, "%d, %d, %d", &arrival, &priority, &burst) == 3) {
            jobList[jobCount].id = id_counter++;
            jobList[jobCount].arrivalTime = arrival;
            jobList[jobCount].priority = priority;
            jobList[jobCount].burstTime = burst;
            
            jobList[jobCount].remainingTime = burst;
            jobList[jobCount].currentPriority = priority;
            jobList[jobCount].state = TASK_READY;
            jobList[jobCount].startTime = -1;
            jobList[jobCount].finishTime = 0; // baslangic sifir
            jobList[jobCount].color = getRandomColor();
            
            printf("Yüklendi -> ID:%04d Varış:%d Öncelik:%d Süre:%d\n", 
                   jobList[jobCount].id, arrival, priority, burst);
            
            jobCount++;
        }
    }
    fclose(file);
    printf("--------------------------------\n");
    printf("Toplam %d görev sisteme alındı.\n\n", jobCount);
}

/* --- scheduler secim mantigi --- */
int findNextJob() {
    // 1. rt gorevler (fcfs)
    for (int i = 0; i < jobCount; i++) {
        if (jobList[i].arrivalTime <= currentTime && 
            jobList[i].state != TASK_TERMINATED && 
            jobList[i].currentPriority == 0) {
            return i;
        }
    }

    // 2. kullanici gorevleri (feedback)
    
    // prio 1
    for (int i = 0; i < jobCount; i++) {
        if (jobList[i].arrivalTime <= currentTime && 
            jobList[i].state != TASK_TERMINATED && 
            jobList[i].currentPriority == 1) {
            return i;
        }
    }

    // prio 2
    for (int i = 0; i < jobCount; i++) {
        if (jobList[i].arrivalTime <= currentTime && 
            jobList[i].state != TASK_TERMINATED && 
            jobList[i].currentPriority == 2) {
            return i;
        }
    }

    // prio 3 (round robin)
    int startSearch = (lastRRIndex + 1) % jobCount;
    int count = 0;
    int i = startSearch;

    while(count < jobCount) {
        if (jobList[i].arrivalTime <= currentTime && 
            jobList[i].state != TASK_TERMINATED && 
            jobList[i].currentPriority == 3) {
            lastRRIndex = i;
            return i;
        }
        i = (i + 1) % jobCount;
        count++;
    }

    return -1;
}

/* --- simulasyon taski --- */
void vSimulationTask(void *pvParameters) {
    printf("Simülasyon Başladı.\n\n");
    
    while (1) {
        int activeJobs = 0;

        // 1. zaman asimi kontrolu
        for (int i = 0; i < jobCount; i++) {
            if (jobList[i].state != TASK_TERMINATED) {
                // 20 sn kurali
                if ((currentTime - jobList[i].arrivalTime) > 20) {
                    printf("\x1b[31m%d.0000 sn\tproses zamanaşımı\t(id:%04d\töncelik:%d\tkalan süre:%d sn)\x1b[0m\n", 
                        currentTime, jobList[i].id, jobList[i].currentPriority, jobList[i].remainingTime);
                    
                    jobList[i].state = TASK_TERMINATED;
                    jobList[i].finishTime = currentTime; // bitis zamanini kaydet
                } else {
                    activeJobs++;
                }
            }
        }

        // hepsi bittiyse
        if (activeJobs == 0 && currentTime > 0) {
            printf("\nTüm görevler tamamlandı veya zaman aşımına uğradı.\n");
            
            // --> TABLOYU BURADA BASIYORUZ <--
            printPerformanceTable();
            
            vTaskEndScheduler();
        }

        // 2. is secimi
        int jobIndex = findNextJob();

        if (jobIndex != -1) {
            PCB *job = &jobList[jobIndex];
            
            char colorStr[20];
            sprintf(colorStr, "\x1b[%dm", job->color);

            // durum yazdir
            if (job->startTime == -1) {
                job->startTime = currentTime;
                printf("%s%d.0000 sn\tproses başladı\t\t(id:%04d\töncelik:%d\tkalan süre:%d sn)\x1b[0m\n", 
                       colorStr, currentTime, job->id, job->currentPriority, job->remainingTime);
            } else {
                printf("%s%d.0000 sn\tproses yürütülüyor\t(id:%04d\töncelik:%d\tkalan süre:%d sn)\x1b[0m\n", 
                       colorStr, currentTime, job->id, job->currentPriority, job->remainingTime);
            }

            job->remainingTime--;

            if (job->remainingTime == 0) {
                // bitti
                printf("%s%d.0000 sn\tproses sonlandı\t\t(id:%04d\töncelik:%d\tkalan süre:%d sn)\x1b[0m\n", 
                       colorStr, currentTime + 1, job->id, job->currentPriority, 0);
                
                job->state = TASK_TERMINATED;
                job->finishTime = currentTime + 1; // bitis zamanini kaydet
            } 
            else if (job->priority != 0) { 
                // user task feedback
                if (job->currentPriority < 3) {
                    job->currentPriority++;
                }
                printf("%s%d.0000 sn\tproses askıda\t\t(id:%04d\töncelik:%d\tkalan süre:%d sn)\x1b[0m\n", 
                       colorStr, currentTime + 1, job->id, job->currentPriority, job->remainingTime);
            }

        } else {
            // bosta
        }

        currentTime++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* --- baslatma --- */
void vSchedulerInit(void) {
    vLoadJobsFromFile("giris.txt");
}

void vSchedulerStart(void) {
    xTaskCreate(vSimulationTask, "SimTask", configMINIMAL_STACK_SIZE * 4, NULL, 1, NULL);
    vTaskStartScheduler();
}