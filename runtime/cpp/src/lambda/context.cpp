
#include <cstdlib>
#include <cstring>

#include "lambda/context.hpp"

using namespace std;

namespace lambda
{
    Context::Context() : aws_request_id(""), log_stream_name(""), log_group_name(""), invoked_function_arn(""),
        function_name(""), function_version(""), memory_limit_in_mb(0), identity(nullptr), client_context(nullptr)
    {

    }

    Context::Context(Context&& move)
    {
        this->assign(&move);
    }

    Context::Context(const Context& copy)
    {
        this->copy(&copy);
    }

    Context& Context::operator=(const Context& other)
    {
        if (&other == this) {
            return *this;
        }

        if (this->identity != nullptr) {
            delete this->identity;
        }
        if (this->client_context != nullptr) {
            delete this->client_context;
        }
        this->copy(&other);

        return *this;
    }

    void Context::copy(const Context *other)
    {
        this->aws_request_id = other->aws_request_id;
        this->log_stream_name = other->log_stream_name;
        this->log_group_name = other->log_group_name;
        this->invoked_function_arn = other->invoked_function_arn;
        this->function_name = other->function_name;
        this->function_version = other->function_version;
        this->memory_limit_in_mb = other->memory_limit_in_mb;
        this->identity = new CognitoIdentity(*(other->identity));
        this->client_context = new ClientContext(*(other->client_context));
    }

    void Context::assign(const Context *other)
    {
        this->aws_request_id = other->aws_request_id;
        this->log_stream_name = other->log_stream_name;
        this->log_group_name = other->log_group_name;
        this->invoked_function_arn = other->invoked_function_arn;
        this->function_name = other->function_name;
        this->function_version = other->function_version;
        this->memory_limit_in_mb = other->memory_limit_in_mb;
        this->identity = other->identity;
        this->client_context = other->client_context;
    }

    Context::~Context()
    {
        if (this->identity != nullptr) {
            delete this->identity;
        }
        if (this->client_context != nullptr) {
            delete this->client_context;
        }
    }

    CognitoIdentity* Context::GetIdentity()
    {
        return this->identity;
    }

    void Context::SetIdentity(CognitoIdentity* identity)
    {
        this->identity = identity;
    }

    ClientContext* Context::GetClientContext()
    {
        return this->client_context;
    }

    void Context::SetClientContext(lambda::ClientContext* client_context)
    {
        this->client_context = client_context;
    }

    string Context::GetAwsRequestId()
    {
        return this->aws_request_id;
    }

    void Context::SetAwsRequestId(string aws_request_id)
    {
        this->aws_request_id = aws_request_id;
    }

    string Context::GetLogStreamName()
    {
        return this->log_stream_name;
    }

    void Context::SetLogStreamName(string log_stream_name)
    {
        this->log_stream_name = log_stream_name;
    }

    string Context::GetLogGroupName()
    {
        return this->log_group_name;
    }

    void Context::SetLogGroupName(string log_group_name)
    {
        this->log_group_name = log_group_name;
    }

    string Context::GetInvokedFunctionArn()
    {
        return this->invoked_function_arn;
    }

    void Context::SetInvokedFunctionArn(string invoked_function_arn)
    {
        this->invoked_function_arn = invoked_function_arn;
    }

    string Context::GetFunctionName()
    {
        return this->function_name;
    }

    void Context::SetFunctionName(string function_name)
    {
        this->function_name = function_name;
    }

    string Context::GetFunctionVersion()
    {
        return this->function_version = function_version;
    }

    void Context::SetFunctionVersion(string function_version)
    {
        this->function_version = function_version;
    }

    unsigned int Context::GetMemoryLimitInMb()
    {
        return this->memory_limit_in_mb;
    }

    void Context::SetMemoryLimitInMb(unsigned int memory_limit_in_mb)
    {
        this->memory_limit_in_mb = memory_limit_in_mb;
    }

    Logger* Context::GetLogger()
    {
        return this->logger;
    }

    void Context::SetLogger(Logger* logger)
    {
        this->logger = logger;
    }


