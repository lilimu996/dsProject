/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"



#include "MQTTClient.h"
#include "MQTTFreeRTOS.h"
#include "string.h"

void messageArrived(MessageData* data)
{
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
        data->message->payloadlen, data->message->payload);
    printf("receive success!!!!!!!!!!");
}

static void prvMQTTEchoTask(void *pvParameters)
{
    /* connect to m2m.eclipse.org, subscribe to a topic, send and receive messages regularly every 1 sec */
    MQTTClient client;
    Network network;
    unsigned char sendbuf[256], readbuf[256];
    int rc = 0, 
        count = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    pvParameters = 0;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    //修改设备MQTT地址
    char* address = "iot-06z00ffa28x9ar9.mqtt.iothub.aliyuncs.com";
    if ((rc = NetworkConnect(&network, address, 1883)) != 0)
        printf("Return code from network connect is %d\n", rc);

    #if defined(MQTT_TASK)
    if ((rc = MQTTStartTask(&client)) != pdPASS)
        printf("Return code from start tasks is %d\n", rc);
    #endif

    //修改设备MQTT用户名，密码，ID
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "k0aglMzMLKf.LED1|securemode=2,signmethod=hmacsha256,timestamp=1696728831630|";
    connectData.username.cstring = "LED1&k0aglMzMLKf";
    connectData.password.cstring = "26345421555c46687d7b05796d944e86bdf930886ada13c514a8a6b230a53b4b";

    if ((rc = MQTTConnect(&client, &connectData)) != 0)
        printf("Return code from MQTT connect is %d\n", rc);
    else
        printf("MQTT Connected\n");

    //订阅Topic
    if ((rc = MQTTSubscribe(&client, "/k0aglMzMLKf/LED1/user/led", 0, messageArrived)) != 0)
        printf("Return code from MQTT subscribe is %d\n", rc);

    while (++count)
    {
        MQTTMessage message;
        char payload[30];

        message.qos = 1;
        message.retained = 0;
        message.payload = payload;
        sprintf(payload, "message number %d", count);
        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, "/k0aglMzMLKf/LED1/user/keyInfo", &message)) != 0)
            printf("Return code from MQTT publish is %d\n", rc);
    #if !defined(MQTT_TASK)
        if ((rc = MQTTYield(&client, 1000)) != 0)
            printf("Return code from yield is %d\n", rc);
    #endif
        printf("Return code from yield is %d\n", rc);
    }

	/* do not return */
}


void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	BaseType_t x = 0L;

	xTaskCreate(prvMQTTEchoTask,	/* The function that implements the task. */
			"MQTTEcho0",			/* Just a text name for the task to aid debugging. */
			usTaskStackSize,	/* The stack size is defined in FreeRTOSIPConfig.h. */
			(void *)x,		/* The task parameter, not used in this case. */
			uxTaskPriority,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
			NULL);				/* The task handle is not used. */
}
/*-----------------------------------------------------------*/


