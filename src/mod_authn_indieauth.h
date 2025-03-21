/*
** file: mod_authn_indieauth.h -- Apache sample authn_indieauth module
**/

#include <httpd.h>

#include <http_config.h>
#include <http_core.h>
#include <http_log.h>
#include <http_request.h>
#include <http_protocol.h>

#include <ap_config.h>

#define INDIEAUTH_HANDLER_NAME "authn_indieauth"
#define INDIEAUTH_AUTH_TYPE "indieauth"

typedef struct {
    char        context[256];
    char *scope;
    char *client_id_path;
} indieauth_config;

static char *indieauth_set_scope(cmd_parms *cmd, void *cfg, const char *arg);
static char *indieauth_set_client_id_path(cmd_parms *cmd, void *cfg, const char *arg);
static const char *indieauth_get_client_id(request_rec *r, indieauth_config *cfg);
void *indieauth_create_dir_conf(apr_pool_t *pool, char *context);
void *indieauth_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD);

static int indieauth_check_user_id_handler(request_rec *r);
static int indieauth_login_handler(request_rec *r);
static int indieauth_redirect_handler(request_rec *r);

static const command_rec indieauth_directives[] =
{
    AP_INIT_TAKE1("IndieAuthClientScope", indieauth_set_scope, NULL, ACCESS_CONF | OR_AUTHCFG, "A list of space separated scopes that the client will request from the provider"),
    AP_INIT_TAKE1("IndieAuthClientID", indieauth_set_client_id_path, NULL, ACCESS_CONF | OR_AUTHCFG, "Path to the client_id metadata document (JSON)."),
    { NULL }
};

static void indieauth_register_hooks(apr_pool_t *p)
{
    ap_hook_check_authn(indieauth_check_user_id_handler, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
    ap_hook_handler(indieauth_login_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(indieauth_redirect_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA authn_indieauth_module = {
    STANDARD20_MODULE_STUFF,
    indieauth_create_dir_conf,                  /* create per-dir    config structures */
    indieauth_merge_dir_conf,                   /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    indieauth_directives,                  /* table of config file commands       */
    indieauth_register_hooks  /* register hooks                      */
};

