#include "main.h"
#include "sys.h"
#include "stdio.h"
#include "dev_io.h"
#include "dev_net.h"
#include "string.h"


static ptIODev ledDev = NULL;
static ptIODev keyDev = NULL;
static ptIODev dbgoutDev = NULL;
static ptNetDev netDev = NULL;

const static WiFiInfo wifi = {WiFi_ID, "GSW24", "li.120500"};
const static TCP_UDP_Info tcp = {TCP_ID, "192.168.1.116",1000 , 8888};


int main(void)
{
    char buf[32] = {0};
    KeyEvent key = {0};
    uint8_t led_status = 0;
    
    HAL_Init();
    SystemClock_Config();
  
    dbgoutDev = IODev_GetDev(DBGOUT);
    if(dbgoutDev != NULL)
    {
        dbgoutDev->Init(dbgoutDev);
    }
    ledDev = IODev_GetDev(LED);
    if(ledDev !=NULL)
    {
        ledDev->Init(ledDev);
    }
    
    keyDev = IODev_GetDev(KEY);
    if(keyDev !=NULL)
    {
        keyDev->Init(keyDev);
    }
    
    netDev = NetDev_GetDev(ESP8266);
    if(netDev != NULL)
    {
        if(netDev->Init(netDev) == 0)
        {
            if(netDev ->Connect(netDev, (char*)&wifi.id, 5000) == 0)
            {
                printf("Connect WiFi success! \r\n");
                //建立TCP连接
                if(netDev->Connect(netDev, (char *)&tcp.id, 500) != 0)
                {
                    printf("Connect TCP :%s failed.\r\n",tcp.IP);
                    while(1);
                }
                else 
                {
                    printf("Connect TCP :%s success.\r\n",tcp.IP);
                }
            }
            else
            {
                printf("Connect WiFi failed! \r\n");
                return -1;
            }
        }
    }
    while(1)
    {
        if(keyDev->Read(keyDev, (uint8_t*)&key, sizeof(KeyEvent)) == 0)
        {
            led_status = !led_status;
            ledDev->Write(ledDev, &led_status, 1);
            sprintf (buf, "Key >%d< press >%d< ms\r\n",key.num, key.time);
            netDev->Write(netDev, buf, strlen(buf), 50);
        }
    }
}










