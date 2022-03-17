/* ����������� ��������� � ��������������� ������� FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "supporting_functions.h"

/* �������� ����� ����������� � ���������� */
static void vSenderTask(void* pvParameters);
static void vReceiverTask(void* pvParameters);

/* ����������� ���������� ���� QueueHandle_t. ��� ����� �������������� ��� ��������
�������, � ������� ����� ���������� ������. */
QueueHandle_t xQueue;

int main(void)
{
	/* ��������� �������, ��������� �������� 5 �������� ���� long (int32_t) */
	xQueue = xQueueCreate(5, sizeof(int32_t));

	if (xQueue != NULL)
	{
		/* ��������� ��� ���������� �����, ������� ����� ���������� ������ � �������.
		���� ������ ����� ���������� � ������� ����� 100, � ������ 500. ��� �����
		����� ������ ���������� ��������� 1. */
		xTaskCreate(vSenderTask, "Sender1", 1000, (void*)100, 1, NULL);
		xTaskCreate(vSenderTask, "Sender2", 1000, (void*)500, 1, NULL);

		/* ��������� ������, ������� ����� ������ �������� �� �������. ���������
		������ ����, ��� � ���������� (������������). */
		xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
				/* ������ ��������, ����� �������� �������� ���������� �����. */
		vTaskStartScheduler();
	}
	else
	{
		/* ������� �� ����� ���� �������. */
		vPrintString("The queue could not be created.\r\n");
	}

	/* ��� ���������� ������ ���������, ��� �� ������ �� ����� �����. ����
	��� ���������, ��� �������� �������� � �������. */
	for (;; );
	return 0;
}

static void vSenderTask(void* pvParameters)
{
	int32_t lValueToSend;
	BaseType_t xStatus;

	/* ��������, ���������� � �������, ����� �������� � ������ ����� ��������.
	��������� ����� ������ �������� ������ ����� ����� ���� ��������. */
	lValueToSend = (int32_t)pvParameters;

	/* ������ ����������� ��� ����������� ���� */
	for (;; )
	{
		/* �������� �������� � �������. ������ �������� ���������� �������,
		� ������� ������������ ������. ������ ���������� ����� ���������� ������.
		������ �������� - ����� ����������. */
		xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);

		if (xStatus != pdPASS)
		{
			/* ������ ������ ������������ �������. */
			vPrintString("Could not send to the queue.\r\n");
		}
		/* ���������� ����������� ������ �������, ������������ ������ � �������.
		����� ������ ������� ����������� ������� � ������������ � ������ ������. */
		taskYIELD();
	}
}

static void vReceiverTask(void* pvParameters)
{
	/* ���������� ����������, ������� ����� ������� �������� �� �������. */
	int32_t lReceivedValue;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);

	/* ��� ������ ����� �������� ����������� ������. */
	for (;; )
	{
		/* ������� ������ ������ ���� ����� ����� �������� ������. */
		if (uxQueueMessagesWaiting(xQueue) != 0)
		{
			vPrintString("Queue should have been empty!\r\n");
		}

		/* ����� ������ �� �������. ������ �������� ���������� �������,
		�� ������� ����������� ������. ������ ���������� ����� ������,
		� ������� �������� ����� ��������. ������ �������� - ����� ����������. */
		xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			/* ������ ������� �� �������, ������ ��������. */
			vPrintStringAndNumber("Received = ", lReceivedValue);
		}
		else
		{
			/* ������ �� ������� �� ���� ������� ����� �������� � 100�� */
			vPrintString("Could not receive from the queue.\r\n");
		}
	}
}






