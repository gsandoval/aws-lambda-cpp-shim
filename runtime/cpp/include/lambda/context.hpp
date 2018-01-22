#ifndef LAMBDA_CONTEXT_H_
#define LAMBDA_CONTEXT_H_

#include <string>
#include <map>

namespace lambda
{
    class CognitoIdentity;
    class ClientContext;
    class Client;
    class Logger;

    class Context
    {
    private:
        std::string aws_request_id;
        std::string log_stream_name;
        std::string log_group_name;
        std::string invoked_function_arn;
        std::string function_name;
        std::string function_version;
        unsigned int memory_limit_in_mb;
        CognitoIdentity* identity;
        ClientContext* client_context;
        Logger* logger;

        void assign(const Context*);
        void copy(const Context*);

    public:
        Context();
        Context(Context&& move);
        Context(const Context& copy);
        Context& operator=(const Context&);
        virtual ~Context();

        CognitoIdentity* GetIdentity();
        void SetIdentity(CognitoIdentity* identity);
        ClientContext* GetClientContext();
        void SetClientContext(lambda::ClientContext* client_context);
        std::string GetAwsRequestId();
        void SetAwsRequestId(std::string aws_request_id);
        std::string GetLogStreamName();
        void SetLogStreamName(std::string log_stream_name);
        std::string GetLogGroupName();
        void SetLogGroupName(std::string log_group_name);
        std::string GetInvokedFunctionArn();
        void SetInvokedFunctionArn(std::string invoked_function_arn);
        std::string GetFunctionName();
        void SetFunctionName(std::string function_name);
        std::string GetFunctionVersion();
        void SetFunctionVersion(std::string function_version);
        unsigned int GetMemoryLimitInMb();
        void SetMemoryLimitInMb(unsigned int memory_limit_in_mb);
        Logger* GetLogger();
        void SetLogger(Logger*);
    };

    typedef void (*logger_function_t)(char *);
    class Logger
    {
    private:
        logger_function_t logger_function;
    public:
        Logger(logger_function_t logger_function);
        ~Logger();

        void Write(std::string);
    };

    class CognitoIdentity
    {
    private:
        std::string cognito_identity_id;
        std::string cognito_identity_pool_id;

    public:
        CognitoIdentity();
        virtual ~CognitoIdentity();

        std::string GetIdentityId();
        void SetIdentityId(std::string identity_id);
        std::string GetIdentityPoolId();
        void SetIdentityPoolId(std::string identity_pool_id);
    };

    class ClientContext
    {
    private:
        Client* client;
        std::map<std::string, std::string> custom;
        std::map<std::string, std::string> environment;

        void copy(const ClientContext *other);
        void assign(const ClientContext *other);

    public:
        ClientContext();
        ClientContext(ClientContext&& move);
        ClientContext(const ClientContext& copy);
        ClientContext& operator=(const ClientContext&);
        virtual ~ClientContext();
        
        Client* GetClient();
        void SetClient(Client* client);
        std::map<std::string, std::string> GetCustom();
        void SetCustom(std::map<std::string, std::string> custom);
        std::map<std::string, std::string> GetEnvironment();
        void SetEnvironment(std::map<std::string, std::string> environment);
    };

    class Client
    {
    private:
        std::string installation_id;
        std::string app_title;
        std::string app_version_name;
        std::string app_version_code;
        std::string app_package_name;

    public:
        Client();
        Client(const Client& copy);
        virtual ~Client();

        std::string GetInstallationId();
        void SetInstallationId(std::string installation_id);
        std::string GetAppTitle();
        void SetAppTitle(std::string app_title);
        std::string GetAppVersionName();
        void SetAppVersionName(std::string app_version_name);
        std::string GetAppVersionCode();
        void SetAppVersionCode(std::string app_version_code);
        std::string GetAppPackageName();
        void SetAppPackageName(std::string app_package_name);
    };
}

#endif // LAMBDA_CONTEXT_H_
