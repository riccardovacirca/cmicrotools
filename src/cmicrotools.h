
#ifndef CMICROTOOLS_H
#define CMICROTOOLS_H

#include "apr.h"
#include "apr_pools.h"
#include "apr_tables.h"
#include "apr_file_io.h"
#include "apr_file_info.h"
#include "apr_lib.h"
#include "apr_strings.h"
#include "apr_escape.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_crypto.h"
#include "apr_env.h"
#include "apr_time.h"
#include "apr_date.h"
#include "apr_getopt.h"
#include "apr_proc_mutex.h"
#include "apr_file_io.h"
#include "apr_file_info.h"
#include "apr_dbd.h"
#include "stdio.h"
#include "errno.h"
#include "time.h"
#include "syscall.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdbool.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/file.h"
#include "string.h"
#include "signal.h"

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/hmac.h>

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

// COMMONS

#define CM_MAX_READ_BUFFER 16384
#define CM_ERROR_TIMESTAMP (-1)

int cm_rand(int l, int h);
int cm_is_empty(const char *s);
int cm_is_int(const char *s);
int cm_is_double(const char *s);
int cm_in_string(const char *s, const char *sub);
char* cm_buffer(apr_pool_t *mp, const char *s, apr_size_t *bf_size);
char* cm_str(apr_pool_t *mp, const char *s, apr_size_t sz);
char* cm_trim(apr_pool_t *pool, const char *str);
const char* cm_strip_char(apr_pool_t *mp, const char *s, char c);
char* cm_slice(apr_pool_t *mp, const char *s, apr_size_t i, apr_size_t l);
const char* cm_str_replace(apr_pool_t *mp, const char *s, const char *f, const char *r);
const char* cm_replace_char(apr_pool_t *mp, const char *s, char f, char r);
char* cm_empty_string_make(apr_pool_t *mp);
apr_array_header_t* cm_split(apr_pool_t *mp, const char *s, const char *sp);
char* cm_join(apr_pool_t *mp, apr_array_header_t *a, const char *sp);
char* cm_md5(apr_pool_t *mp, const char *s);
char* cm_base64_encode(apr_pool_t *mp, const char *s);
char* cm_base64_decode(apr_pool_t* mp, const char* s);
apr_table_t* cm_args_to_table(apr_pool_t *mp, const char *q);
int cm_table_nelts(apr_table_t *t);
apr_table_entry_t* cm_table_elt(apr_table_t *t, int i);
char* cm_datetime(apr_pool_t *mp, apr_time_t t, const char *f);
char* cm_datetime_local(apr_pool_t *mp, apr_time_t t, const char *f);
char* cm_datetime_utc(apr_pool_t *mp, apr_time_t t, const char *f);
int cm_is_dir(const char *d, apr_pool_t *mp);
int cm_is_file(const char *f, apr_pool_t *mp);
apr_status_t cm_file_open(apr_file_t **fd, const char *f, apr_int32_t fl, apr_pool_t *mp);
apr_status_t cm_file_open_read(apr_file_t **fd, const char *f, apr_pool_t *mp);
apr_status_t cm_file_open_append(apr_file_t **fd, const char *f, apr_pool_t *mp);
apr_status_t cm_file_open_truncate(apr_file_t **fd, const char *f, apr_pool_t *mp);
apr_size_t cm_file_write(apr_file_t *fd, const char *buf, apr_size_t l);
apr_size_t cm_file_read(apr_pool_t *mp, apr_file_t *fd, void **buf);
apr_time_t cm_timestamp(int year, int month, int day, int hour, int minute, int second);
apr_time_t cm_now();
apr_table_entry_t* cm_table_entry(apr_table_t *t, int i);
char* cm_pipein(apr_pool_t *mp);
char* cm_env(const char *e, apr_pool_t *mp);
void cm_daemonize();

// JSON

#include "json.h"
#define CM_JSON_TRUE_S  "true"
#define CM_JSON_FALSE_S "false"
#define CM_JSON_NULL_S  "null"

typedef enum cm_json_type_t
{
  CM_JSON_T_ZERO       = 0x00000000,
  CM_JSON_T_NULL       = 0x00000001,
  CM_JSON_T_BOOLEAN    = 0x00000002,
  CM_JSON_T_PAIR       = 0x00000004,
  CM_JSON_T_INT16      = 0x00000008,
  CM_JSON_T_UINT16     = 0x00000010,
  CM_JSON_T_INT32      = 0x00000020,
  CM_JSON_T_UINT32     = 0x00000040,
  CM_JSON_T_INT64      = 0x00000080,
  CM_JSON_T_UINT64     = 0x00000100,
  CM_JSON_T_FLOAT      = 0x00000200,
  CM_JSON_T_DOUBLE     = 0x00000400,
  CM_JSON_T_STRING     = 0x00000800,
  CM_JSON_T_OBJECT     = 0x00001000,
  CM_JSON_T_ARRAY      = 0x00002000,
  CM_JSON_T_DATE       = 0x00004000,
  CM_JSON_T_NUMBER     = 0x00008000,
  CM_JSON_T_TABLE      = 0x00010000,
  CM_JSON_T_TIMESTAMP  = 0x00020000,
  CM_JSON_T_JSON       = 0x00040000,
  CM_JSON_T_DBD_SCHEMA = 0x00080000,
  CM_JSON_T_VECTOR     = 0x00100000
} cm_json_type_t;

