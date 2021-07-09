#pragma once

using ExecuteAtAppInitCode = void (*)();
struct ExecuteAtAppInit {
    ExecuteAtAppInit(ExecuteAtAppInitCode code) {
        code();
    }
};

#define EXECUTE_AT_APP_INIT_WITH_ID(identifier)                                                                 \
    void EXECUTE_AT_APP_INIT_##identifier##_code();                                                             \
    static ExecuteAtAppInit EXECUTE_AT_APP_INIT_##identifier##_object{EXECUTE_AT_APP_INIT_##identifier##_code}; \
    static void EXECUTE_AT_APP_INIT_##identifier##_code()

#define EXECUTE_AT_APP_INIT EXECUTE_AT_APP_INIT_WITH_ID(default)
