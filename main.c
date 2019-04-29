/* Standard C libraries */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* OS specific libraries */
#ifdef _WIN32

#include<windows.h>

#endif

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_spi.h"

#include "nrf24.h"


#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%lx) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};

FT_HANDLE ftHandle;

int main() {
    FT_STATUS status = FT_OK;
    FT_DEVICE_LIST_INFO_NODE devList = {0};
    ChannelConfig channelConf = {0};
    uint8 address = 0;
    uint32 channels = 0;
    uint16 data = 0;
    uint8 i = 0;
    uint8 latency = 2;

    channelConf.ClockRate = 8000000;
    channelConf.LatencyTimer = latency;
    channelConf.configOptions =
            SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3;// | SPI_CONFIG_OPTION_CS_ACTIVELOW;
    channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/

    status = SPI_GetNumChannels(&channels);
    APP_CHECK_STATUS(status);
    printf("Number of available SPI channels = %d\n", (int) channels);

    if (channels > 0) {
        for (i = 0; i < channels; i++) {
            status = SPI_GetChannelInfo(i, &devList);
            APP_CHECK_STATUS(status);
            printf("Information on channel number %d:\n", i);
            /* print the dev info */
            printf("Flags=0x%lx\n", devList.Flags);
            printf("Type=0x%lx\n", devList.Type);
            printf("ID=0x%lx\n", devList.ID);
            printf("LocId=0x%lx\n", devList.LocId);
            printf("SerialNumber=%s\n", devList.SerialNumber);
            printf("Description=%s\n", devList.Description);
            printf("ftHandle=0x%p\n", devList.ftHandle);/*is 0 unless open*/
        }
    }

    if (channels == 0) {
        return -1;
    }

    /* Open the first available channel */
    status = SPI_OpenChannel(0, &ftHandle);
    APP_CHECK_STATUS(status);
    printf("\nhandle=0x%p status=0x%lx\n", ftHandle, status);
    status = SPI_InitChannel(ftHandle, &channelConf);
    APP_CHECK_STATUS(status);


//    printf("Begin to test GPIO.\n");
//    for (int n = 0; n < 100000; n++) {
//        status = FT_WriteGPIO(ftHandle, 0xFF, 0x00);
//        APP_CHECK_STATUS(status);
//        printf("Low\n");
//        Sleep(200);
//        status = FT_WriteGPIO(ftHandle, 0xFF, 0xFF);
//        APP_CHECK_STATUS(status);
//        printf("High\n");
//        Sleep(200);
//    }

    /* initializes hardware pins */
    nrf24_init();

    /* RF channel: #2 , payload length: 4 */
    nrf24_config(2, 16);

    uint8_t rx_mac[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t tx_mac[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};

    /* Set the module's own address */
    nrf24_rx_address(rx_mac);

    /* Set the transmit address */
    nrf24_tx_address(tx_mac);

    uint8_t data_array[16] = {0};


    for (int n = 0; n < 100000; n++) {
        snprintf((char *) data_array, sizeof(data_array), "Hello, %d", n);
        printf("Sending data: %s\n",(char *)data_array);
        /* Automatically goes to TX mode */
        nrf24_send(data_array);
        /* Wait for transmission to end */
        while (nrf24_isSending()) {
            Sleep(1);
        }
        uint8_t temp;

        /* Make analysis on last transmission attempt */
        temp = nrf24_lastMessageStatus();

        if (temp == NRF24_TRANSMISSION_OK) {
            printf("Transmission went OK\n");
        } else if (temp == NRF24_MESSAGE_LOST) {
            printf("Message is lost ...\n");
        }

        /* Retransmission count indicates the transmission quality */
        temp = nrf24_retransmissionCount();
        printf("Retransmission count: %d\r\n", temp);
        Sleep(500);
    }


    status = SPI_CloseChannel(ftHandle);
    APP_CHECK_STATUS(status);


}