typedef struct cm_json_pair_t {
  const char *key;
  void *val;
  cm_json_type_t type;  
} cm_json_pair_t;

typedef apr_array_header_t cm_json_object_t;
cm_json_object_t* cm_json_decode(apr_pool_t *mp, const char *s);
const char* cm_json_encode(apr_pool_t *mp, const void *obj, cm_json_type_t t);

// LOGGER

#define CM_LOG_MAX_FILE_SIZE 500 * 1024 * 1024 /* (10MB) */
#define CM_LOG_MAX_MSG_SIZE 512
#define CM_LOG_MSG_FMT "[%s] [%s] [%05d] %s\r\n"

typedef struct cm_logger_t {
  apr_pool_t *pool;
  apr_file_t *fh;
  const char *fname;
  apr_proc_mutex_t *mutex;
  apr_size_t max_size;
} cm_logger_t;

cm_logger_t* cm_log_alloc(apr_pool_t *mp, apr_proc_mutex_t *m, const char *f, apr_size_t sz);
void cm_log_rotate(cm_logger_t *l);
void cm_log_destroy(cm_logger_t *l);

#define cm_log(l, t, m) do {\
  if (l != NULL && t != NULL && m != NULL) {\
    char _log[CM_LOG_MAX_MSG_SIZE], _ts[APR_CTIME_LEN];\
    apr_time_t _now = apr_time_now();\
    apr_ctime(_ts, _now);\
    apr_snprintf(_log, sizeof(_log), CM_LOG_MSG_FMT, _ts, t, __LINE__, m);\
    size_t _len = strlen(_log);\
    if (_len > 0 && _len < (sizeof(_log)-1) && _log[_len-1] == '\n') {\
      apr_proc_mutex_lock(l->mutex);\
      apr_file_printf(l->fh, "%s", _log);\
      apr_file_flush(l->fh);\
      cm_log_rotate(l);\
      apr_proc_mutex_unlock(l->mutex);\
    }\
  }\
} while (0)

// DBD

typedef struct cm_dbd_t {
  int err;
  const char *er_msg;
  const apr_dbd_driver_t *drv;
  apr_dbd_t *hdl;
  apr_dbd_transaction_t *trx;
} cm_dbd_t;

cm_dbd_t* cm_dbd_alloc(apr_pool_t *mp);
int cm_dbd_open(apr_pool_t *mp, cm_dbd_t *dbd, const char *drv, const char *con);
const char* cm_dbd_escape(apr_pool_t *mp, cm_dbd_t *dbd, const char *s);
int cm_dbd_query(apr_pool_t *mp, cm_dbd_t *dbd, const char *sql);
apr_array_header_t* cm_dbd_select(apr_pool_t *mp, cm_dbd_t *dbd, const char *sql);
int cm_dbd_transaction_start(apr_pool_t *mp, cm_dbd_t *dbd);
int cm_dbd_transaction_end(apr_pool_t *mp, cm_dbd_t *dbd);
//int cm_dbd_prepared_query(apr_pool_t *mp, cm_dbd_t *dbd, const char *sql, const char **args, int sz);
int cm_dbd_prepared_query(apr_pool_t *mp, cm_dbd_t *dbd, const char *sql, apr_table_t *args);
apr_array_header_t* cm_dbd_prepared_select(apr_pool_t *mp, cm_dbd_t *dbd, const char *sql, apr_table_t *args);
int cm_dbd_num_records(apr_array_header_t *rset);
int cm_dbd_num_columns(apr_array_header_t *rset);
apr_array_header_t* cm_dbd_column_names(apr_pool_t *mp, apr_array_header_t *rset);
apr_table_t* cm_dbd_record(apr_array_header_t *rset, int idx);
const char* cm_dbd_field_value(apr_array_header_t *rset, int idx, const char*key);
int cm_dbd_field_set(apr_array_header_t *rset, int idx, const char *key, const char *val);
int cm_dbd_close(cm_dbd_t *dbd);
const char* cm_dbd_driver_name(cm_dbd_t *dbd);
const char* cm_dbd_error(cm_dbd_t *dbd);

