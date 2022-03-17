/* Подключение библиотек и вспомогательных функций FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "supporting_functions.h"

/* Создание задач отправителя и получателя */
static void vSenderTask(void* pvParameters);
static void vReceiverTask(void* pvParameters);

/* Объявляется переменная типа QueueHandle_t. Она будет использоваться для хранения
очереди, к которой будут обращаться задачи. */
QueueHandle_t xQueue;

int main(void)
{
	/* Создается очередь, способная вместить 5 значений типа long (int32_t) */
	xQueue = xQueueCreate(5, sizeof(int32_t));

	if (xQueue != NULL)
	{
		/* Создается два экземпляра задач, которые будут отправлять данные в очередь.
		Одна задача будет записывать в очередь число 100, а другая 500. Для обеих
		задач выбран одинаковый приоритет 1. */
		xTaskCreate(vSenderTask, "Sender1", 1000, (void*)100, 1, NULL);
		xTaskCreate(vSenderTask, "Sender2", 1000, (void*)500, 1, NULL);

		/* Создается задача, которая будет читать значения из очереди. Приоритет
		задачи выше, чем у предыдущих (отправляющих). */
		xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
				/* Запуск шедулера, после которого начнется выполнение задач. */
		vTaskStartScheduler();
	}
	else
	{
		/* Очередь не может быть создана. */
		vPrintString("The queue could not be created.\r\n");
	}

	/* При правильной работе программы, она не дойдет до этого места. Если
	это произошло, это означает проблемы с памятью. */
	for (;; );
	return 0;
}

static void vSenderTask(void* pvParameters)
{
	int32_t lValueToSend;
	BaseType_t xStatus;

	/* Величина, посылаемая в очередь, будет передана в задачу через параметр.
	Благодаря этому каждый экземляр задачи будет иметь свое значение. */
	lValueToSend = (int32_t)pvParameters;

	/* Задача реализована как бесконечный цикл */
	for (;; )
	{
		/* Отправка значения в очередь. Первый параметр определяет очередь,
		в которую отправляются данные. Второй определяет адрес посылаемых данных.
		Третий параметр - время блокировки. */
		xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);

		if (xStatus != pdPASS)
		{
			/* Выдача ошибки переполнения очереди. */
			vPrintString("Could not send to the queue.\r\n");
		}
		/* Разрешение выполняться другим задачам, отправляющис данные в очередь.
		Вызов данной функции информирует шедулер о переключении к другой задаче. */
		taskYIELD();
	}
}

static void vReceiverTask(void* pvParameters)
{
	/* Объявление переменной, которая будет хранить значения из очереди. */
	int32_t lReceivedValue;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);

	/* Эта задача также является бесконечным циклом. */
	for (;; )
	{
		/* Очередь всегда должна быть пуста перед запуском задачи. */
		if (uxQueueMessagesWaiting(xQueue) != 0)
		{
			vPrintString("Queue should have been empty!\r\n");
		}

		/* Прием данных из очереди. Первый параметр определяет очередь,
		из которой считываются данные. Второй определяет адрес буфера,
		в который значения будут помещены. Третий параметр - время блокировки. */
		xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			/* Данные приняты из очереди, запись значения. */
			vPrintStringAndNumber("Received = ", lReceivedValue);
		}
		else
		{
			/* Данные из очереди не были приняты после ожидания в 100мс */
			vPrintString("Could not receive from the queue.\r\n");
		}
	}
}






