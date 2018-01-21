#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>

#include "json.hpp"

extern "C" {
#include "handler_response.h"
}

char* new_string(const char *orig)
{
    size_t orig_size = strlen(orig);
    char *new_str = (char *)malloc(orig_size + 1);
    strncpy(new_str, orig, orig_size);
    new_str[orig_size] = 0;
    return new_str;
}


typedef nlohmann::json* (*remote_function_t)(nlohmann::json*, nlohmann::json*);

void* library_handle;
remote_function_t remote_function;

void setup_environment(const char* environment_str)
{
    if (strlen(environment_str) > 0) {
        nlohmann::json environment = nlohmann::json::parse(environment_str);
        for (auto& entry : nlohmann::json::iterator_wrapper(environment)) {
            setenv(entry.key().c_str(), entry.value().get<std::string>().c_str(), 1);
        }
    }
}

char* open_dynamic_library(const char* path, const char* environment_str)
{
    setup_environment(environment_str);

    std::string library_path(path);
    library_path.append(".so");

    library_handle = dlopen(library_path.c_str(), RTLD_LAZY);
    if (!library_handle) {
        return dlerror();
    }
    
	return 0;
}

char* symbol_lookup(const char *name)
{
    dlerror();

    remote_function = (remote_function_t) dlsym(library_handle, name);
    char *dlsym_error = dlerror();
    if (dlsym_error) {
        return dlsym_error;
    }
    
    return 0;
}

struct handler_return handle_request(const char *event_str, const char *context_str, const char *environment_str)
{
    setup_environment(environment_str);

    runtime_log(new_string("handle_request event"));
    runtime_log(new_string(event_str));
    runtime_log(new_string("handle_request context"));
    runtime_log(new_string(context_str));
    runtime_log(new_string("handle_request environment"));
    runtime_log(new_string(environment_str));

    nlohmann::json* event = new nlohmann::json(nlohmann::json::parse(event_str));
    nlohmann::json* context = new nlohmann::json(nlohmann::json::parse(context_str));

    nlohmann::json* response = remote_function(event, context);

    char* payload = new_string(response->dump().c_str());

    delete response;

    struct handler_return return_value;
    return_value.payload = payload;
    return_value.error.message = NULL;
    return_value.error.type = NULL;
    return return_value;
}