// HTTP REQUEST

typedef enum cm_request_type_t {
  CM_REQUEST_T_NONE,
  CM_REQUEST_T_INT,
  CM_REQUEST_T_DOUBLE,
  CM_REQUEST_T_STRING,
  CM_REQUEST_T_PASSWORD,
  CM_REQUEST_T_DATE,
  CM_REQUEST_T_EMAIL
} cm_request_type_t;

typedef enum cm_request_filter_t {
  CM_REQUEST_F_NONE,
  CM_REQUEST_F_MD5
} cm_request_filter_t;

typedef struct cm_request_validator_t {
  const char *key;
  cm_request_type_t type;
  cm_request_filter_t filter;
} cm_request_validator_t;

typedef struct cm_http_request_t {
  apr_pool_t *pool;
  int client_port;
  const char *method;
  const char *body;
  const char *query;
  const char *uri;
  const char *http_version;
  const char *client_ip;
  const char *prev_method;
  const char *prev_uri;
  const char *session_id;
  const char *message;
  const char *username;
  const char *password;
  apr_table_t *headers;
  apr_table_t *args;
  apr_table_t *parsed_uri;
  apr_table_t *cookies;
  apr_array_header_t *multipart_data;
} cm_http_request_t;

cm_http_request_t* cm_http_request_alloc(apr_pool_t *mp);
apr_table_t *cm_http_request_validate_args(cm_http_request_t *r, cm_request_validator_t *vd, int nargs);
apr_table_t *cm_http_request_validate_multipart_args(cm_http_request_t *r, cm_request_validator_t *vd, int nargs);

// HTTP RESPONSE

typedef struct cm_http_response_t {
  apr_pool_t *pool;
  int status;
  int is_binary;
  apr_table_t *headers;
  apr_size_t size;
  void *buffer;
} cm_http_response_t;

cm_http_response_t* cm_http_response_alloc(apr_pool_t *mp);
const char* cm_http_response_hd_serialize(cm_http_response_t *r);
void cm_http_response_hd_set(cm_http_response_t *r, const char *k, const char *v);
const char* cm_http_response_hd_get(cm_http_response_t *r, const char *k);
void cm_http_response_buffer_set(cm_http_response_t *r, void *buf, apr_size_t sz);

// SERVICE

typedef struct cm_service_t {
  apr_pool_t *pool;
  int authorized;
  char *er_msg;
  cm_http_request_t *request;
  cm_http_response_t *response;
  cm_dbd_t *dbd;
  cm_logger_t *logger;
} cm_service_t;

typedef int (*cm_route_t)(cm_service_t *sv);

cm_service_t* cm_alloc(apr_pool_t *mp);
void cm_handler(cm_service_t *sv);
void cm_route(cm_service_t *sv, const char *mth, const char *uri, cm_route_t fn);
void cm_printf(cm_service_t *sv, const char *fmt, ...);

#define _cm_authorized_routes(_s) \
  for (\
    int _auth_flag = (_s->authorized = 1); \
    _auth_flag && !_s->response->status; \
    _auth_flag = (_s->authorized = 0)\
  )

#define cm_authorized_routes(_s,_f) \
  for (int _loop = 1; _loop && !_s->response->status && _f(_s);_loop = 0)

#define cm_download(s, buf, sz, f, enc) \
  do {\
    cm_http_response_hd_set(s->response, "Expires", "Mon, 23 Oct 1972 16:00:00 GMT");\
    cm_http_response_hd_set(s->response, "Pragma", "hack");\
    cm_http_response_hd_set(s->response, "Cache-Control", "must-revalidate,post-check=0,pre-check=0");\
    cm_http_response_hd_set(s->response, "Cache-Control", "private");\
    cm_http_response_hd_set(s->response, "Content-Description", "File Transfer");\
    cm_http_response_hd_set(s->response, "Content-Disposition", apr_psprintf(s->pool, "attachment; filename=%s", f));\
    cm_http_response_hd_set(s->response, "Content-Transfer-Encoding", enc);\
    /*cm_http_response_hd_set(s, "Content-Length", apr_psprintf(s->pool, "%" APR_UINT64_T_FMT, sz));*/\
    cm_http_response_hd_set(s->response, "Content-Type", "application/download");\
    cm_http_response_buffer_set(s->response, (void*)buf, sz);\
  } while (0)


char* cm_jwt_base64_encode(const unsigned char *input, int length);
char* cm_jwt_token_create(apr_pool_t *mp, apr_table_t *claims, const char *key);
char* cm_hmac_encode(const char *key, const char *s, apr_size_t sz);
int cm_jwt_token_validate(apr_pool_t *mp, const char *tok, const char *key);

