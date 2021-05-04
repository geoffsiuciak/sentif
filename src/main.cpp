#include "sentif.h"
  
int main(int argc, char * const argv[]) 
{
    if (argc == 1) _(HELP);
	auto ctx = new_opt_ctx();
	int flag;

    while((flag = getopt(argc, argv, FLAGS)) != -1)
    {
        switch(flag)
        {
            case 'p':  ctx->host     = atoi(optarg);  break;
            case 'a':  ctx->admin    = atoi(optarg);  break;
            case 'r':  ctx->root     = optarg;        break;
            case 'o':  ctx->logout   = optarg;        break;
            case 'e':  ctx->errout   = optarg;        break;
            case 't':  ctx->protocol = TCP;           break;
            case 'u':  ctx->protocol = UDP;           break;
            case 'b':  ctx->run_back = true;          break;
            case 'd':  ctx->log_use  = false;         break;
            case 's':  _(ERROUT);                     break;
            case 'h':  _(HELP);                       break;
            case 'v':  _(HELP_VERBOSE);               break;
            default:   exit(SENTIF_BAIL);
        }
    }

    if (!ctx->logout) { ctx->logout = (char*)LOGOUT; }
    if (!ctx->errout) { ctx->errout = (char*)ERROUT; }
	if (ctx->host == -1) bail_("missing host port");
	if (ctx->protocol == -1) bail_("missing protocol");

    if (ctx->host == ctx->admin)
        bail_("admin service cannot run on the host port");

    if (!ctx->root) {
		char cwd[BUFFER_S];
		memset(cwd, '\0', sizeof(cwd));
		if (!(cwd == getcwd(cwd, BUFFER_S)))
			bail_("failed to get name of cwd");
		else
			ctx->root = cwd;
	} 
	
	if (!(access(ctx->root, R_OK) == 0 && access(ctx->root, W_OK) == 0)) {
		bail_("you don't have R/W priviledges for this root");
	}

    if (ctx->log_use && !(access(ctx->logout, F_OK) == 0)) {
        if (!(creat(ctx->logout, S_IRWXU) > -1))
            bail_("failed to init logout");
    }
	
    if (!(access(ctx->errout, F_OK) == 0)) {
		if (!(creat(ctx->errout, S_IRWXU) > -1))
			bail_("failed to init errout");
	}
	
    to_the_moon(ctx);

    return SENTIF_PROPER;
}
