#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <sstream>

#include "json.hpp"
#include "lambda/context.hpp"

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


typedef nlohmann::json* (*remote_function_t)(nlohmann::json*, lambda::Context*);

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

lambda::Context* parse_context(nlohmann::json& json_context)
{
    /*
    "client_context": null, 
    */
    auto context = new lambda::Context();
    if (json_context.at("aws_request_id").is_string()) {
        context->SetAwsRequestId(json_context.at("aws_request_id").get<std::string>());
    }
    if (json_context.at("log_stream_name").is_string()) {
        context->SetLogStreamName(json_context.at("log_stream_name").get<std::string>());
    }
    if (json_context.at("log_group_name").is_string()) {
        context->SetLogGroupName(json_context.at("log_group_name").get<std::string>());
    }
    if (json_context.at("invoked_function_arn").is_string()) {
        context->SetInvokedFunctionArn(json_context.at("invoked_function_arn").get<std::string>());
    }
    if (json_context.at("function_name").is_string()) {
        context->SetFunctionName(json_context.at("function_name").get<std::string>());
    }
    if (json_context.at("function_version").is_string()) {
        context->SetFunctionVersion(json_context.at("function_version").get<std::string>());
    }
    if (json_context.at("memory_limit_in_mb").is_string()) {
        std::string memory_limit_str = json_context.at("memory_limit_in_mb").get<std::string>();
        unsigned int memory_limit = 0;
        std::istringstream iss(memory_limit_str);
        iss >> memory_limit;
        context->SetMemoryLimitInMb(memory_limit);
    }
    if (json_context.at("identity").is_object()) {
        lambda::CognitoIdentity* identity = new lambda::CognitoIdentity();
        if (json_context.at("identity").at("cognito_identity_id").is_string()) {
            identity->SetIdentityId(json_context.at("identity").at("cognito_identity_id").get<std::string>());
        }
        if (json_context.at("identity").at("cognito_identity_pool_id").is_string()) {
            identity->SetIdentityPoolId(json_context.at("identity").at("cognito_identity_pool_id").get<std::string>());
        }
        context->SetIdentity(identity);
    }

    return context;
}

struct handler_return handle_request(const char *event_str, const char *context_str, const char *environment_str)
{
    setup_environment(environment_str);

    nlohmann::json* event = new nlohmann::json(nlohmann::json::parse(event_str));

    auto logger = new lambda::Logger(runtime_log);

    nlohmann::json json_context = nlohmann::json::parse(context_str);
    auto context = parse_context(json_context);
    context->SetLogger(logger);

    nlohmann::json* response = remote_function(event, context);

    char* payload = new_string(response->dump().c_str());

    delete response;
    delete logger;
    delete context;
    delete event;

    struct handler_return return_value;
    return_value.payload = payload;
    return_value.error.message = NULL;
    return_value.error.type = NULL;
    return return_value;
}