#ifdef _DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#ifdef _DAEMON
#define DAEMON 1
#else
#define DAEMON 0
#endif

#ifdef _MONGOOSE
#define MONGOOSE 1
#else
#define MONGOOSE 0
#endif

#ifdef _TLS
#define TLS 1
#else
#define TLS 0
#endif

#define HTTP_OK_FMT "HTTP/1.1 200 OK\r\n%sContent-Length: %d\r\n\r\n"
#define CM_DBD_POOL_INIT_SIZE 20

typedef struct cm_dbd_pool_t {
  apr_array_header_t *connections;
  int counter;
  apr_proc_mutex_t *mutex;
} cm_dbd_pool_t;

typedef struct cm_server_t {
  apr_pool_t *pool;
  const char *host;
  const char *port;
  const char *port_s;
  const char *addr;
  const char *addr_s;
  const char *timeout;
  const char *max_threads;
  const char *log_file;
  const char *dbd_driver;
  const char *dbd_conn_s;
  const char *upload_dir;
  cm_logger_t *logger;
} cm_server_t;

typedef void(*sighd_t)(int s);

cm_server_t* cm_server_alloc(apr_pool_t *mp);
void cm_server_destroy(cm_server_t *s);
int cm_dbd_pool_alloc(apr_pool_t *mp);
void cm_dbd_pool_add(apr_pool_t *mp, const char *drv, const char *conn_s);
int cm_dbd_pool_init(apr_pool_t *mp, const char *drv, const char *conn_s);
cm_dbd_t* cm_dbd_pool_get();
void cm_dbd_pool_release();
void cm_dbd_pool_destroy();
void cm_http_request_headers_set(apr_pool_t *mp, cm_http_request_t *rq, struct mg_http_message *hm);
apr_table_t* cm_http_request_args_parse(apr_pool_t *mp, const char *s, const char *sp1, const char *sp2);
apr_table_t* cm_http_request_cookies_parse(apr_pool_t *mp, struct mg_http_message *hm);
apr_table_t* cm_http_query_string_parse(apr_pool_t*mp, struct mg_http_message *hm);
apr_table_t* cm_http_request_body_parse(apr_pool_t*mp, struct mg_http_message *hm);
extern void cm_signal_exit(int signum);
void cm_signal_handler(struct sigaction *sig_action, sighd_t cb);
int cm_http_request_multipart_parse(apr_pool_t *mp, cm_http_request_t *rq, struct mg_connection *c, struct mg_http_message *hm);
void cm_http_request_handler(struct mg_connection *c, int ev, void *ev_data);
int cm_cmd_args_parse(cm_server_t *s, int argc, char *argv[], char **er_msg);
int cm_server_init(apr_pool_t *mp, cm_server_t **s, int argc, char *argv[], char **er_msg);

extern cm_dbd_pool_t *dbd_pool;

















#ifdef _CM_PDF

#include <math.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <fontconfig/fontconfig.h>

void cm_pdf(apr_pool_t *m, const char *s, const char *d, double w, double h);

#endif /* _CM_PDF */


#ifdef download
#define Z_APACHE_RESPONSE_BUFFER(r, bf, sz, ct) do {\
  if (sz > 0) {\
    void *buffer = (void*)apr_palloc(r->pool, sizeof(unsigned char)*(sz));\
    if (buffer != NULL) {\
      memcpy(buffer, bf, sz);\
      ap_set_content_type(r, ct);\
      ap_rwrite(buffer, sz, r);\
    }\
  }\
} while (0)

#define Z_APACHE_RESPONSE_DOWNLOAD(r, bf, sz, fn) do {\
  if ((r!=NULL) && (bf!=NULL) && (sz > 0) && (fn!=NULL)){\
    apr_table_setn(r->headers_out, "Expires", "Mon, 23 Oct 1972 16:00:00 GMT");\
    apr_table_setn(r->headers_out, "Pragma", "hack");\
    apr_table_setn(r->headers_out, "Cache-Control", "must-revalidate,post-check=0,pre-check=0");\
    apr_table_setn(r->headers_out, "Cache-Control", "private");\
    apr_table_setn(r->headers_out, "Content-Description", "File Transfer");\
    apr_table_setn(r->headers_out, "Content-Disposition", apr_psprintf(r->pool, "attachment; filename=\"%s\"", fn));\
    apr_table_setn(r->headers_out, "Content-Transfer-Encoding", "binary");\
    apr_table_setn(r->headers_out, "Content-Length", apr_psprintf(r->pool, "%" APR_UINT64_T_FMT, sz));\
    Z_APACHE_RESPONSE_BUFFER(r, bf, sz, "application/download");\
  }\
} while (0)

#endif







#ifdef __cplusplus
}
#endif
#endif /* CMICROTOOLS_H */
