//
// Created by meshi on 24-7-16.
//

#ifndef ESPANDORA_HA_CLIENT_H
#define ESPANDORA_HA_CLIENT_H

#include <esp_err.h>

#define HA_HOST "hass.hessian.me"
#define HA_USER_AGENT "ESPANDORA/1.0"
#define HA_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiIyZTBjNWJiMjU3ZDk0ZGY3ODljMzkwMjk1MjQzZDM2MSIsImlhdCI6MTcyMTA4OTgyOCwiZXhwIjoyMDM2NDQ5ODI4fQ.udQGcc9LAspri5F3cXRhaToIDT9VuKN90cJ8OBGOSlw"

// REST API doc: https://developers.home-assistant.io/docs/api/rest/
// websocket message example:
// {"type":"call_service","domain":"climate","service":"set_hvac_mode","service_data":{"entity_id":"climate.miir_ir02_6576_ir_aircondition_control","hvac_mode":"off"},"id":38}
// {"type":"call_service","domain":"climate","service":"set_hvac_mode","service_data":{"entity_id":"climate.miir_ir02_6576_ir_aircondition_control","hvac_mode":"off"},"id":39}

// {"type":"call_service","domain":"select","service":"select_option","target":{"entity_id":"select.remote_ir_1787800922005848064"},"service_data":{"option":"电源"},"id":38}
//
esp_err_t ha_rest_post(const char *url, const char *data);

esp_err_t ha_call_service(const char *domain, const char *service, const char *entity_id, const char *data);

#endif //ESPANDORA_HA_CLIENT_H
