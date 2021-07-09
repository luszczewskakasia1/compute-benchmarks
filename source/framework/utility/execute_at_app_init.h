#pragma once

#define EXECUTE_AT_APP_INIT(identifier)                                                  \
    struct EXECUTE_AT_APP_INIT_##identifier {                                            \
        EXECUTE_AT_APP_INIT_##identifier();                                              \
    };                                                                                   \
                                                                                         \
    static EXECUTE_AT_APP_INIT_##identifier EXECUTE_AT_APP_INIT_##identifier##_object{}; \
    EXECUTE_AT_APP_INIT_##identifier::EXECUTE_AT_APP_INIT_##identifier()
