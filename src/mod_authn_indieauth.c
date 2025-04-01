/* 
**  mod_authn_indieauth.c -- Apache sample authn_indieauth module
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /authn_indieauth in as follows:
**
**    #   apache2.conf
**    LoadModule authn_indieauth_module modules/mod_authn_indieauth.so
**    <Location /authn_indieauth>
**    SetHandler authn_indieauth
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
*/ 

#include "mod_authn_indieauth.h"

static char *indieauth_set_scope(cmd_parms *cmd, void *cfg, const char *arg) {
    indieauth_config *conf = (indieauth_config*) cfg;
    if(conf) {
        strcpy(conf->scope, arg);
    }
}

static char *indieauth_set_client_id_path(cmd_parms *cmd, void *cfg, const char *arg) {
    indieauth_config *conf = (indieauth_config*) cfg;
    if(conf) {
        strcpy(conf->client_id_path, arg);
    }
}

static const char *indieauth_get_client_id(request_rec *r, indieauth_config *cfg) {
    char *path = cfg->client_id_path;
    if(strcmp(path, "")) {
        path = ""; // TODO: config's root path
    }
    // TODO: convert to full URI
    return path;
}

void *indieauth_create_dir_conf(apr_pool_t *pool, char *context_dir) {
    context_dir = context_dir ? context_dir : "/";
    indieauth_config *cfg = apr_pcalloc(pool, sizeof(indieauth_config));
    if(cfg) {
        /* Set some default values */
        strcpy(cfg->context_dir, context_dir);
        cfg->scope = "profile";
        cfg->client_id_path = "";
    }
    return cfg;
}

void *indieauth_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD) {
    indieauth_config *base = (indieauth_config *) BASE ; /* This is what was set in the parent context */
    indieauth_config *add = (indieauth_config *) ADD ;   /* This is what is set in the new context */
    indieauth_config *conf = (indieauth_config *) indieauth_create_dir_conf(pool, base->context_dir); /* This will be the merged configuration */

    /* Merge configurations */
    conf->scope = strcmp(add->scope, "") == 0 ? base->scope : add->scope;
    conf->client_id_path = strcmp(add->client_id_path, "") == 0 ? base->scope : add->scope;

    return conf;
}

static int indieauth_check_user_id_handler(request_rec *r)
{
	if (ap_auth_type(r) == NULL)
		return DECLINED;

	if (ap_is_initial_req(r) == 0) {

		if (r->main != NULL)
			r->user = r->main->user;
		else if (r->prev != NULL)
			r->user = r->prev->user;

		if (r->user != NULL) {

			ap_log_rerror(
			    APLOG_MARK, APLOG_DEBUG, 0, r,
			    "recycling user '%s' from initial request "
			    "for sub-request",
			    r->user);

			return OK;
		}
	}

	indieauth_config *cfg = (indieauth_config*) ap_get_module_config(r->per_dir_config, &authn_indieauth_module);

	if (strcasecmp((const char *)ap_auth_type(r), INDIEAUTH_AUTH_TYPE) == 0)
		return OK; // TODO: this needs to do the work

	return DECLINED;
}

static int indieauth_login_handler(request_rec *r)
{ // TODO
    /* First off, we need to check if this is a call for this handler.
     * If it is, we accept it and do our things, if not, we simply return DECLINED,
     * and the server will try somewhere else.
     */
    if (!r->handler || strcmp(r->handler, INDIEAUTH_HANDLER_NAME)) {
        return DECLINED;
    }

    indieauth_config *cfg = (indieauth_config*) ap_get_module_config(r->per_dir_config, &authn_indieauth_module);
    r->content_type = "text/html";

    if (!r->header_only)
        ap_rputs("The sample page from mod_authn_indieauth.c\n", r);
    return OK;
}

static int indieauth_redirect_handler(request_rec *r)
{ // TODO
    /* First off, we need to check if this is a call for this handler.
     * If it is, we accept it and do our things, if not, we simply return DECLINED,
     * and the server will try somewhere else.
     */
    if (!r->handler || strcmp(r->handler, INDIEAUTH_HANDLER_NAME)) {
        return DECLINED;
    }

    indieauth_config *cfg = (indieauth_config*) ap_get_module_config(r->per_dir_config, &authn_indieauth_module);
    r->content_type = "text/html";

    if (!r->header_only)
        ap_rputs("The sample page from mod_authn_indieauth.c\n", r);
    return OK;
}

