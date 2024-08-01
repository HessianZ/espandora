//
// Created by Hessian on 2023/7/29.
//

#ifndef ESP_MENJIN_MQTT_H
#define ESP_MENJIN_MQTT_H

void mqtt_task(void *pvParameters);
void mqtt_publish(char *topic, char* content, int retain);
void mqtt_client_id(char *id_string);
void mqtt_handle_cmd(char *payload, int len);
void ha_state_push(char *ent_id, char *state);
int ha_entity_id(char *ent_id, char* dev_id, char* entity_name);
int ha_dev_topic(char* topic, char *ent_id, char* suffix);
#endif //ESP_MENJIN_MQTT_H