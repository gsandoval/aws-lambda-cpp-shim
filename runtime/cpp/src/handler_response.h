#ifndef HANDLER_RESPONSE_H_
#define HANDLER_RESPONSE_H_

#ifdef __cplusplus
extern "C" {
#endif

void runtime_log(char *msg);
long long runtime_rtm();

#ifdef __cplusplus
}
#endif

struct handler_error
{
    char* message;
    char* type;
};

struct handler_return 
{
    char* payload;
    struct handler_error error;
};

char* open_dynamic_library(const char* path, const char* env);
char* symbol_lookup(const char *name);
struct handler_return handle_request(const char *event, const char *context, const char *environment);


#endif // HANDLER_RESPONSE_H_