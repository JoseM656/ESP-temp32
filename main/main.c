/*
 * José Morata
 *
 * This code is just a example.
 */

  
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define BUTTON_GPIO 0 // Boton del esp32

SemaphoreHandle_t button_sem; // Variable global para el semaforo

QueueHandle_t data_queue; // variable global

// Defino la estructura

struct system_status_t {

    uint8_t cpu_percentage;
    uint8_t ram_percentage;
    uint8_t system_tem;
    uint8_t id_core;
    uint16_t uptime_ms;
};
  

// Prepara la ISR en RAM
void IRAM_ATTR button_isr(void *arg) {

    BaseType_t task_woken = pdFALSE;

    xSemaphoreGiveFromISR(button_sem, &task_woken);

    if (task_woken) {

        portYIELD_FROM_ISR();

    }

}

// Configura el gipo 0
void configure_button(void) {

    gpio_config_t io_conf = {

        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Flanco descendente

    };

    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr, NULL);

}

  

// Espera la interrupcion y se aplica preemptacion
void button_task(void *pvParameters){

    while (1) {

        if (xSemaphoreTake(button_sem, portMAX_DELAY)) {

            printf("Interrupción recibida. Esperando 10 segundos...\n");

            vTaskDelay(pdMS_TO_TICKS(10000));

        }

    }

}


// Nucleo 0: Tarea productora
void producer_task(void *pvParameters) {

    struct system_status_t data; // Instancio la esctructura con una variable

    while (1){

        printf("[Core 0] Procesando datos...\n");

        // Cada 3000ms la tarea despierta y genera 3 datos pseudoaleatorios

        uint32_t t = xTaskGetTickCount();

        uint8_t cpu = t % 101;

        uint8_t ram = t % 101;

        uint8_t tem = t % 101;

        // Completo la estructura con esos datos

        data.cpu_percentage = cpu;

        data.ram_percentage = ram;

        data.system_tem = tem;

        data.id_core = 0;

        data.uptime_ms = 3000;

        printf("[Core 0] Enviando datos...\n");

        xQueueSend(data_queue, &data, portMAX_DELAY); // Mete las instancias de data en la cola

        vTaskDelay(pdMS_TO_TICKS(3000));

    }

}

  

// Nucleo 1: Tarea consumidora
void consumer_task(void *pvParameters) {

    struct system_status_t received; // se recibe las instancias de struct

    while (1) {

        if (xQueueReceive(data_queue, &received, portMAX_DELAY)) {

  

            printf("[Core 1] Datos recibidos de %d\n", received.id_core);

            printf("[Core 1] Tiempo de espera entre ciclo %d\n", received.uptime_ms);

            printf("[Core 1] Consumo de CPU: %d\n", received.cpu_percentage);

            printf("[Core 1] Consumo de RAM: %d\n", received.ram_percentage);

            printf("[Core 1] Temperatura del sistema: %d\n", received.system_tem);

        }

    }

}

  

void app_main(void) {

    data_queue = xQueueCreate(5, sizeof(struct system_status_t)); // cuanto se espera recibir de la cola
    button_sem = xSemaphoreCreateBinary(); // Asigna la varible global a un semaforo

    if (data_queue == NULL) {

        printf("[System] Error creando la cola o cola vacia");
        return;

  

    }


    if (button_sem == NULL) {

        printf("Error creando semáforo\n");
        return;

    }

    configure_button();

    xTaskCreatePinnedToCore(

        producer_task,
        "Producer",
        2048,
        NULL,
        5,
        NULL,
        0 // core 0
    );

  

    xTaskCreatePinnedToCore(

        consumer_task,
        "Consumer",
        2048,
        NULL,
        5,
        NULL,
        1 // core 1
    );


    xTaskCreatePinnedToCore(

        button_task,
        "ISR_Interruption",
        2048,
        NULL,
        8,
        NULL,
        1 // Core 1, listo para una interrupcion.

    );

}