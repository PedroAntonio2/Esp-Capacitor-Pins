#ifndef WIFI_H
#define WIFI_H

extern char mensagem[100];

void initialise_wifi(void);
void send_messages(char topico[50], char exemplo[50], char mensagem[50]);
void subscribe_to_topic(char topic[50]);

#endif