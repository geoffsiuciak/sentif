#include "sentif.h"

ctx_t* new_opt_ctx()
{
    static ctx_t ctx = { 
		-1, -1, -1, 
        true, false,
        nullptr, nullptr
    };
    return &ctx;
}

void to_the_moon(const ctx_t *ctx)
{
	printf("sentif running!\n");
    printf("on host port %i\n", ctx->host);
    printf("using %s\n", ctx->protocol == TCP ? "TCP" : "UDP");
    printf("from root %s\n", ctx->root);

    if (ctx->admin != -1)
        printf("with admin port %i\n", ctx->admin);
    else
        printf("with admin disabled\n");

    sentif::WebServer s(*ctx);
    s.go();
    // s.ban("127.0.0.1");
}

void _(const char* file)
{
	if (!(access(file, R_OK) == 0)) 
		bail_("display file not found");    
	
	char buffer[1024];
    int bytes;
    int fd = open(file, O_RDONLY);
    
	while ((bytes = read(fd, buffer, 1024)) > 0)
        write(1, buffer, bytes);

	exit(SENTIF_PROPER);
}

void bail_(const char* msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(SENTIF_BAIL);
}
