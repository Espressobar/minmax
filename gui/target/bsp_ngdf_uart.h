#ifndef TARGET_UART_H_
#define TARGET_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void setupUART();
char* runUART(uint32_t* length);
void sendUART(const void* data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