    CognitoIdentity::CognitoIdentity() : cognito_identity_id(""), cognito_identity_pool_id("")
    {
    }

    CognitoIdentity::~CognitoIdentity()
    {
    }

    string CognitoIdentity::GetIdentityId()
    {
        return this->cognito_identity_id;
    }

    void CognitoIdentity::SetIdentityId(string identity_id)
    {
        this->cognito_identity_id = identity_id;
    }

    string CognitoIdentity::GetIdentityPoolId()
    {
        return this->cognito_identity_pool_id;
    }

    void CognitoIdentity::SetIdentityPoolId(string identity_pool_id)
    {
        this->cognito_identity_pool_id = identity_pool_id;
    }


    Client::Client() : installation_id(""), app_title(""), app_version_name(""), app_version_code(""), app_package_name("")
    {
    }

    Client::Client(const Client& copy)
    {
        this->installation_id = copy.installation_id;
        this->app_title = copy.app_title;
        this->app_version_name = copy.app_version_name;
        this->app_version_code = copy.app_version_code;
        this->app_package_name = copy.app_package_name;
    }

    Client::~Client()
    {
    }

    string Client::GetInstallationId()
    {
        return this->installation_id;
    }

    void Client::SetInstallationId(string installation_id)
    {
        this->installation_id = installation_id;
    }

    string Client::GetAppTitle()
    {
        return this->app_title;
    }
    
    void Client::SetAppTitle(string app_title)
    {
        this->app_title = app_title;
    }
    
    string Client::GetAppVersionName()
    {
        return this->app_version_name;
    }
    
    void Client::SetAppVersionName(string app_version_name)
    {
        this->app_version_name = app_version_name;
    }
    
    string Client::GetAppVersionCode()
    {
        return this->app_version_code;
    }
    
    void Client::SetAppVersionCode(string app_version_code)
    {
        this->app_version_code = app_version_code;
    }
    
    string Client::GetAppPackageName()
    {
        return this->app_package_name;
    }
    
    void Client::SetAppPackageName(string app_package_name)
    {
        this->app_package_name = app_package_name;
    }
    

    ClientContext::ClientContext()
    {
        this->client = nullptr;
        this->custom = map<string, string>();
        this->environment = map<string, string>();
    }
    
    ClientContext::ClientContext(ClientContext&& move)
    {
        this->assign(&move);
    }
    
    ClientContext::ClientContext(const ClientContext& copy)
    {
        this->copy(&copy);
    }
    
    ClientContext& ClientContext::operator=(const ClientContext& other)
    {
        if (&other == this) {
            return *this;
        }

        if (this->client != nullptr) {
            delete this->client;
        }
        this->copy(&other);

        return *this;
    }

    void ClientContext::copy(const ClientContext *other)
    {
        this->custom = other->custom;
        this->environment = other->environment;
        this->client = new Client(*(other->client));
    }

    void ClientContext::assign(const ClientContext *other)
    {
        this->custom = other->custom;
        this->environment = other->environment;
        this->client = other->client;
    }

    ClientContext::~ClientContext()
    {
        if (this->client != nullptr) {
            delete this->client;
        }
    }

    Client* ClientContext::GetClient()
    {
        return this->client;
    }

    void ClientContext::SetClient(Client* client)
    {
        this->client = client;
    }
    
    map<string, string> ClientContext::GetCustom()
    {
        return this->custom;
    }
    
    void ClientContext::SetCustom(map<string, string> custom)
    {
        this->custom = custom;
    }
    
    map<string, string> ClientContext::GetEnvironment()
    {
        return this->environment;
    }
    
    void ClientContext::SetEnvironment(map<string, string> environment)
    {
        this->environment = environment;
    }
    

    Logger::Logger(logger_function_t logger_function)
    {
        this->logger_function = logger_function;
    }

    Logger::~Logger()
    {

    }

    void Logger::Write(string message)
    {
        size_t orig_size = message.size();
        char *new_str = (char *) malloc(orig_size + 1);
        strncpy(new_str, message.c_str(), orig_size);
        new_str[orig_size] = 0;
        this->logger_function(new_str);
    }
}
