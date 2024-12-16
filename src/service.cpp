#include "service.hpp"

int HelloWorldController(cm_service_t *s) {
  cm_http_response_hd_set(s->response, "Content-Type", "application/json");
  const char *msg = cm_json_encode(s->pool, "Hello, World!", CM_JSON_T_STRING);
  cm_printf(s, "{\"err\":%s,\"log\":%s,\"out\":%s}", "false", "null", msg);
  return 200;
}

void cm_handler(cm_service_t *s) {
  cm_route(s, "GET", "/api/helloworld", HelloWorldController);
}
